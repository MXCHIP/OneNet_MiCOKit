#include "regina_tcp_protocol.h"
#include "regina/allocator.h"
#include "util.h"
#include "extent.h"
#include "common.h"
#include "stream.h"
#include "compression.h"
#include "security.h"
#include "std_wrapper.h"
#include "device_status.h"
#include "regina_config.h"


#define AES_SESSION_KEY_LEN 16


#define regina_tcp_pro_wb16(v, out) do { \
        uint8_t *uo = (uint8_t*)out; \
        uint16_t uv = (uint16_t)v; \
        uo[0] = (uint8_t)(uv >> 8); \
        uo[1] = (uint8_t)(uv);\
    } while(0)

struct regina_tcp_pro_t {
    regina_tcp_pro_handler_t *handler;
    compression_codec_t *comp_codec;
    crypto_codec_t *crypto_codec;
};


typedef int (*crypto_codec_fn)(crypto_codec_t *codec, const char *data,
                               size_t size, regina_stream_t *output);
typedef int (*crypto_codec_flush_fn)(crypto_codec_t *codec, regina_stream_t *output);
static int regina_tcp_pro_crypto_codec_stream(crypto_codec_t *codec,
                                              regina_stream_t *input,
                                              size_t size,
                                              crypto_codec_fn codec_fun,
                                              crypto_codec_flush_fn codec_flush_fun,
                                              regina_stream_t *output)
{
    struct regina_extent_t extent;
    size_t count;
    int err = REGINA_ERR_NONE;

    while (size) {
        count = 1;
        regina_stream_extract(input, &extent, &count);

        /* assert (count); */

        if (extent.len <= size) {
            err = codec_fun(codec, extent.base, extent.len, output);
            size -= extent.len;
            regina_stream_erase(input, extent.len);
        }
        else {
            err = codec_fun(codec, extent.base, size, output);
            regina_stream_erase(input, size);
            size = 0;
        }

        if (REGINA_ERR_NONE != err)
            break;
    }

    if (REGINA_ERR_NONE == err)
        err = codec_flush_fun(codec, output);

    return err;

}

#define regina_tcp_pro_encrypt_stream(codec, input, size, output) \
    regina_tcp_pro_crypto_codec_stream(codec, input, size, \
                                       security_crypto_codec_encode, \
                                       security_crypto_codec_flush_encoder, \
                                       output)

#define regina_tcp_pro_decrypt_stream(codec, input, size, output) \
    regina_tcp_pro_crypto_codec_stream(codec, input, size, \
                                       security_crypto_codec_decode, \
                                       security_crypto_codec_flush_decoder, \
                                       output)


typedef int compression_codec_fn(compression_codec_t *codec, const char *data,
                                 size_t size, regina_stream_t *output);
static int regina_tcp_pro_compression_codec_stream(compression_codec_t *codec,
                                                   regina_stream_t *input,
                                                   size_t size,
                                                   compression_codec_fn codec_fun,
                                                   regina_stream_t *output)
{
    struct regina_extent_t extent;
    size_t count;
    int err = REGINA_ERR_NONE;

    while (size) {
        count = 1;
        regina_stream_extract(input, &extent, &count);

        /* assert(count); */

        if (extent.len <= size) {
            err = codec_fun(codec, extent.base, extent.len, output);
            size -= extent.len;
            regina_stream_erase(input, extent.len);
        }
        else {
            err = codec_fun(codec, extent.base, size, output);
            regina_stream_erase(input, size);
            size = 0;
        }

        if (REGINA_ERR_NONE != err)
            break;
    }

    return err;
}

#define regina_tcp_pro_compress_stream(codec, input, size, output)    \
    regina_tcp_pro_compression_codec_stream(codec, input, size,         \
                                            compression_codec_encode, output)

#define regina_tcp_pro_decompress_stream(codec, input, size, output) \
    regina_tcp_pro_compression_codec_stream(codec, input, size, \
                                            compression_codec_decode, output)

static int regina_tcp_pro_write_fixed_header(char pkt_id, size_t length, char *out)
{
    int i;

    *(out++) = pkt_id << 4;
    for(i = 1; i <= 4; ++i) {
        *((uint8_t*)out) = length % 128;
        length /= 128;
        if(length > 0) {
            *out |= 128;
            ++out;
        }
        else {
            return i + 1; /* one byte for packet id */
        }
    }

    return 0;
}


/* payload must be destroy by regina_free when no use. */
static int regina_tcp_pro_fill_bootstrap_connect_payload(
    const char *session_key, size_t size,
    regina_session_key_type_t acc_key_type,
    char **payload, size_t *payload_size)
{
    int err;
    regina_bytes_t *key;
    regina_access_request_t *req;

    req = regina_access_request_create();
    if (!req)
        return REGINA_ERR_NOMEM;

    if (device_status_get_mode() == DEVICE_MODE_NORMAL) {
        req->mode = ENUM_DEVICE_MODE_NORMAL;
    }
    else {
        req->mode = ENUM_DEVICE_MODE_DEBUG;
    }

    req->session_key = regina_union_null_or_bytes_branch_1_create();
    if (!req->session_key) {
        req->destroy(req);
        return REGINA_ERR_NOMEM;
    }

    key = regina_bytes_move_create((const uint8_t*)session_key, size, NULL);
    if (!key) {
        req->destroy(req);
        return REGINA_ERR_NOMEM;
    }

    req->sdk_uuid = regina_string_move_create(regina_config_get_sdk_uuid(), NULL);
    if (!req->sdk_uuid) {
        req->destroy(req);
        return REGINA_ERR_NOMEM;
    }

    req->session_key->data = key;
    req->acc_session_key_type = acc_key_type;

    err = REGINA_ERR_NOMEM;
    *payload_size = req->get_size(req);
    *payload = (char*)regina_malloc(*payload_size);
    if (*payload) {
        avro_writer_t writer = avro_writer_memory(*payload, *payload_size);
        if (writer) {
            req->serialize(writer, req);
            avro_writer_free(writer);
            err = REGINA_ERR_NONE;
        }
    }

    req->destroy(req);
    return err;
}

static int regina_tcp_pro_do_pack_bootstrap_connectpkt(const char *pub_key, size_t pbk_len,
                                                       const char *payload, size_t size,
                                                       regina_stream_t *pkt)
{
    const size_t var_header_length = 2;
    size_t remaining_length;
    char fixed_header[5];
    char var_header[2];
    int fixed_header_len, err;
    crypto_codec_t *rsa_codec;
    regina_stream_t enc_payload[1];

    err = security_crypto_rsa_codec_create(&rsa_codec, pub_key, pbk_len, NULL, 0);
    REGINA_RETURN_IF_ERR(err);

    regina_stream_init(enc_payload);
    err = security_crypto_codec_encode(rsa_codec, payload, size, enc_payload);
    if (REGINA_ERR_NONE == err)
        err = security_crypto_codec_flush_encoder(rsa_codec, enc_payload);
    security_crypto_codec_destroy(rsa_codec);

    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    size = regina_stream_get_size(enc_payload);
    remaining_length = size + var_header_length;
    fixed_header_len = regina_tcp_pro_write_fixed_header(
        REGINA_TCP_PRO_PKT_CONNECT, remaining_length, fixed_header);

    if (0 == fixed_header_len) {
        regina_stream_destroy(enc_payload);
        return REGINA_ERR_INTERNAL;
    }

    err = regina_stream_write(pkt, fixed_header, (size_t)fixed_header_len);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    var_header[0] = 0x01;
    var_header[1] = REGINA_TCP_PRO_CRYPTO_ALG_AES;

    err = regina_stream_write(pkt, var_header, var_header_length);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    err = regina_stream_write_stream(pkt, enc_payload);
    regina_stream_destroy(enc_payload);

    return err;
}

int regina_tcp_pro_create(regina_tcp_pro_t **protocol, regina_tcp_pro_handler_t *handler)
{
    int err;
    regina_tcp_pro_t *pro;

    REGINA_RETURN_IF_NIL2(protocol, handler, REGINA_ERR_BADPARAM);
    pro = (regina_tcp_pro_t*)regina_malloc(sizeof(regina_tcp_pro_t));
    if (!pro)
        return REGINA_ERR_NOMEM;

    regina_memset(pro, 0, sizeof(regina_tcp_pro_t));
    pro->handler = handler;
    err = security_crypto_aes_codec_create(&pro->crypto_codec, AES_SESSION_KEY_LEN);
    if (REGINA_ERR_NONE != err) {
        pro->crypto_codec = NULL;
        regina_tcp_pro_destroy(pro);
        return err;
    }

    *protocol = pro;
    return REGINA_ERR_NONE;
}

void regina_tcp_pro_destroy(regina_tcp_pro_t *protocol)
{
    if (!protocol)
        return;

    security_crypto_codec_destroy(protocol->crypto_codec);

    regina_free(protocol);
}

int regina_tcp_pro_set_session_key(regina_tcp_pro_t *protocol,
                                   const char *key, size_t size,
                                   regina_tcp_pro_crypto_alg_t alg)
{
    crypto_codec_t *codec;
    int err;

    if (alg != REGINA_TCP_PRO_CRYPTO_ALG_AES)
        return REGINA_ERR_UNSUPPORTED;

    if (key && size)
        err = security_crypto_aes_codec_create_ex(&codec, key, size);
    else
        err = security_crypto_aes_codec_create(&codec, 16);

    REGINA_RETURN_IF_ERR(err);

    security_crypto_codec_destroy(protocol->crypto_codec);
    protocol->crypto_codec = codec;

    return REGINA_ERR_NONE;
}

int regina_tcp_pro_pack_bootstrap_connectpkt(regina_tcp_pro_t *protocol,
                                             const char *pub_key, size_t pbk_len,
                                             regina_session_key_type_t acc_key_type,
                                             regina_stream_t *pkt)
{
    int err;
    char key[16];
    char *payload;
    size_t size;

    security_crypto_codec_get_key(protocol->crypto_codec, key);
    err = regina_tcp_pro_fill_bootstrap_connect_payload(key, 16, acc_key_type,
                                                        &payload, &size);
    REGINA_RETURN_IF_ERR(err);

    err = regina_tcp_pro_do_pack_bootstrap_connectpkt(pub_key, pbk_len, payload, size, pkt);
    regina_free(payload);
    return err;
}

int regina_tcp_pro_pack_acceptor_connectpkt(regina_tcp_pro_t *protocol,
                                            uint16_t keepalive,
                                            const char *token, size_t tk_len,
                                            regina_stream_t *payload,
                                            struct regina_stream_t *pkt)
{
    regina_stream_t enc_payload[1];
    char fixed_header[5];
    int fixed_header_len, err;
    size_t var_header_len;
    char *var_header;
    size_t size;

    regina_stream_init(enc_payload);

    size = regina_stream_get_size(payload);
    err = regina_tcp_pro_encrypt_stream(protocol->crypto_codec, payload, size, enc_payload);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    size = regina_stream_get_size(enc_payload);
    var_header_len = 6 + tk_len;
    fixed_header_len = regina_tcp_pro_write_fixed_header(
        REGINA_TCP_PRO_PKT_CONNECT, var_header_len + size, fixed_header);

    if (0 == fixed_header_len) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    err = regina_stream_write(pkt, fixed_header, (size_t)fixed_header_len);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    err = regina_stream_reserve_next(pkt, &var_header, var_header_len);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(enc_payload);
        return err;
    }

    var_header[0] = 0x01;
    var_header[1] = REGINA_TCP_PRO_CRYPTO_ALG_AES;
    regina_tcp_pro_wb16(keepalive, var_header + 2);
    regina_tcp_pro_wb16(tk_len, var_header + 4);
    regina_memcpy(var_header + 6, token, tk_len);

    err = regina_stream_write_stream(pkt, enc_payload);
    regina_stream_destroy(enc_payload);
    return err;
}

int regina_tcp_pro_pack_ping_pkt(regina_tcp_pro_t *pro, struct regina_stream_t *pkt)
{
    char *buf;
    int err;

    REGINA_RETURN_IF_NIL2(pro, pkt, REGINA_ERR_BADPARAM);

    err = regina_stream_reserve_next(pkt, &buf, 2);
    REGINA_RETURN_IF_ERR(err);

    buf[0] = REGINA_TCP_PRO_PKT_PINGREQ << 4;
    buf[1] = 0;
    return REGINA_ERR_NONE;
}


int regina_tcp_pro_pack_sync_pkt(regina_tcp_pro_t *pro,
                                 int comp, int enc,
                                 regina_stream_t *payload,
                                 struct regina_stream_t *pkt)
{

    int err;
    int destroy;
    char fixed_header[5];
    const size_t var_header_len = 1;
    size_t remaining_len, size;
    char var_header[1];
    int fixed_header_len;
    regina_stream_t comp_payload[1];
    regina_stream_t enc_payload[1];


    var_header[0] = 0;
    destroy = 0;

    size = regina_stream_get_size(payload);
    if (comp) {
        regina_stream_init(comp_payload);
        err = regina_tcp_pro_compress_stream(pro->comp_codec, payload,
                                             size, comp_payload);
        REGINA_RETURN_IF_ERR(err);
        payload = comp_payload;
        destroy = 1;
        size = regina_stream_get_size(payload);
        var_header[0] |= REGINA_TCP_PRO_COMP_ALG_ZIP << 3;
    }

    if (enc) {
        regina_stream_init(enc_payload);
        err = regina_tcp_pro_encrypt_stream(pro->crypto_codec, payload,
                                            size, enc_payload);
        if (destroy)
            regina_stream_destroy(payload);

        REGINA_RETURN_IF_ERR(err);
        payload = enc_payload;
        destroy = 1;
        size = regina_stream_get_size(payload);
        var_header[0] |= REGINA_TCP_PRO_CRYPTO_ALG_AES;
    }

    remaining_len = var_header_len + size;
    fixed_header_len = regina_tcp_pro_write_fixed_header(
        REGINA_TCP_PRO_PKT_SYNC, remaining_len, fixed_header);
    if (0 == fixed_header_len)
        return REGINA_ERR_INTERNAL;

    err = regina_stream_write(pkt, fixed_header, fixed_header_len);
    if (REGINA_ERR_NONE == err)
        err = regina_stream_write(pkt, var_header, var_header_len);
    if (REGINA_ERR_NONE == err)
        err = regina_stream_write_stream(pkt, payload);

    if (destroy)
        regina_stream_destroy(payload);

    return err;
}

int regina_tcp_pro_pack_disconnect_pkt(regina_tcp_pro_t *protocol,
                                       regina_tcp_pro_disconn_reason_t reason,
                                       struct regina_stream_t *pkt)
{
    char *buf;
    int err;

    (void)protocol;

    err = regina_stream_reserve_next(pkt, &buf, 3);
    REGINA_RETURN_IF_ERR(err);

    buf[0] = REGINA_TCP_PRO_PKT_DISCONNECT << 4;
    buf[1] = 1;
    buf[2] = reason;

    return REGINA_ERR_NONE;
}


static int regina_tcp_pro_handle_connack_pkt(regina_tcp_pro_t *pro,
                                             regina_stream_t *stream,
                                             size_t size)
{
    char var_header;
    regina_stream_t decrypted_payload[1];
    char *buf;
    int err;

    if (1 > size)
        return REGINA_ERR_PROTOCOL_ERROR;

    if (!pro->handler || !pro->handler->handle_conn_ack) {
        regina_stream_erase(stream, size);
        return REGINA_ERR_NONE;
    }

    regina_stream_read(stream, &var_header, 1);
    size -= 1;

    buf = NULL;
    if (0 != size) {
        regina_stream_init(decrypted_payload);
        err = regina_tcp_pro_decrypt_stream(pro->crypto_codec, stream,
                                            size, decrypted_payload);

        if (REGINA_ERR_NONE != err) {
            regina_stream_destroy(decrypted_payload);
            return err;
        }

        size = regina_stream_get_size(decrypted_payload);
        buf = (char*)regina_malloc(size);

        if (!buf) {
            regina_stream_destroy(decrypted_payload);
            return REGINA_ERR_NOMEM;
        }

        regina_stream_read(decrypted_payload, buf, size);
        regina_stream_destroy(decrypted_payload);
    }

    err = pro->handler->handle_conn_ack(pro->handler->context,
                                        (regina_tcp_pro_connack_t)var_header,
                                        buf, size);
    regina_free(buf);

    return err;
}

static int regina_tcp_pro_handle_pingresp_pkt(regina_tcp_pro_t *pro,
                                              regina_stream_t *stream,
                                              size_t size)
{
    if (0 != size)
        return REGINA_ERR_PROTOCOL_ERROR;

    if (!pro->handler || !pro->handler->handle_ping_resp)
        return REGINA_ERR_NONE;

    pro->handler->handle_ping_resp(pro->handler->context);
    return REGINA_ERR_NONE;
}


static int regina_tcp_pro_handle_sync_pkt(regina_tcp_pro_t *pro,
                                          regina_stream_t *stream,
                                          size_t size)
{
    char flags;
    char *buf;
    int err;
    int destroy;
    regina_stream_t decrypted_payload[1];
    char crypto_alg, compress_alg;

    if (size < 1)
        return REGINA_ERR_PROTOCOL_ERROR;

    if (!pro->handler || !pro->handler->handle_sync) {
        regina_stream_erase(stream, size);
        return REGINA_ERR_NONE;
    }

    regina_stream_read(stream, &flags, 1);
    size -= 1;
    destroy = 0;

    crypto_alg = flags & 0x07;
    switch (crypto_alg) {
        case REGINA_TCP_PRO_CRYPTO_ALG_AES: {
            regina_stream_init(decrypted_payload);
            err = regina_tcp_pro_decrypt_stream(pro->crypto_codec, stream,
                                                size, decrypted_payload);
            REGINA_RETURN_IF_ERR(err);
            destroy = 1;
            stream = decrypted_payload;
            size = regina_stream_get_size(stream);
            break;
        }

        default:
            return REGINA_ERR_UNSUPPORTED;
    }

    compress_alg = (((unsigned char)flags) >> 3) & 0x07;
    if (REGINA_TCP_PRO_COMP_ALG_NONE != compress_alg)
        return REGINA_ERR_UNSUPPORTED;

    buf = (char*)regina_malloc(size);
    if (buf) {
        regina_stream_read(stream, buf, size);
        err = pro->handler->handle_sync(pro->handler->context, buf, size);
        regina_free(buf);
    }
    else
        err = REGINA_ERR_NOMEM;

    if (destroy)
        regina_stream_destroy(stream);
    return err;
}

static int regina_tcp_pro_handle_disconnect_pkt(regina_tcp_pro_t *pro,
                                                regina_stream_t *stream,
                                                size_t size)
{
    char var_header;

    if (1 != size)
        return REGINA_ERR_PROTOCOL_ERROR;

    if (!pro->handler || !pro->handler->handle_disconnect) {
        regina_stream_erase(stream, size);
        return REGINA_ERR_NONE;
    }

    regina_stream_read(stream, &var_header, 1);
    if ((var_header < 0) || (var_header >= REGINA_TCP_PRO_DISCONN_REASON_COUNT))
        return REGINA_ERR_PROTOCOL_ERROR;

    pro->handler->handle_disconnect(pro->handler->context,
                                    (regina_tcp_pro_disconn_reason_t)var_header);
    return REGINA_ERR_NONE;
}


int regina_tcp_pro_handle_inbound_pkt(regina_tcp_pro_t *protocol,
                                      regina_stream_t *stream)
{
    char pkt_id;
    char c;
    size_t offset, size, remaining_length, multiplier;
    int err;
    const int PKT_NOT_COMPLETE = 0;
    const int PKT_HANDLED = 1;

    offset = 0;
    if (REGINA_ERR_NONE != regina_stream_peek(stream, &c, offset))
        return PKT_NOT_COMPLETE;

    pkt_id = ((uint8_t)c) >> 4;
    ++offset;
    remaining_length = 0;
    multiplier = 1;
    while (1) {
        if (REGINA_ERR_NONE != regina_stream_peek(stream, &c, offset))
            return PKT_NOT_COMPLETE;

        ++offset;

        remaining_length += (c & 0x7f) * multiplier;
        if (!(c & 0x80))
            break;

        multiplier *= 128;
        if (multiplier > 128 * 128 * 128)
            return REGINA_ERR_PROTOCOL_ERROR;
    }

    size = regina_stream_get_size(stream);
    if (size < offset + remaining_length)
        return PKT_NOT_COMPLETE;

    regina_stream_erase(stream, offset);
    switch (pkt_id) {
    case REGINA_TCP_PRO_PKT_CONNACK:
        err = regina_tcp_pro_handle_connack_pkt(protocol, stream,
                                                remaining_length);
        break;

    case REGINA_TCP_PRO_PKT_PINGRESP:
        err = regina_tcp_pro_handle_pingresp_pkt(protocol, stream,
                                                 remaining_length);
        break;

    case REGINA_TCP_PRO_PKT_SYNC:
        err = regina_tcp_pro_handle_sync_pkt(protocol, stream,
                                             remaining_length);
        break;

    case REGINA_TCP_PRO_PKT_DISCONNECT:
        err = regina_tcp_pro_handle_disconnect_pkt(protocol, stream,
                                                   remaining_length);
        break;

    default:
        return REGINA_ERR_PROTOCOL_ERROR;
    }

    if (REGINA_ERR_NONE != err)
        return err;

    return PKT_HANDLED;
}


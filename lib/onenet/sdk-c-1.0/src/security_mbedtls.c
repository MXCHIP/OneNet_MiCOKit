#include "security.h"
#include <mbedtls/platform.h>
#include <mbedtls/aes.h>
#include <mbedtls/md5.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/pk.h>
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "stream.h"
#include "std_wrapper.h"

typedef enum {
    CRYPTO_CODEC_AES,
    CRYPTO_CODEC_RSA,

    CRYPTO_CODEC_COUNT
} crypto_codec_type_t;

struct crypto_codec_t {
    crypto_codec_type_t type;
};


/*-------------------------------- AES CODEC -------------------------------------*/

/**
 * AES/CBC/PKCS7Padding
 */
typedef struct aes_codec_t {
    crypto_codec_t base;

    unsigned char key[32];
    unsigned int keybits;

    mbedtls_aes_context enc_ctx[1];
    mbedtls_aes_context dec_ctx[1];

    unsigned char unenc_block[16];
    unsigned char undec_block[16];
    int unenc_block_size;
    int undec_block_size;

    unsigned char enc_iv[16];
    unsigned char dec_iv[16];
} aes_codec_t;

#if defined(MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES)
static int security_crypto_entropy_source(void *data, unsigned char *output,
                                          size_t len, size_t *olen)
{
    uint32_t seed = (uint32_t)regina_time(NULL);
    (void)data;

    *olen = len;

    while (len > 0) {
        if (len >= sizeof(uint32_t))
            *((uint32_t*)output) = REGINA_RANDOM(seed);
        else {
            seed = REGINA_RANDOM(seed);
            regina_memcpy(output, &seed, len);
            break;
        }

        len -= sizeof(uint32_t);
        output += sizeof(uint32_t);
        seed += 17;
    }
    return 0;
}
#endif /* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES */

int security_crypto_aes_codec_create(crypto_codec_t **codec, size_t bytes)
{
    mbedtls_ctr_drbg_context ctr_drbg[1];
    mbedtls_entropy_context entropy[1];
    int err;
    char key[32];

    if ((16 != bytes) && (24 != bytes) && (32 != bytes))
        return REGINA_ERR_BADPARAM;

    mbedtls_entropy_init(entropy);

#if defined(MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES)
    mbedtls_entropy_add_source(entropy, security_crypto_entropy_source,
                               NULL, 32, MBEDTLS_ENTROPY_SOURCE_STRONG);
#endif /* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES */

    mbedtls_ctr_drbg_init(ctr_drbg);
    err = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy, NULL, 0);
    if (0 == err) {
        err = mbedtls_ctr_drbg_random(ctr_drbg, (unsigned char*)key, bytes);
        if (0 == err)
            security_crypto_aes_codec_create_ex(codec, key, bytes);

        mbedtls_ctr_drbg_free(ctr_drbg);
    }

    mbedtls_entropy_free(entropy);
    return (0 == err) ? REGINA_ERR_NONE : REGINA_ERR_INTERNAL;
}

int security_crypto_aes_codec_create_ex(crypto_codec_t **codec,
                                        const char *key, size_t bytes)
{
    aes_codec_t *aes_codec;

    REGINA_RETURN_IF_NIL2(codec, key, REGINA_ERR_BADPARAM);
    if ((16 != bytes) && (24 != bytes) && (32 != bytes))
        return REGINA_ERR_BADPARAM;

    aes_codec = (aes_codec_t*)regina_malloc(sizeof(aes_codec_t));
    REGINA_RETURN_IF_NIL(aes_codec, REGINA_ERR_NOMEM);

    regina_memset(aes_codec, 0, sizeof(aes_codec_t));
    aes_codec->base.type = CRYPTO_CODEC_AES;
    *codec = &aes_codec->base;

    regina_memcpy(aes_codec->key, key, bytes);
    aes_codec->keybits = bytes * 8;

    mbedtls_aes_init(aes_codec->enc_ctx);
    mbedtls_aes_init(aes_codec->dec_ctx);

    mbedtls_aes_setkey_enc(aes_codec->enc_ctx, aes_codec->key, aes_codec->keybits);
    mbedtls_aes_setkey_dec(aes_codec->dec_ctx, aes_codec->key, aes_codec->keybits);

    return REGINA_ERR_NONE;

}

static void security_crypto_aes_codec_destroy(crypto_codec_t *codec)
{
    aes_codec_t *aes_codec;

    if (!codec)
        return;

    aes_codec = TYPE_CAST(codec, aes_codec_t, base);
    mbedtls_aes_free(aes_codec->enc_ctx);
    mbedtls_aes_free(aes_codec->dec_ctx);

    regina_free(aes_codec);
}

static int security_crypto_aes_codec_encdec(mbedtls_aes_context *ctx,
                                            unsigned char iv[16],
                                            unsigned char uncodec_block[16],
                                            int *uncodec_block_size,
                                            const char *input, size_t size,
                                            regina_stream_t *output,
                                            int mode)
{
    size_t out_size;
    char *buf;
    int err;
    size_t remaining_size;

    out_size = size + (size_t)(*uncodec_block_size);
    remaining_size = out_size % 16;
    if (0 == remaining_size)
        remaining_size = 16; /* may be the last block, save it casually */

    out_size -= remaining_size;

    err = regina_stream_reserve_next(output, &buf, out_size);
    REGINA_RETURN_IF_ERR(err);


    if (*uncodec_block_size) {
        regina_memcpy(uncodec_block + *uncodec_block_size, input, 16 - *uncodec_block_size);
        mbedtls_aes_crypt_cbc(ctx, mode, 16, iv, uncodec_block, (unsigned char*)buf);

        out_size -= 16;
        buf += 16;
        input += 16 - *uncodec_block_size;
    }

    mbedtls_aes_crypt_cbc(ctx, mode, out_size, iv,
                          (const unsigned char*)input, (unsigned char*)buf);

    *uncodec_block_size = (int)remaining_size;
    regina_memcpy(uncodec_block, input + out_size, remaining_size);

    return REGINA_ERR_NONE;
}

static int security_crypto_aes_codec_encode(crypto_codec_t *codec,
                                            const char *input, size_t size,
                                            regina_stream_t *output)
{
    aes_codec_t *aes_codec = TYPE_CAST(codec, aes_codec_t, base);
    return security_crypto_aes_codec_encdec(aes_codec->enc_ctx,
                                            aes_codec->enc_iv,
                                            aes_codec->unenc_block,
                                            &aes_codec->unenc_block_size,
                                            input, size, output,
                                            MBEDTLS_AES_ENCRYPT);
}

static int security_crypto_aes_codec_decode(crypto_codec_t *codec,
                                            const char *input, size_t size,
                                            regina_stream_t *output)
{
    aes_codec_t *aes_codec = TYPE_CAST(codec, aes_codec_t, base);
    return security_crypto_aes_codec_encdec(aes_codec->dec_ctx,
                                            aes_codec->dec_iv,
                                            aes_codec->undec_block,
                                            &aes_codec->undec_block_size,
                                            input, size, output,
                                            MBEDTLS_AES_DECRYPT);
}


static int security_crypto_aes_codec_flush_encoder(crypto_codec_t *codec,
                                                   regina_stream_t *output)
{
    int err;
    char *buf;
    aes_codec_t *aes_codec = TYPE_CAST(codec, aes_codec_t, base);

    err = regina_stream_reserve_next(output, &buf, 16);
    REGINA_RETURN_IF_ERR(err);


    regina_memset(aes_codec->unenc_block + aes_codec->unenc_block_size,
           16 - aes_codec->unenc_block_size,
           16 - aes_codec->unenc_block_size);

    mbedtls_aes_crypt_cbc(aes_codec->enc_ctx, MBEDTLS_AES_ENCRYPT, 16,
                          aes_codec->enc_iv, aes_codec->unenc_block,
                          (unsigned char*)buf);

    if (aes_codec->unenc_block_size >= 16) {
        /* assert (16 == aes_codec->unenc_block_size); */

        err = regina_stream_reserve_next(output, &buf, 16);
        REGINA_RETURN_IF_ERR(err);

        regina_memset(aes_codec->unenc_block, 16 , 16);
        mbedtls_aes_crypt_cbc(aes_codec->enc_ctx, MBEDTLS_AES_ENCRYPT, 16,
                              aes_codec->enc_iv, aes_codec->unenc_block,
                              (unsigned char*)buf);
    }

    regina_memset(aes_codec->enc_iv, 0, 16);
    aes_codec->unenc_block_size = 0;

    mbedtls_aes_setkey_enc(aes_codec->enc_ctx, aes_codec->key, aes_codec->keybits);
    return REGINA_ERR_NONE;
}

static int security_crypto_aes_codec_flush_decoder(crypto_codec_t *codec,
                                                   regina_stream_t *output)
{
    char *buf;
    unsigned char size;
    int err;
    aes_codec_t *aes_codec = TYPE_CAST(codec, aes_codec_t, base);

    if (16 != aes_codec->undec_block_size)
        return REGINA_ERR_DATA_CORRUPTION;


    err = regina_stream_reserve_next(output, &buf, 16);
    REGINA_RETURN_IF_ERR(err);

    mbedtls_aes_crypt_cbc(aes_codec->dec_ctx, MBEDTLS_AES_DECRYPT, 16,
                          aes_codec->dec_iv, aes_codec->undec_block,
                          (unsigned char*)buf);

    size = ((unsigned char*)buf)[15];
    if (size > 16)
        return REGINA_ERR_DATA_CORRUPTION;

    for (size = buf[15]; size > 1; --size) {
        if (buf[16 - size] != buf[15])
            return REGINA_ERR_DATA_CORRUPTION;
    }

    regina_stream_revert(output, ((unsigned char*)buf)[15]);

    regina_memset(aes_codec->dec_iv, 0, 16);
    aes_codec->undec_block_size = 0;

    mbedtls_aes_setkey_dec(aes_codec->dec_ctx, aes_codec->key, aes_codec->keybits);
    return REGINA_ERR_NONE;
}

static size_t security_crypto_aes_codec_get_key(crypto_codec_t *codec, char *key)
{
    aes_codec_t *aes_codec = TYPE_CAST(codec, aes_codec_t, base);
    size_t key_bytes = aes_codec->keybits / 8;

    if (key)
        regina_memcpy(key, aes_codec->key, key_bytes);

    return key_bytes;
}

/*-------------------------------- RSA CODEC ------------------------------------*/

/* PKCS#1 V1.5 padding */
typedef struct rsa_codec_t {
    crypto_codec_t base;

    mbedtls_pk_context enc_ctx[1];
    mbedtls_pk_context dec_ctx[1];

    unsigned char *enc_block;
    unsigned char *dec_block;

    size_t enc_block_size;
    size_t dec_block_size;

    mbedtls_ctr_drbg_context ctr_drbg[1];
} rsa_codec_t;


static int security_crypto_rsa_codec_init(rsa_codec_t *codec,
                                          const char *pub_key, size_t pbk_len,
                                          const char *priv_key, size_t pvk_len)
{
    int err;
    mbedtls_entropy_context entropy[1];

    mbedtls_pk_init(codec->enc_ctx);
    mbedtls_pk_init(codec->dec_ctx);

    mbedtls_entropy_init(entropy);
#if defined(MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES)
    mbedtls_entropy_add_source(entropy, security_crypto_entropy_source,
                               NULL, 32, MBEDTLS_ENTROPY_SOURCE_STRONG);
#endif /* MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES */

    mbedtls_ctr_drbg_init(codec->ctr_drbg);
    err = mbedtls_ctr_drbg_seed(codec->ctr_drbg, mbedtls_entropy_func, entropy, NULL, 0);
    mbedtls_entropy_free(entropy);

    if (0 != err)
        return REGINA_ERR_INTERNAL;

    if (pub_key && pbk_len) {
        int ret;
        unsigned char *key = (unsigned char*)regina_malloc(pbk_len + 1);
        size_t key_len;

        REGINA_RETURN_IF_NIL(key, REGINA_ERR_NOMEM);

        regina_memcpy(key, pub_key, pbk_len);
        key[pbk_len] = '\0';

        ret = mbedtls_pk_parse_public_key(codec->enc_ctx, key, pbk_len + 1);
        regina_free(key);

        if (0 != ret)
            return REGINA_ERR_BADPARAM;

        key_len = mbedtls_pk_get_len(codec->enc_ctx);
        codec->enc_block = (unsigned char*)regina_malloc(key_len);
        REGINA_RETURN_IF_NIL(codec->enc_block, REGINA_ERR_NOMEM);
    }

    if (priv_key && pvk_len) {
        int ret;
        size_t key_len;
        unsigned char *key = (unsigned char*)regina_malloc(pvk_len + 1);
        REGINA_RETURN_IF_NIL(key, REGINA_ERR_NOMEM);

        regina_memcpy(key, priv_key, pvk_len);
        key[pvk_len] = '\0';

        ret = mbedtls_pk_parse_key(codec->enc_ctx, key, pbk_len + 1, NULL, 0);
        regina_free(key);

        if (0 != ret)
            return REGINA_ERR_BADPARAM;

        key_len = mbedtls_pk_get_len(codec->dec_ctx);
        codec->dec_block = (unsigned char*)regina_malloc(key_len);
        REGINA_RETURN_IF_NIL(codec->dec_block, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

int security_crypto_rsa_codec_create(crypto_codec_t **codec,
                                     const char *pub_key, size_t pbk_len,
                                     const char *priv_key, size_t pvk_len)
{
    rsa_codec_t *rsa_codec;
    int err;

    rsa_codec = (rsa_codec_t*)regina_malloc(sizeof(rsa_codec_t));
    REGINA_RETURN_IF_NIL(rsa_codec, REGINA_ERR_NOMEM);
    regina_memset(rsa_codec, 0, sizeof(rsa_codec_t));

    rsa_codec->base.type = CRYPTO_CODEC_RSA;
    *codec = &rsa_codec->base;

    err = security_crypto_rsa_codec_init(rsa_codec, pub_key, pbk_len,
                                         priv_key, pvk_len);

    if (REGINA_ERR_NONE != err) {
        security_crypto_codec_destroy(*codec);
        return err;
    }

    return REGINA_ERR_NONE;
}

static void security_crypto_rsa_codec_destroy(crypto_codec_t *codec)
{
    rsa_codec_t *rsa_codec = TYPE_CAST(codec, rsa_codec_t, base);

    mbedtls_pk_free(rsa_codec->enc_ctx);
    mbedtls_pk_free(rsa_codec->dec_ctx);
    mbedtls_ctr_drbg_free(rsa_codec->ctr_drbg);

    regina_free(rsa_codec->enc_block);
    regina_free(rsa_codec->dec_block);

    regina_free(rsa_codec);
}


static int security_crypto_rsa_codec_encode(crypto_codec_t *codec, const char *input,
                                            size_t size, regina_stream_t *output)
{
    size_t key_len;
    rsa_codec_t *rsa_codec = TYPE_CAST(codec, rsa_codec_t, base);

    key_len = mbedtls_pk_get_len(rsa_codec->enc_ctx);

    /* PKCS1 V1.5 padding size is 11 bytes. */
    if (key_len - 11 < rsa_codec->enc_block_size + size)
        return REGINA_ERR_BADPARAM;

    regina_memcpy(rsa_codec->enc_block + rsa_codec->enc_block_size, input, size);
    rsa_codec->enc_block_size += size;

    return REGINA_ERR_NONE;
}

static int security_crypto_rsa_codec_flush_encoder(crypto_codec_t *codec, regina_stream_t *output)
{
    size_t key_len, enc_len;
    char *buf;
    int err;
    rsa_codec_t *rsa_codec = TYPE_CAST(codec, rsa_codec_t, base);

    key_len = mbedtls_pk_get_len(rsa_codec->enc_ctx);
    /* assert(key_len - 11 >= rsa_codec->enc_block_size); */

    err = regina_stream_reserve_next(output, &buf, key_len);
    REGINA_RETURN_IF_ERR(err);

    enc_len = 0;
    err = mbedtls_pk_encrypt(rsa_codec->enc_ctx, rsa_codec->enc_block,
                             rsa_codec->enc_block_size,
                             (unsigned char*)buf, &enc_len, key_len,
                             mbedtls_ctr_drbg_random, rsa_codec->ctr_drbg);
    if (0 != err)
        return REGINA_ERR_INTERNAL;

    return REGINA_ERR_NONE;
}

static int security_crypto_rsa_codec_decode(crypto_codec_t *codec, const char *input,
                                            size_t size, regina_stream_t *output)
{
    (void)codec;
    (void)input;
    (void)size;
    (void)output;

    /* not implement. */
    /* assert(false); */

    return 0;
}

static int security_crypto_rsa_codec_flush_decoder(crypto_codec_t *codec, regina_stream_t *input)
{
    (void)codec;
    (void)input;

    /* not implement. */
    /* assert(false); */

    return 0;
}


static size_t security_crypto_rsa_codec_get_key(crypto_codec_t *codec, char *key)
{
    (void)codec;
    (void)key;

    /* not implement. */
    /* assert(false); */

    return 0;
}

/*---------------------------- COMMON CODEC INTERFACE ---------------------------*/

typedef struct crypto_codec_methods_t {
    void (*destroy)(crypto_codec_t *codec);
    int (*encode)(crypto_codec_t *codec, const char *input, size_t size, regina_stream_t *output);
    int (*decode)(crypto_codec_t *codec, const char *input, size_t size, regina_stream_t *output);
    int (*flush_encoder)(crypto_codec_t *codec, regina_stream_t *output);
    int (*flush_decoder)(crypto_codec_t *codec, regina_stream_t *output);
    size_t (*get_key)(crypto_codec_t *codec, char *key);
} crypto_codec_methods_t;


static const crypto_codec_methods_t crypto_codec_methods[] = {
    { /* AES */
        security_crypto_aes_codec_destroy,
        security_crypto_aes_codec_encode,
        security_crypto_aes_codec_decode,
        security_crypto_aes_codec_flush_encoder,
        security_crypto_aes_codec_flush_decoder,
        security_crypto_aes_codec_get_key
    },

    { /* RSA */
        security_crypto_rsa_codec_destroy,
        security_crypto_rsa_codec_encode,
        security_crypto_rsa_codec_decode,
        security_crypto_rsa_codec_flush_encoder,
        security_crypto_rsa_codec_flush_decoder,
        security_crypto_rsa_codec_get_key
    }
};

int security_crypto_init()
{
    int err;
    err = mbedtls_platform_set_calloc_free(regina_calloc, regina_free);
    if (0 != err)
        return REGINA_ERR_INTERNAL;

    return REGINA_ERR_NONE;
}

void security_crypto_deinit()
{
}

void security_crypto_codec_destroy(crypto_codec_t *codec)
{
    if (codec)
        crypto_codec_methods[codec->type].destroy(codec);
}

int security_crypto_codec_encode(crypto_codec_t *codec, const char *input,
                                 size_t size, regina_stream_t *output)
{
    return crypto_codec_methods[codec->type].encode(codec, input, size, output);
}

int security_crypto_codec_decode(crypto_codec_t *codec, const char *input,
                                 size_t size, regina_stream_t *output)
{
    return crypto_codec_methods[codec->type].decode(codec, input, size, output);
}

int security_crypto_codec_flush_encoder(crypto_codec_t *codec,
                                        struct regina_stream_t *output)
{
    return crypto_codec_methods[codec->type].flush_encoder(codec, output);
}

int security_crypto_codec_flush_decoder(crypto_codec_t *codec,
                                        struct regina_stream_t *output)
{
    return crypto_codec_methods[codec->type].flush_decoder(codec, output);
}

size_t security_crypto_codec_get_key(crypto_codec_t *codec, char *key)
{
    return crypto_codec_methods[codec->type].get_key(codec, key);
}

void security_md5(const char *input, size_t size, char md[MD5_LENGTH])
{
    mbedtls_md5((const unsigned char*)input, size, (unsigned char*)md);
}

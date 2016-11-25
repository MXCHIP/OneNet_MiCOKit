#include "regina_config.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "security.h"
#include "stream.h"


typedef struct regina_confing_t {
    regina_sdk_config_t *conf;
    regina_list_node_t *next;
} regina_config_t;

static regina_config_t g_regina_conf;

/* the length of the key must be 16, 24 or 32 bytes. */
static const char key[] = {
    0x25, 0xcc, 0x96, 0x77,
    0x91, 0x5f, 0xda, 0xa8,
    0x92, 0x43, 0x61, 0x87,
    0x19, 0x86, 0x20, 0xec
};


static int regina_decrypt_conf(const char *conf, size_t size, regina_stream_t *stream)
{
    crypto_codec_t *codec;
    int err;

    err = security_crypto_aes_codec_create_ex(&codec, key, sizeof(key));
    REGINA_RETURN_IF_ERR(err);

    err = security_crypto_codec_decode(codec, conf, size, stream);
    if (REGINA_ERR_NONE == err)
        err = security_crypto_codec_flush_decoder(codec, stream);

    security_crypto_codec_destroy(codec);
    return err;
}


int regina_config_init(const char *conf, size_t size)
{
    regina_stream_t stream[1];
    int err;
    char *buf;

    REGINA_RETURN_IF_NIL2(conf, size, REGINA_ERR_BADPARAM);
    regina_memset(&g_regina_conf, 0, sizeof(g_regina_conf));

    regina_stream_init(stream);

    err = regina_decrypt_conf(conf, size, stream);
    if (REGINA_ERR_NONE != err) {
        regina_stream_destroy(stream);
        return err;
    }

    size = regina_stream_get_size(stream);
    buf = regina_malloc(size);
    if (buf)  {
        avro_reader_t reader;
        regina_stream_read(stream, buf, size);

        reader = avro_reader_memory(buf, size);
        if (reader) {
            g_regina_conf.conf = regina_sdk_config_deserialize(reader);
            avro_reader_free(reader);
        }

        regina_free(buf);
    }

    regina_stream_destroy(stream);

    if (NULL == g_regina_conf.conf)
        return REGINA_ERR_BADPARAM;

    regina_list_shuffle(g_regina_conf.conf->bootstrap_access_points);

    regina_config_goto_first_bootstrap();
    if (NULL == g_regina_conf.next) {
        REGINA_LOG0(FATAL, "Bug: no server in the SDK configuration.");
        return REGINA_ERR_INTERNAL;
    }
    return REGINA_ERR_NONE;
}

void regina_config_destroy()
{
    if (g_regina_conf.conf)
        g_regina_conf.conf->destroy(g_regina_conf.conf);
    regina_memset(&g_regina_conf, 0, sizeof(g_regina_conf));
}

const char *regina_config_get_sdk_uuid()
{
    return g_regina_conf.conf->sdk_uuid->data;
}

int32_t regina_config_get_sensor_data_max_size()
{
    return g_regina_conf.conf->sensor_data_limit->max_size;
}

int32_t regina_config_get_sensor_data_min_interval()
{
    return g_regina_conf.conf->sensor_data_limit->min_interval;
}

int32_t regina_config_get_event_max_count_per_day( void )
{
    return g_regina_conf.conf->event_limit->max_count_per_day;
}

int32_t regina_config_get_event_max_size()
{
    return g_regina_conf.conf->event_limit->max_size;
}

void regina_config_goto_first_bootstrap()
{
    g_regina_conf.next = regina_list_begin(
        g_regina_conf.conf->bootstrap_access_points);
}

const regina_bootstrap_access_point_t *
regina_config_get_next_bootstrap_server(int32_t protocol)
{
    regina_bootstrap_access_point_t *acc_point = NULL;

    while (g_regina_conf.next) {
        void *data = regina_list_get_data(g_regina_conf.next);
        g_regina_conf.next = regina_list_next(g_regina_conf.next);

        acc_point = (regina_bootstrap_access_point_t*)data;
        if (acc_point) {
            if (protocol == acc_point->protocol)
                return acc_point;
        }
    }

    return acc_point;
}

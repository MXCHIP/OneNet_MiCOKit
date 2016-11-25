#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>


#include <sys/time.h>
#include <unistd.h>


#include "regina/device.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "regina/error.h"
#include "conf.h"
#include "regina_profile.h"
#include "regina_sensor_data.h"
#include "regina_event.h"
#include "regina_notification.h"
#include "regina_configuration.h"

#define MAX_SENSOR_DATA_COUNT 6

typedef struct sample_context_t {
    regina_sensor_data_t sensor_data_buf[MAX_SENSOR_DATA_COUNT];
    regina_enum_ack_t sensor_data_ack_buf[MAX_SENSOR_DATA_COUNT];

    regina_uplink_data_t data;
    regina_uplink_data_ack_t ack;

    int sample_interval;
    int sample_expired_time;
} sample_context_t;

static void sample_log(void *ctx, regina_log_level_t ll, const char *format, ...)
{
    static const char *level[] = {
        "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
    };
    va_list ap;
    struct timeval tv;
    time_t t;
    struct tm *st;

    gettimeofday(&tv, NULL);

    t = tv.tv_sec;
    st = localtime(&t);

    printf("[%02d:%02d:%02d.%03d] [%s] ", st->tm_hour, st->tm_min,
           st->tm_sec, (int)tv.tv_usec/1000, level[ll]);

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    printf("\n");
}

static void sample_context_init(sample_context_t *ctx)
{
    const int default_sample_interval = 15;
    memset(ctx, 0, sizeof(sample_context_t));
    ctx->sample_interval = default_sample_interval;
    ctx->ack.sensor_data_ack = ctx->sensor_data_ack_buf;
    ctx->sample_expired_time = ctx->sample_interval;

    REGINA_LOG1(INFO, "Current sample interval is %d", default_sample_interval);
}

static void sample_retreive_profile(sample_context_t *ctx, const char *ieme,
                                    char **profile, size_t *size)
{
    p12_t0_profile_t *prof = p12_t0_profile_create();
    avro_writer_t writer;

    prof->ieme = regina_string_move_create(ieme, NULL);

    *size = prof->get_size(prof);
    *profile = (char*)malloc(*size);

    writer = avro_writer_memory(*profile, *size);
    prof->serialize(writer, prof);

    prof->destroy(prof);
    avro_writer_free(writer);
}

static void sample_add_sensor_data(sample_context_t *ctx)
{
    static float speed = 10.0f;
    static float temperature = 18.0f;

    p12_t4_sensor_data_t *sd;
    avro_writer_t writer;
    int idx;

    if (ctx->data.sd_count >= MAX_SENSOR_DATA_COUNT) {
        REGINA_LOG0(ERROR, "failed to add the sensor data for buffer is full.");
        return;
    }

    sd = p12_t4_sensor_data_create();

    sd->speed = speed;
    speed += 0.1f;

    sd->temperature = temperature;
    temperature += 0.05f;

    REGINA_LOG2(INFO, "add sensor data: speed=%f, temperature=%f.",
                speed, temperature);

    idx = ctx->data.sd_count;
    ctx->sensor_data_buf[idx].len = sd->get_size(sd);
    ctx->sensor_data_buf[idx].data = (char*)malloc(ctx->sensor_data_buf[idx].len);
    writer = avro_writer_memory(ctx->sensor_data_buf[idx].data,
                                ctx->sensor_data_buf[idx].len);
    sd->serialize(writer, sd);

    sd->destroy(sd);
    avro_writer_free(writer);

    ctx->data.sensor_data = ctx->sensor_data_buf;
    ++ctx->data.sd_count;
}

static void sample_add_event(sample_context_t *ctx)
{
    static float pression = 2.3f;
    p12_t2_event_t *evt = p12_t2_event_create();
    avro_writer_t writer;

    REGINA_LOG1(INFO, "Send event: pression=%f.", pression);
    evt->pression = pression;
    pression += 0.025f;

    ctx->data.event.len = evt->get_size(evt);
    ctx->data.event.event = (char*)malloc(ctx->data.event.len);
    ctx->data.event.level = REGINA_EVENT_LEVEL_WARNING;

    writer = avro_writer_memory(ctx->data.event.event, ctx->data.event.len);
    evt->serialize(writer, evt);

    evt->destroy(evt);
    avro_writer_free(writer);
}

static void sample_handle_notification(void *ctx, const regina_notification_id_t *id,
                                       const char *notification, size_t size)
{
    sample_context_t *context = (sample_context_t*)ctx;
    avro_reader_t reader = avro_reader_memory(notification, size);

    p12_t3_notification_t *noti = p12_t3_notification_deserialize(reader);
    p12_t3_notification_t *noti_conf;
    char confirmation[1000];
    regina_string_t *conf;
    avro_writer_t writer;

    REGINA_LOG2(INFO, "The content of the notification(%s) is: '%s'.",
                id->uuid,  noti->news->data);

    noti_conf = p12_t3_notification_create();


    snprintf(confirmation, sizeof(confirmation), "[CONFIRMATION: %s]", id->uuid);
    noti_conf->confirmation = p12_t3_union_null_or_string_branch_1_create();

    conf = regina_string_move_create(confirmation, NULL);
    noti_conf->confirmation->data = conf;
    noti_conf->news = regina_string_move_create("", NULL);

    size = noti_conf->get_size(noti_conf);

    free(context->data.ntf_id.uuid);
    context->data.ntf_id.uuid = strdup(id->uuid);
    context->data.ntf_id.internal = id->internal;
    context->data.cf_len = noti_conf->get_size(noti_conf);

    free(context->data.ntf_confirmation);
    context->data.ntf_confirmation = (char*)malloc(context->data.cf_len);

    writer = avro_writer_memory(context->data.ntf_confirmation, context->data.cf_len);

    noti_conf->serialize(writer, noti_conf);

    avro_writer_free(writer);
    noti_conf->destroy(noti_conf);

    noti->destroy(noti);
    avro_reader_free(reader);
}

static int sample_handle_configuration(void *ctx, const char *configuration, size_t size)
{
    sample_context_t *sample_ctx = (sample_context_t*)ctx;
    avro_reader_t reader;
    p12_t1_configuration_t *conf;

    if (!configuration || !size)
        return 0;

    reader = avro_reader_memory(configuration, size);
    conf = p12_t1_configuration_deserialize(reader);

    sample_ctx->sample_expired_time -= sample_ctx->sample_interval;
    sample_ctx->sample_expired_time += conf->sample_interval;
    sample_ctx->sample_interval = conf->sample_interval;

    REGINA_LOG1(INFO, "Receive configuration, current sample interval is %d seconds.",
                sample_ctx->sample_interval);

    conf->destroy(conf);
    avro_reader_free(reader);

    return 0;
}

static void sample_handle_software_update(void *ctx, const regina_module_update_info_t *modules,
                                          int count)
{
    int i;
    (void)ctx;

    REGINA_LOG0(INFO, "+++++++++++++++++++++++++++++++++++++++++++++++");
    REGINA_LOG1(INFO, "recv software update, %d modules:", count);

    for (i = 0; i < count; ++i) {
        int j;
        char md5[100];
        char *cursor = md5;

        REGINA_LOG1(INFO, "MODULE [%s]:", modules[i].name);
        REGINA_LOG1(INFO, "    URL: %s", modules[i].url);
        REGINA_LOG1(INFO, "    VERSION: %s", modules[i].version);


        for (j = 0; j < 16; ++j) {
            unsigned int c = ((unsigned char*)modules[i].md5)[j];

            sprintf(cursor, "%02X", c);
            cursor += 2;
        }
        REGINA_LOG1(INFO, "    MD5: %s", md5);
    }
    REGINA_LOG0(INFO, "+++++++++++++++++++++++++++++++++++++++++++++++");
}

static void sample_clear_uplink_data(regina_uplink_data_t *data,
                                     const regina_uplink_data_ack_t *ack)
{
    size_t i;

    for (i = 0; i < data->sd_count; ++i) {
        if (REGINA_ACK_SUCCESS != ack->sensor_data_ack[i]) {
            REGINA_LOG1(ERROR, "failed to add sensor data, ack=%d",
                        ack->sensor_data_ack[i]);
        }

        free(data->sensor_data[i].data);
        data->sensor_data[i].data = NULL;
        data->sensor_data[i].len = 0;
    }
    data->sd_count = 0;

    if (REGINA_ACK_SUCCESS != ack->event_ack) {
        REGINA_LOG1(ERROR, "failed to send event, ack=%d", ack->event_ack);
    }
    free(data->event.event);
    data->event.event = NULL;

    if (REGINA_ACK_SUCCESS != ack->nt_confirmation_ack) {
        REGINA_LOG2(ERROR, "failed to send the confirmation of the notification(%s), ack=%d",
                    data->ntf_id.uuid, ack->nt_confirmation_ack);
    }
    free(data->ntf_confirmation);
    free(data->ntf_id.uuid);
    data->ntf_confirmation = NULL;
    data->ntf_id.uuid = NULL;
}




int main(int argc, char **argv)
{
    int err;
    sample_context_t ctx;
    int milliseconds = 0;
    regina_uplink_data_ack_t *ack;
    char *profile = NULL;
    size_t prof_len = 0;
    char devid[21];
    char acc_token[9];
    const regina_module_info_t modules[] = {
        {
            "sample",
            "1.0.0"
        }
    };

    if (argc != 2) {
        printf("USAGE: %s <IEME> \n", argv[0]);
        return -1;
    }

    regina_log_set_logger(NULL, sample_log);

    sample_context_init(&ctx);

    ack = &ctx.ack;

    sample_retreive_profile(&ctx, argv[1], &profile, &prof_len);

    err = regina_device_open(regina_conf_get(), REGINA_CONF_SIZE,
                             "NEpnYIVcRI", profile, prof_len,
                             modules, sizeof(modules)/sizeof(modules[0]),
                             &ctx.data, ack);
    free(profile);
    profile = NULL;
    sample_clear_uplink_data(&ctx.data, ack);
    if (REGINA_ERR_NONE != err) {
        regina_device_close();
        REGINA_LOG1(ERROR, "Failed to connect to the server, error= %d.", err);
        return -1;
    }

    regina_device_get_device_id(devid, acc_token);
    printf("Device id is [%s]\n", devid);
    printf("Access token is [%s]\n", acc_token);

    while (1)  {

        err = regina_device_check_and_keepalive();
        if (REGINA_ERR_NONE != err) {
            REGINA_LOG1(ERROR, "Connection is closed, error=%d.", err);
            break;
        }

        err = regina_device_handle_downlink_data(&ctx,
                                                 sample_handle_notification,
                                                 sample_handle_configuration,
                                                 sample_handle_software_update);
        if (REGINA_ERR_NONE != err) {
            REGINA_LOG1(ERROR, "Failed to handle downlink data, error=%d.", err);
            break;
        }

        if (milliseconds >= ctx.sample_expired_time * 1000) {
            sample_add_sensor_data(&ctx);
            ctx.sample_expired_time += ctx.sample_interval;
        }

        /* simulate an event per 900 seconds. */
        if (0 == (milliseconds % 900000))
            sample_add_event(&ctx);

        err = regina_device_send(&ctx.data, ack);
        if (REGINA_ERR_NONE != err) {
            REGINA_LOG1(ERROR, "Failed to send the data, error=%d.", err);
            break;
        }

        sample_clear_uplink_data(&ctx.data, ack);

        /* request the thirdparty service per 500 seconds. */
        if (0 == (milliseconds % 500000)) {
            char *resp;
            size_t size;
            regina_enum_ack_t ack;

            err = regina_device_request_thirdparty_service(REGINA_THIRDPARTY_SERVICE_QOS1,
                                                           "req", 3, &ack, &resp, &size);

            if (REGINA_ERR_NONE != err) {
                REGINA_LOG1(ERROR, "Failed to request thirdparty service, error=%d.", err);
                break;
            }

            REGINA_LOG2(INFO, "Received the response(%lu bytes) from the "
                        "thirdparty service, ack=%d.", size, ack);
            regina_free(resp);
        }

        usleep(100 * 1000);
        milliseconds += 100;
    }

    regina_device_close();
    REGINA_LOG0(INFO, "Device stopped.");

    return 0;
}

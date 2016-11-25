#include "acceptor.h"
#include "regina/allocator.h"
#include "regina/device.h"
#include "std_wrapper.h"
#include "common.h"
#include "util.h"
#include "device_status.h"
#include "stream.h"
#include "auth_service.h"
#include "profile_service.h"
#include "sensor_data_service.h"
#include "event_service.h"
#include "configuration_service.h"
#include "notification_service.h"
#include "soft_service.h"
#include "thirdparty_service.h"
#include "thread.h"
#include "channel.h"

#define MAX_RESPONSE_INTERVAL_SECONDS 60

struct acceptor_t {
    int authenticated;
    int waiting_ack;

    channel_hook_t hook;
    channel_t *channel;

    access_point_t *ap;

    auth_service_t *auth_svc;
    profile_service_t *prof_svc;
    sensor_data_service_t *sd_svc;
    event_service_t *evnt_svc;
    configuration_service_t *conf_svc;
    notification_service_t *noti_svc;
    soft_service_t *soft_svc;
    thirdparty_service_t *thrd_svc;
};


static int acceptor_is_uplink_data_empty(const regina_uplink_data_t *data)
{
    int empty = ((!data->event.event || !data->event.len) &&
                 (!data->ntf_confirmation || !data->cf_len) &&
                 !data->check_software_update);

    if (!empty)
        return 0;

    if (data->sensor_data && data->sd_count) {
        size_t i;
        for (i = 0; i < data->sd_count; ++i) {
            if (data->sensor_data[i].data && data->sensor_data[i].len)
                return 0;
        }
    }

    return 1;
}

static int acceptor_do_handle_sync_resp(acceptor_t *acc,
                                        regina_sync_response_t *resp)
{
    int err;

    err = auth_service_handle_reg_resp(
        acc->auth_svc, resp->register_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = auth_service_handle_auth_resp(
        acc->auth_svc, resp->authen_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = profile_service_handle_sync_resp(
        acc->prof_svc, resp->profile_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = sensor_data_service_handle_sync_resp(
        acc->sd_svc, resp->sensor_data_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = event_service_handle_sync_resp(
        acc->evnt_svc, resp->event_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_handle_sync_resp(
        acc->noti_svc, resp->notify_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_handle_cmd_resp(
        acc->noti_svc, resp->cmd_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_handle_confirmation_ack(
        acc->noti_svc, resp->notify_confirm_ack);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_handle_cmdconfirm_ack(
        acc->noti_svc, resp->cmd_confirm_ack);
    REGINA_RETURN_IF_ERR(err);

    err = configuration_service_handle_sync_resp(
        acc->conf_svc, resp->config_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = soft_service_handle_sync_resp(
        acc->soft_svc, resp->soft_sync_response);
    REGINA_RETURN_IF_ERR(err);

    err = thirdparty_service_handle_resp(
        acc->thrd_svc, resp->thirdparty_service_response);
    REGINA_RETURN_IF_ERR(err);

    err = device_status_save();

    acc->waiting_ack = (sensor_data_service_is_waiting_ack(acc->sd_svc) ||
                        event_service_is_waiting_ack(acc->evnt_svc) ||
                        notification_service_is_waiting_confirmation_ack(acc->noti_svc) ||
                        profile_service_is_waiting_ack(acc->prof_svc) ||
                        thirdparty_service_is_waiting_ack(acc->thrd_svc));
    return err;
}

static void acceptor_handle_close(void *ctx, int err)
{
    acceptor_t *acc = (acceptor_t*)ctx;
    acc->authenticated = 0;

    auth_service_handle_close(acc->auth_svc);
    profile_service_handle_close(acc->prof_svc);
    sensor_data_service_handle_close(acc->sd_svc);
    event_service_handle_close(acc->evnt_svc);
    configuration_service_handle_close(acc->conf_svc);
    notification_service_handle_close(acc->noti_svc);
    soft_service_handle_close(acc->soft_svc);
}

static int acceptor_handle_sync(void *ctx, const char *payload, size_t size)
{
    int err = REGINA_ERR_NONE;
    acceptor_t *acc = (acceptor_t*)ctx;
    avro_reader_t reader;

    if (!payload || !size) {
        REGINA_LOG0(ERROR, "Bug: receive SyncResponse from acceptor successfully"
                    " but no payload.");
        return REGINA_ERR_PROTOCOL_ERROR;
    }

    reader = avro_reader_memory(payload, size);
    if (reader) {
        regina_sync_response_t *resp;

        resp = regina_sync_response_deserialize(reader);
        avro_reader_free(reader);

        if (resp) {
            err = acceptor_do_handle_sync_resp(acc, resp);
            resp->destroy(resp);
        }
    }

    return err;
}


static int acceptor_handle_conn_ack(void *ctx, const char *payload, size_t size)
{
    int err;
    err = acceptor_handle_sync(ctx, payload, size);
    if (REGINA_ERR_NONE == err) {
        acceptor_t *acc = (acceptor_t*)ctx;
        acc->authenticated = !auth_service_is_waiting_auth(acc->auth_svc);
    }
    return err;
}

static int acceptor_init(acceptor_t *acc, channel_t *channel)
{
    int err;

    acc->hook.context = acc;
    acc->hook.handle_close = acceptor_handle_close;
    acc->hook.handle_conn_ack = acceptor_handle_conn_ack;
    acc->hook.handle_sync = acceptor_handle_sync;

    err = auth_service_create(&acc->auth_svc);
    REGINA_RETURN_IF_ERR(err);

    err = profile_service_create(&acc->prof_svc);
    REGINA_RETURN_IF_ERR(err);

    err = sensor_data_service_create(&acc->sd_svc);
    REGINA_RETURN_IF_ERR(err);

    err = event_service_create(&acc->evnt_svc);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_create(&acc->noti_svc);
    REGINA_RETURN_IF_ERR(err);

    err = configuration_service_create(&acc->conf_svc);
    REGINA_RETURN_IF_ERR(err);

    err = soft_service_create(&acc->soft_svc);
    REGINA_RETURN_IF_ERR(err);

    err = thirdparty_service_create(&acc->thrd_svc);
    REGINA_RETURN_IF_ERR(err);

    acc->channel = channel;
    return REGINA_ERR_NONE;
}

static int acceptor_do_extract_sync_ack(acceptor_t *acc, regina_sync_response_ack_t **ack)
{
    int err;

    *ack = regina_sync_response_ack_create();
    REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);

    err = soft_service_extract_ack(acc->soft_svc, &(*ack)->software_ack);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_extract_syncack(acc->noti_svc, &(*ack)->notify_ack);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_extract_cmdack(acc->noti_svc, &(*ack)->cmd_ack);
    REGINA_RETURN_IF_ERR(err);

    err = configuration_service_extract_ack(acc->conf_svc, &(*ack)->config_ack);
    REGINA_RETURN_IF_ERR(err);

    if ((REGINA_UNION_LONG_OR_NULL_BRANCH_0 == (*ack)->software_ack->type) ||
        (REGINA_UNION_NULL_OR_STRING_BRANCH_1 == (*ack)->notify_ack->type) ||
        (REGINA_UNION_NULL_OR_STRING_BRANCH_1 == (*ack)->cmd_ack->type) ||
        (REGINA_UNION_LONG_OR_NULL_BRANCH_0 == (*ack)->config_ack->type)) {
        return REGINA_ERR_NONE;
    }

    (*ack)->destroy(*ack);
    *ack = NULL;
    return REGINA_ERR_NONE;
}

static int acceptor_extract_sync_ack(acceptor_t *acc, regina_union_t **ack)
{
    regina_sync_response_ack_t *sync_ack = NULL;
    int err;

    err = acceptor_do_extract_sync_ack(acc, &sync_ack);
    if (REGINA_ERR_NONE != err) {
        if (sync_ack)
            sync_ack->destroy(sync_ack);
        return err;
    }

    if (sync_ack) {
        *ack = regina_union_sync_response_ack_or_null_branch_0_create();
        if (NULL == *ack) {
            sync_ack->destroy(sync_ack);
            return REGINA_ERR_NOMEM;
        }
        (*ack)->data = sync_ack;
    }
    else {
        *ack = regina_union_sync_response_ack_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

static int acceptor_extract_sync_request(acceptor_t *acc,
                                         const char *profile,
                                         size_t prof_len,
                                         const regina_module_info_t *modules,
                                         size_t module_count,
                                         const regina_uplink_data_t *data,
                                         thirdparty_service_req_t *thrd_req,
                                         regina_sync_request_t *req)
{
    int err;
    regina_uplink_data_t guard;

    if (!data) {
        regina_memset(&guard, 0, sizeof(guard));
        data = &guard;
    }

    err = auth_service_extract_reg_req(acc->auth_svc, modules, module_count,
                                       &req->register_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = auth_service_extract_auth_req(acc->auth_svc, modules, module_count,
                                        &req->authen_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = profile_service_extract_req(
        acc->prof_svc, profile, prof_len, &req->profile_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = sensor_data_service_extract_req(
        acc->sd_svc, data->sensor_data, data->sd_count, &req->sensor_data_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = event_service_extract_req(acc->evnt_svc, &data->event, &req->event_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_extract_confirmation(acc->noti_svc,
                                                    &data->ntf_id,
                                                    data->ntf_confirmation,
                                                    data->cf_len,
                                                    req);
    REGINA_RETURN_IF_ERR(err);


    err = soft_service_extract_req(acc->soft_svc, data->check_software_update,
                                   &req->soft_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = notification_service_extract_req(acc->noti_svc, &req->notify_sync_request);
    REGINA_RETURN_IF_ERR(err);


    err = configuration_service_extract_req(acc->conf_svc, &req->config_sync_request);
    REGINA_RETURN_IF_ERR(err);

    err = acceptor_extract_sync_ack(acc, &req->sync_response_ack);
    REGINA_RETURN_IF_ERR(err);

    err = thirdparty_service_extract_req(acc->thrd_svc, thrd_req,
                                         &req->thirdparty_service_request);
    REGINA_RETURN_IF_ERR(err);

    return REGINA_ERR_NONE;
}

static regina_enum_ack_t acceptor_response_result_to_ack(regina_response_result_t result)
{
    switch(result) {
    case ENUM_RESPONSE_RESULT_SUCCESS:
        return REGINA_ACK_SUCCESS;

    case ENUM_RESPONSE_RESULT_INVALID_DATA:
        return REGINA_ACK_INVALID_DATA;

    case ENUM_RESPONSE_RESULT_FREQ_LIMIT:
        return REGINA_ACK_FREQ_LIMIT;

    case ENUM_RESPONSE_RESULT_COUNT_LIMIT:
        return REGINA_ACK_COUNT_LIMIT;

    case ENUM_RESPONSE_RESULT_DATA_SIZE_LIMIT:
        return REGINA_ACK_DATA_SIZE_LIMIT;

    case ENUM_RESPONSE_RESULT_UNSUPPORT:
        return REGINA_ACK_UNSUPPORTED;

    case ENUM_RESPONSE_RESULT_INTERNAL_ERROR:
    default:
        break;
    }

    return REGINA_ACK_INTERNAL_ERROR;
}

static int acceptor_serialize_sync_req(acceptor_t *acc,
                                       const char *profile,
                                       size_t prof_len,
                                       const regina_module_info_t *modules,
                                       size_t module_count,
                                       const regina_uplink_data_t *data,
                                       thirdparty_service_req_t *thrd_req,
                                       regina_stream_t *stream)
{
    int err;
    regina_sync_request_t *sync_req;
    char *buf;
    size_t size;

    sync_req = regina_sync_request_create();
    REGINA_RETURN_IF_NIL(sync_req, REGINA_ERR_NOMEM);

    err = acceptor_extract_sync_request(acc, profile, prof_len,
                                        modules, module_count,
                                        data, thrd_req, sync_req);
    if (REGINA_ERR_NONE != err) {
        sync_req->destroy(sync_req);
        return err;
    }

    size = sync_req->get_size(sync_req);
    err = regina_stream_reserve_next(stream, &buf, size);

    if (REGINA_ERR_NONE == err && buf) {
        avro_writer_t writer = avro_writer_memory(buf, size);
        if (writer) {
            sync_req->serialize(writer, sync_req);
            avro_writer_free(writer);
            err = REGINA_ERR_NONE;
        }
    }
    sync_req->destroy(sync_req);

    return err;
}

static int acceptor_wait_ack(acceptor_t *acc, struct regina_uplink_data_ack_t *ack)
{
    int err = REGINA_ERR_NONE;

    while ((REGINA_ERR_NONE == err)) {
        /* must read the data from the TCP/IP stack. */
        err = channel_poll(acc->channel);
        if (!acc->waiting_ack)
            break;
        regina_sleep(50);
    }

    if (REGINA_ERR_NONE == err && ack) {
        regina_response_result_t result;
        regina_response_result_t *results;
        size_t count, i;

        result = profile_service_get_and_reset_ack(acc->prof_svc);
        (void)result; /* ignore for handled by auth service. */

        result = notification_service_get_and_reset_confirmation_ack(acc->noti_svc);
        ack->nt_confirmation_ack = acceptor_response_result_to_ack(result);

        result = event_service_get_and_reset_ack(acc->evnt_svc);
        ack->event_ack = acceptor_response_result_to_ack(result);

        sensor_data_service_get_and_reset_ack(acc->sd_svc, &results, &count);
        for (i = 0; i < count; ++i) {
            ack->sensor_data_ack[i] = acceptor_response_result_to_ack(results[i]);
        }
        regina_free(results);
    }

    return err;
}

int acceptor_create(acceptor_t **acceptor, channel_t *channel)
{
    int err;
    acceptor_t *acc;

    acc = (acceptor_t*)regina_malloc(sizeof(acceptor_t));
    REGINA_RETURN_IF_NIL(acc, REGINA_ERR_NOMEM);

    regina_memset(acc, 0, sizeof(acceptor_t));

    err = acceptor_init(acc, channel);
    if (REGINA_ERR_NONE != err) {
        acceptor_destroy(acc);
        return err;
    }

    *acceptor = acc;
    return REGINA_ERR_NONE;
}

void acceptor_destroy(acceptor_t *acc)
{
    if (!acc)
        return;

    auth_service_destroy(acc->auth_svc);
    profile_service_destroy(acc->prof_svc);
    sensor_data_service_destroy(acc->sd_svc);
    event_service_destroy(acc->evnt_svc);
    notification_service_destroy(acc->noti_svc);
    configuration_service_destroy(acc->conf_svc);
    soft_service_destroy(acc->soft_svc);
    thirdparty_service_destroy(acc->thrd_svc);

    regina_free(acc);
}

int acceptor_check_and_keepalive(acceptor_t *acc)
{
    return channel_poll(acc->channel);
}

int acceptor_authenticate(acceptor_t *acc, access_point_t *ap,
                          const char *profile, size_t prof_len,
                          const regina_module_info_t *modules,
                          size_t module_count,
                          const struct regina_uplink_data_t *data,
                          struct regina_uplink_data_ack_t *ack)
{
    int err;
    regina_stream_t payload[1];

    err = channel_set_access_point(acc->channel, ap, &acc->hook);
    REGINA_RETURN_IF_ERR(err);

    REGINA_LOG0(INFO, "Start to connect to the ACCEPTOR...");
    err = channel_open(acc->channel);

    REGINA_RETURN_IF_ERR(err);

    regina_stream_init(payload);

    err = acceptor_serialize_sync_req(acc, profile, prof_len,
                                      modules, module_count,
                                      data, NULL, payload);
    if (REGINA_ERR_NONE == err) {
        acc->waiting_ack = 1;
        err = channel_acceptor_auth(acc->channel, payload);
    }
    regina_stream_destroy(payload);

    REGINA_RETURN_IF_ERR(err);

    channel_set_timeout(acc->channel, MAX_RESPONSE_INTERVAL_SECONDS);
    while(REGINA_ERR_NONE == err) {
        err = channel_poll(acc->channel);
        if (acc->authenticated)
            break;

        regina_sleep(50);
    }

    if (REGINA_ERR_NONE == err)
        err = acceptor_wait_ack(acc, ack);
    channel_set_timeout(acc->channel, 0);

    return err;
}

int acceptor_send(acceptor_t *acc, const regina_uplink_data_t *data,
                  regina_uplink_data_ack_t *ack)
{
    int err;
    regina_stream_t payload[1];

    REGINA_RETURN_IF_NIL(data, REGINA_ERR_BADPARAM);

    if (!acc->authenticated)
        return REGINA_ERR_BAD_STATE;

    if (acceptor_is_uplink_data_empty(data)) {
        ack->event_ack = REGINA_ACK_SUCCESS;
        ack->nt_confirmation_ack = REGINA_ACK_SUCCESS;
        return REGINA_ERR_NONE;
    }

    regina_stream_init(payload);
    err = acceptor_serialize_sync_req(acc, NULL, 0, NULL,
                                      0, data, NULL,  payload);
    if (REGINA_ERR_NONE == err) {
        acc->waiting_ack = 1;
        err = channel_acceptor_send(acc->channel, payload);
    }
    regina_stream_destroy(payload);

    if (REGINA_ERR_NONE == err) {
        channel_set_timeout(acc->channel, MAX_RESPONSE_INTERVAL_SECONDS);
        err = acceptor_wait_ack(acc, ack);
        channel_set_timeout(acc->channel, 0);
    }

    return err;
}

int acceptor_handle_downlink_data(acceptor_t *acc, void *ctx,
                                  notification_callback nt_callback,
                                  configuration_callback cf_callback,
                                  software_update_callback su_callback)
{
    int err;
    int has_ack = 0;

    if (nt_callback)
        has_ack |= notification_service_handle_notification(acc->noti_svc, ctx, nt_callback);

    if (cf_callback)
        has_ack |= configuration_service_handle_config(acc->conf_svc, ctx, cf_callback);

    if (su_callback)
        has_ack |= soft_service_handle_soft_update(acc->soft_svc, ctx, su_callback);

    err = device_status_save();
    REGINA_RETURN_IF_ERR(err);

    if (has_ack) {
        regina_stream_t payload[1];

        /* send the ack of the downlink data. */
        regina_stream_init(payload);
        REGINA_LOG_DEBUG0("START to pack downlink data ack.");
        err = acceptor_serialize_sync_req(acc, NULL, 0, NULL, 0,  NULL, NULL, payload);
        REGINA_LOG_DEBUG1("End to pack dowlink data ack, error=%d.", err);
        if (REGINA_ERR_NONE == err) {
            err = channel_acceptor_send(acc->channel, payload);
        }
        regina_stream_destroy(payload);
    }

    return err;
}

int acceptor_get_configuration(acceptor_t *acc, char **conf, size_t *size)
{
    return configuration_service_get_config(acc->conf_svc, conf, size);
}

int acceptor_request_thirdparty_service(acceptor_t *acc, regina_thirdparty_service_qos_t qos,
                                        const char *request, size_t req_size,
                                        regina_enum_ack_t *ack,
                                        char **response, size_t *resp_size)
{
    int err;
    regina_stream_t payload[1];
    thirdparty_service_req_t req[1];

    REGINA_RETURN_IF_NIL2(request, req_size, REGINA_ERR_BADPARAM);
    if (!acc->authenticated)
        return REGINA_ERR_BAD_STATE;

    regina_stream_init(payload);
    req->qos = qos;
    req->body = request;
    req->size = req_size;

    err = acceptor_serialize_sync_req(acc, NULL, 0, NULL, 0, NULL, req,  payload);
    if (REGINA_ERR_NONE == err) {
        err = channel_acceptor_send(acc->channel, payload);
    }
    regina_stream_destroy(payload);

    if (REGINA_ERR_NONE == err) {
        channel_set_timeout(acc->channel, MAX_RESPONSE_INTERVAL_SECONDS);

        while (REGINA_ERR_NONE == err) {
            err = channel_poll(acc->channel);
            if (REGINA_ERR_NONE != err)
                break;

            if (!thirdparty_service_is_waiting_ack(acc->thrd_svc)) {
                regina_response_result_t result;
                result = thirdparty_service_get_and_reset_ack(acc->thrd_svc,
                                                              response, resp_size);
                if (ack)
                    *ack = acceptor_response_result_to_ack(result);

                break;
            }

            regina_sleep(100);
        }

        channel_set_timeout(acc->channel, 0);
    }

    return err;
}

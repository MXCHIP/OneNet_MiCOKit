#include "sensor_data_service.h"
#include "regina/allocator.h"
#include "regina/device.h"
#include "device_status.h"
#include "common.h"
#include "util.h"
#include "regina_config.h"
#include "std_wrapper.h"

struct sensor_data_service_t {
    int64_t seq_num;
    int waiting_ack;
    regina_response_result_t *ack;
    size_t count;
};

int sensor_data_service_create(sensor_data_service_t **svc)
{
    *svc = (sensor_data_service_t*)regina_malloc(sizeof(sensor_data_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(sensor_data_service_t));
    return REGINA_ERR_NONE;
}

void sensor_data_service_destroy(sensor_data_service_t *svc)
{
    if (!svc)
        return;

    regina_free(svc->ack);
    regina_free(svc);
}

int sensor_data_service_extract_req(sensor_data_service_t *svc,
                                    const struct regina_sensor_data_t *data,
                                    size_t count,
                                    regina_union_t **req)
{
    if (!data || !count) {
        *req = regina_union_sensor_data_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_sensor_data_sync_request_t *sd_req;
        size_t i;

        REGINA_LOG_DEBUG0("[SDAT SVC] Extracting SYNC request.");

        *req = regina_union_sensor_data_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        sd_req = regina_sensor_data_sync_request_create();
        REGINA_RETURN_IF_NIL(sd_req, REGINA_ERR_NOMEM);

        (*req)->data = sd_req;
        sd_req->sensor_data_entries = regina_list_create();
        REGINA_RETURN_IF_NIL(sd_req->sensor_data_entries, REGINA_ERR_NOMEM);

        for (i = 0; i < count; ++i) {
            regina_sensor_data_entry_t *entry;

            entry = regina_sensor_data_entry_create();
            REGINA_RETURN_IF_NIL(entry, REGINA_ERR_NOMEM);

            if (NULL == regina_list_push_back(sd_req->sensor_data_entries, entry))
                return REGINA_ERR_NOMEM;

            entry->data = regina_bytes_copy_create(
                (const uint8_t*)data[i].data, data[i].len);
            REGINA_RETURN_IF_NIL(entry->data, REGINA_ERR_NOMEM);
            entry->at = data[i].timestamp;
        }

        sd_req->seq_num = ++svc->seq_num;
        svc->waiting_ack = 1;
        svc->count = count;

        REGINA_LOG_DEBUG0("[SDAT SVC] Complete to extract SYNC request.");
    }

    return REGINA_ERR_NONE;
}

int sensor_data_service_is_waiting_ack(sensor_data_service_t *svc)
{
    return svc->waiting_ack;
}

int sensor_data_service_handle_sync_resp(sensor_data_service_t *svc,
                                         regina_union_t *resp)
{
    if ((resp->type == REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_BRANCH_0) &&
        resp->data) {
        regina_sensor_data_sync_response_t *sd_resp;

        sd_resp = (regina_sensor_data_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG2("[SDAT SVC] Receive sensor data sync response, "
                          "local sn=%d, remote sn=%d.",
                          svc->seq_num, sd_resp->seq_num);

        if (svc->seq_num == sd_resp->seq_num) {
            size_t count, i;
            regina_list_node_t *cursor;

            count = regina_list_get_size(sd_resp->sensor_data_delivery_status);
            if (count != svc->count) {
                REGINA_LOG2(FATAL, "Bug: the count of sensor data ack is not the same, "
                            "expect %lu but %lu.", svc->count, count);
                return REGINA_ERR_PROTOCOL_ERROR;
            }

            svc->ack = (regina_response_result_t*)regina_malloc(
                sizeof(regina_response_result_t) * count);
            REGINA_RETURN_IF_NIL(svc->ack, REGINA_ERR_NOMEM);

            i = 0;
            for (cursor = regina_list_begin(sd_resp->sensor_data_delivery_status);
                 cursor; cursor = regina_list_next(cursor)) {
                regina_response_result_t *result;
                result = (regina_response_result_t*)regina_list_get_data(cursor);
                svc->ack[i] = *result;
                ++i;
            }

            svc->waiting_ack = 0;
        }
    }

    return REGINA_ERR_NONE;
}

void sensor_data_service_get_and_reset_ack(sensor_data_service_t *svc,
                                           regina_response_result_t **results,
                                           size_t *count)
{
    *results = svc->ack;
    *count = svc->count;

    svc->ack = NULL;
    svc->count = 0;
}

void sensor_data_service_handle_close(sensor_data_service_t *svc)
{
    REGINA_LOG_DEBUG0("[SDAT SVC] Handle close.");
}

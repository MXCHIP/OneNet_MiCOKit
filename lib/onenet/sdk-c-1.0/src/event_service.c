#include "event_service.h"
#include "regina/allocator.h"
#include "regina/device.h"
#include "common.h"
#include "util.h"
#include "device_status.h"
#include "regina_config.h"
#include "std_wrapper.h"



struct event_service_t {
    int64_t seq_num;
    int waiting_ack;
    regina_response_result_t ack;
};

int event_service_create(event_service_t **svc)
{
    *svc = (event_service_t*)regina_malloc(sizeof(event_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(event_service_t));
    (void)event_service_get_and_reset_ack(*svc);
    return REGINA_ERR_NONE;
}

void event_service_destroy(event_service_t *svc)
{
    if (!svc)
        return;

    regina_free(svc);
}

int event_service_extract_req(event_service_t *svc,
                              const regina_event_t *event,
                              regina_union_t **req)

{
    if (!event->event || !event->len) {
        *req = regina_union_event_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_event_sync_request_t *ev_req;
        regina_event_level_t level;

        switch(event->level) {
        case REGINA_EVENT_LEVEL_ERROR:
            level = ENUM_EVENT_LEVEL_ERROR;
            break;

        case REGINA_EVENT_LEVEL_WARNING:
            level = ENUM_EVENT_LEVEL_WARNING;
            break;

        case REGINA_EVENT_LEVEL_INFOMATION:
            level = ENUM_EVENT_LEVEL_INFOMATION;
            break;

        case REGINA_EVENT_LEVEL_DEBUG:
            level = ENUM_EVENT_LEVEL_DEBUG;
            break;

        case REGINA_EVENT_LEVEL_USER_DEFINITION_TYPE:
            level = ENUM_EVENT_LEVEL_USER_DEFINITION_TYPE;
            break;

        default:
            *req = regina_union_event_sync_request_or_null_branch_1_create();
            REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
            svc->ack = ENUM_RESPONSE_RESULT_INVALID_DATA;
            svc->waiting_ack = 0;
            return REGINA_ERR_NONE;
        }

        REGINA_LOG_DEBUG0("[EVNT SVC] Extracting SYNC request.");

        *req = regina_union_event_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        ev_req = regina_event_sync_request_create();
        REGINA_RETURN_IF_NIL(ev_req, REGINA_ERR_NOMEM);

        (*req)->data = ev_req;
        ev_req->body = regina_bytes_move_create(
            (const uint8_t*)event->event, event->len, NULL);
        REGINA_RETURN_IF_NIL(ev_req->body, REGINA_ERR_NOMEM);

        ev_req->level = level;
        ev_req->seq_num = ++svc->seq_num;
        ev_req->at = event->timestamp;
        svc->waiting_ack = 1;

        REGINA_LOG_DEBUG0("[EVNT SVC] Compelte to extract SYNC request.");
    }

    return REGINA_ERR_NONE;
}

int event_service_is_waiting_ack(event_service_t *svc)
{
    return svc->waiting_ack;
}

int event_service_handle_sync_resp(event_service_t *svc,
                                   regina_union_t *resp)
{
    if ((resp->type == REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_BRANCH_0) &&
        resp->data) {
        regina_event_sync_response_t *ev_resp;

        ev_resp = (regina_event_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG2("[EVNT SVC] Receive event sync response, local sn=%ld"
                          " remote sn=%ld.", svc->seq_num, ev_resp->seq_num);

        if (svc->seq_num == ev_resp->seq_num) {
            svc->waiting_ack = 0;
            svc->ack = ev_resp->result;
        }
    }

    return REGINA_ERR_NONE;
}

regina_response_result_t event_service_get_and_reset_ack(event_service_t *svc)
{
    regina_response_result_t result;
    result = svc->ack;
    svc->ack = ENUM_RESPONSE_RESULT_SUCCESS;
    return result;
}

void event_service_handle_close(event_service_t *svc)
{
    REGINA_LOG_DEBUG0("[EVNT SVC] Handle close.");
}

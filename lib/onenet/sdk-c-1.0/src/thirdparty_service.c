#include "thirdparty_service.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "std_wrapper.h"


struct thirdparty_service_t {
    int32_t seq_num;
    regina_response_result_t ack;
    int waiting_ack;
    char *resp_body;
    size_t resp_size;
};

int thirdparty_service_create(thirdparty_service_t **svc)
{
    *svc = (thirdparty_service_t*)regina_malloc(sizeof(thirdparty_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(thirdparty_service_t));
    (void)thirdparty_service_get_and_reset_ack(*svc, NULL, NULL);
    return REGINA_ERR_NONE;
}

void thirdparty_service_destroy(thirdparty_service_t *svc)
{
    if (svc) {
        (void)thirdparty_service_get_and_reset_ack(svc, NULL, NULL);
        regina_free(svc);
    }
}

int thirdparty_service_extract_req(thirdparty_service_t *svc,
                                   thirdparty_service_req_t *thrd_req,
                                   regina_union_t **req)
{
    int created = 0;

    if (thrd_req && thrd_req->body && thrd_req->size) {
        REGINA_LOG_DEBUG0("[THRD SVC] Extracting thirdparty service request....");

        if ((REGINA_THIRDPARTY_SERVICE_QOS0 == thrd_req->qos) ||
            (REGINA_THIRDPARTY_SERVICE_QOS1 == thrd_req->qos)) {
            regina_thirdparty_service_request_t *av_req;

            *req = regina_union_thirdparty_service_request_or_null_branch_0_create();
            REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

            av_req = regina_thirdparty_service_request_create();
            REGINA_RETURN_IF_NIL(av_req, REGINA_ERR_NOMEM);

            (*req)->data = av_req;
            av_req->data = regina_bytes_move_create(
                (const uint8_t*)thrd_req->body, thrd_req->size, NULL);
            REGINA_RETURN_IF_NIL(av_req->data, REGINA_ERR_NOMEM);

            av_req->ext_data = regina_union_bytes_or_null_branch_1_create();
            REGINA_RETURN_IF_NIL(av_req->ext_data, REGINA_ERR_NOMEM);

            av_req->qos = thrd_req->qos;
            av_req->seq_num = svc->seq_num;

            svc->waiting_ack = 1;
            created = 1;
        }
        else {
            svc->ack = ENUM_RESPONSE_RESULT_UNSUPPORT;
        }

        REGINA_LOG_DEBUG0("[THRD SVC] Complete to extract thirdparty service request.");
    }

    if (!created) {
        *req = regina_union_thirdparty_service_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

int thirdparty_service_handle_resp(thirdparty_service_t *svc,
                                   regina_union_t *resp)
{
    regina_thirdparty_service_response_t *thrd_resp;

    if (!resp->data ||
        (REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_1 == resp->type))
        return REGINA_ERR_NONE;


    thrd_resp = (regina_thirdparty_service_response_t*)resp->data;
    REGINA_LOG_DEBUG2("[THRD SVC] Receive sync response, local sn=%d, "
                      "remote sn=%d", svc->seq_num, thrd_resp->seq_num);
    if (svc->seq_num != thrd_resp->seq_num)
        return REGINA_ERR_PROTOCOL_ERROR;

    ++svc->seq_num;

    svc->ack = thrd_resp->result;
    svc->waiting_ack = 0;

    if (thrd_resp->data->data &&
        (REGINA_UNION_BYTES_OR_NULL_BRANCH_0 == thrd_resp->data->type)) {
        regina_bytes_t *resp_body;
        resp_body = (regina_bytes_t*)thrd_resp->data->data;

        REGINA_LOG_DEBUG1("Thirdparty service response body has %d bytes.",
                          resp_body->size);

        if (resp_body->size) {
            svc->resp_body = (char*)regina_malloc(resp_body->size);
            REGINA_RETURN_IF_NIL(svc->resp_body, REGINA_ERR_NOMEM);

            regina_memcpy(svc->resp_body, resp_body->buffer, resp_body->size);
            svc->resp_size = resp_body->size;
        }
    }

    return REGINA_ERR_NONE;
}

void thirdparty_service_handle_close(thirdparty_service_t *svc)
{
    svc->seq_num = 0;
    svc->waiting_ack = 0;
    thirdparty_service_get_and_reset_ack(svc, NULL, NULL);
}

int thirdparty_service_is_waiting_ack(thirdparty_service_t *svc)
{
    return svc->waiting_ack;
}

regina_response_result_t thirdparty_service_get_and_reset_ack(thirdparty_service_t *svc,
                                                              char **resp, size_t *size)
{
    regina_response_result_t result;

    if (resp && size) {
        *resp = svc->resp_body;
        *size = svc->resp_size;
    }
    else {
        regina_free(svc->resp_body);
    }

    svc->resp_body = NULL;
    svc->resp_size = 0;

    result = svc->ack;
    svc->ack = ENUM_RESPONSE_RESULT_SUCCESS;

    return result;
}

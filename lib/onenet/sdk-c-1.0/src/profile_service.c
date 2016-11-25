#include "profile_service.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "device_status.h"
#include "security.h"
#include "std_wrapper.h"
#include "acc_dev_gen.h"


struct profile_service_t {
    char prof_md5[MD5_LENGTH];
    int waiting_ack;
    regina_response_result_t ack;
};

int profile_service_create(profile_service_t **svc)
{
    REGINA_RETURN_IF_NIL(svc, REGINA_ERR_BADPARAM);

    *svc = (profile_service_t*)regina_malloc(sizeof(profile_service_t));
    if (!*svc)
        return REGINA_ERR_NOMEM;

    regina_memset(*svc, 0, sizeof(profile_service_t));
    (void)profile_service_get_and_reset_ack(*svc);

    return REGINA_ERR_NONE;
}

void profile_service_destroy(profile_service_t *svc)
{
    if (!svc)
        return;

    regina_free(svc);
}

int profile_service_extract_req(profile_service_t *svc,
                                const char *profile,
                                size_t length,
                                regina_union_t **req)
{

    if (profile && length) {
        const char *auth_code = device_status_get_auth_code();
        if (!auth_code || ('\0' == auth_code[0])){
            regina_profile_sync_request_t *prof_req;

            REGINA_LOG_DEBUG0("[PROF SVC] Extracting SYNC request.");

            *req = regina_union_profile_sync_request_or_null_branch_0_create();
            REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

            prof_req = regina_profile_sync_request_create();
            REGINA_RETURN_IF_NIL(prof_req, REGINA_ERR_NOMEM);

            (*req)->data = prof_req;
            prof_req->profile_body = regina_bytes_move_create(
                (const uint8_t*)profile, length, NULL);
            REGINA_RETURN_IF_NIL(prof_req->profile_body, REGINA_ERR_NOMEM);

            svc->waiting_ack = 1;

            REGINA_LOG_DEBUG0("[PROF SVC] Complete to extract SYNC request.");
            return REGINA_ERR_NONE;
        }
    }

    *req = regina_union_profile_sync_request_or_null_branch_1_create();
    REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

    return REGINA_ERR_NONE;
}

int profile_service_is_waiting_ack(profile_service_t *svc)
{
  return svc->waiting_ack;
}

int profile_service_handle_sync_resp(profile_service_t *svc,
                                     regina_union_t *resp)
{
    if ((resp->type == REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_BRANCH_0) &&
        resp->data) {
        regina_profile_sync_response_t *pf_resp;
        pf_resp = (regina_profile_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG0("[PROF SVC] Receive profile sync response.");

        svc->waiting_ack = 0;
        svc->ack = pf_resp->result;

        if (ENUM_RESPONSE_RESULT_SUCCESS == pf_resp->result)
            device_status_set_profile_md5(svc->prof_md5);
    }

    return REGINA_ERR_NONE;
}

regina_response_result_t profile_service_get_and_reset_ack(profile_service_t *svc)
{
    regina_response_result_t result;
    result = svc->ack;
    svc->ack = ENUM_RESPONSE_RESULT_SUCCESS;
    return result;
}

void profile_service_handle_close(profile_service_t *svc)
{
    REGINA_LOG_DEBUG0("[PROF SVC] Handle close.");
}

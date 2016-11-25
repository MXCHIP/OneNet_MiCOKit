#include "auth_service.h"
#include "regina/allocator.h"
#include "regina/device.h"
#include "common.h"
#include "util.h"
#include "device_status.h"
#include "regina_config.h"
#include "std_wrapper.h"

struct auth_service_t {
    int authenticated;
    int waiting_auth_resp;
};

static int auth_service_fill_sofware_info(regina_list_t *info_list,
                                          const regina_module_info_t *modules,
                                          size_t module_count)
{
    size_t i;

    if (!modules && module_count)
        return REGINA_ERR_BADPARAM;

    for (i = 0; i < module_count; ++i) {
        regina_software_info_t *info = regina_software_info_create();
        REGINA_RETURN_IF_NIL(info, REGINA_ERR_NOMEM);

        if (modules[i].name)
            info->module_name = regina_string_move_create(modules[i].name, NULL);
        else
            info->module_name = regina_string_move_create("", NULL);

        if (modules[i].version)
            info->version = regina_string_move_create(modules[i].version, NULL);
        else
            info->version = regina_string_move_create("", NULL);

        if (!info->module_name || !info->version) {
            info->destroy(info);
            return REGINA_ERR_NOMEM;
        }

        if (!regina_list_push_back(info_list, info))
            return REGINA_ERR_NOMEM;
    }

    return REGINA_ERR_NONE;
}

int auth_service_create(auth_service_t **svc)
{
    *svc = (auth_service_t*)regina_malloc(sizeof(auth_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);
    regina_memset(*svc, 0, sizeof(auth_service_t));
    return REGINA_ERR_NONE;
}

void auth_service_destroy(auth_service_t *svc)
{
    if (svc) {
        regina_free(svc);
    }
}

int auth_service_extract_auth_req(auth_service_t *svc,
                                  const regina_module_info_t *modules,
                                  size_t module_count,
                                  regina_union_t **req)
{
    /* assert(svc); */
    /* assert(req); */

    if (svc->authenticated || device_status_is_new()) {
        *req = regina_union_authen_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
        svc->waiting_auth_resp = 1;
    }
    else {
        regina_authen_sync_request_t *auth_req;
        int err;

        REGINA_LOG_DEBUG0("[AUTH SVC] Extracting authentication request.");

        *req = regina_union_authen_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        auth_req = regina_authen_sync_request_create();
        REGINA_RETURN_IF_NIL(auth_req, REGINA_ERR_NOMEM);
        (*req)->data = auth_req;

        auth_req->device_id = device_status_get_device_id();
        REGINA_RETURN_IF_NIL(auth_req->device_id, REGINA_ERR_NOMEM);

        auth_req->sdk_uuid = regina_string_move_create(regina_config_get_sdk_uuid(), NULL);
        REGINA_RETURN_IF_NIL(auth_req->sdk_uuid, REGINA_ERR_NOMEM);
        auth_req->auth_code = regina_string_move_create(
            device_status_get_auth_code(), NULL);
        REGINA_RETURN_IF_NIL(auth_req->auth_code, REGINA_ERR_NOMEM);

        auth_req->software_info = regina_list_create();
        REGINA_RETURN_IF_NIL(auth_req->software_info, REGINA_ERR_NOMEM);

        err = auth_service_fill_sofware_info(auth_req->software_info,
                                             modules, module_count);
        REGINA_RETURN_IF_ERR(err);

        svc->authenticated = 1;
        svc->waiting_auth_resp = 1;

        REGINA_LOG_DEBUG0("[AUTH SVC] Complete to extract authentication request.");
    }

    return REGINA_ERR_NONE;
}

int auth_service_extract_reg_req(auth_service_t *svc,
                                 const regina_module_info_t *modules,
                                 size_t module_count,
                                 regina_union_t **req)
{
    /* assert(svc); */
    /* assert(req); */

    if (svc->authenticated || !device_status_is_new()) {
        *req = regina_union_register_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_register_sync_request_t *reg_req;
        int err;

        REGINA_LOG_DEBUG0("[AUTH SVC] Extracting register request.");

        *req = regina_union_register_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
        reg_req = regina_register_sync_request_create();
        REGINA_RETURN_IF_NIL(reg_req, REGINA_ERR_NOMEM);

        (*req)->data = reg_req;
        reg_req->regiter_code = regina_string_move_create(
            device_status_get_reg_code(), NULL);
        REGINA_RETURN_IF_NIL(reg_req->regiter_code, REGINA_ERR_NOMEM);
        reg_req->sdk_uuid = regina_string_move_create(
            regina_config_get_sdk_uuid(), NULL);
        REGINA_RETURN_IF_NIL(reg_req->sdk_uuid, REGINA_ERR_NOMEM);


        reg_req->software_info = regina_list_create();
        REGINA_RETURN_IF_NIL(reg_req->software_info, REGINA_ERR_NOMEM);

        err = auth_service_fill_sofware_info(reg_req->software_info,
                                             modules, module_count);
        REGINA_RETURN_IF_ERR(err);


        svc->authenticated = 1;
        svc->waiting_auth_resp = 1;

        REGINA_LOG_DEBUG0("[AUTH SVC] Complete to extract register request.");
    }

    return REGINA_ERR_NONE;
}

int auth_service_handle_reg_resp(auth_service_t *svc,
                                 regina_union_t *resp)
{
    int err;

    regina_register_sync_response_t *reg_resp;
    regina_string_t *auth_code;
    int64_t *device_id;

    /* assert (svc); */
    /* assert (resp); */

    if (!resp->data ||
        (REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_BRANCH_1 == resp->type))
        return REGINA_ERR_NONE;

    svc->waiting_auth_resp = 0;
    reg_resp = (regina_register_sync_response_t*)resp->data;

    if (ENUM_RESPONSE_RESULT_SUCCESS != reg_resp->result) {
        REGINA_LOG1(ERROR, "Failed to register the device, response=%d.", reg_resp->result);
        return REGINA_ERR_REGISTER_FAILED;
    }

    if ((REGINA_UNION_STRING_OR_NULL_BRANCH_0 != reg_resp->device_id->type) ||
        (REGINA_UNION_STRING_OR_NULL_BRANCH_0 != reg_resp->auth_code->type)) {
        REGINA_LOG0(FATAL, "Bug: protocol error of register response message.");
        return REGINA_ERR_PROTOCOL_ERROR;
    }

    device_id = (int64_t*)reg_resp->device_id->data;
    err = device_status_set_device_id(*device_id);
    REGINA_RETURN_IF_ERR(err);

    auth_code = (regina_string_t*)reg_resp->auth_code->data;
    err = device_status_set_auth_code(auth_code->data);
    REGINA_RETURN_IF_ERR(err);

    REGINA_LOG_DEBUG1("[AUTH SVC] Register the device successfully,"
                      " device id is %ld.", *device_id);

    return REGINA_ERR_NONE;
}

int auth_service_handle_auth_resp(auth_service_t *svc,
                                  regina_union_t *resp)
{
    regina_authen_sync_response_t *auth_resp;

    /* assert(svc); */
    /* assert(resp); */

    if (!resp->data ||
        (REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_BRANCH_1 == resp->type))
        return REGINA_ERR_NONE;

    svc->waiting_auth_resp = 0;
    auth_resp = (regina_authen_sync_response_t*)resp->data;
    if (ENUM_RESPONSE_RESULT_SUCCESS != auth_resp->result) {
        REGINA_LOG1(ERROR, "Failed to authenticate, response=%d.", auth_resp->result);
        return REGINA_ERR_NOAUTHORITY;
    }

    REGINA_LOG_DEBUG0("[AUTH SVC] Authenticate the device successfully.");
    return REGINA_ERR_NONE;
}

void auth_service_handle_close(auth_service_t *svc)
{
    REGINA_LOG_DEBUG0("[AUTH SVC] Handle close.");
    svc->authenticated = 0;
}

int auth_service_is_waiting_auth(auth_service_t *svc)
{
    return svc->waiting_auth_resp;
}

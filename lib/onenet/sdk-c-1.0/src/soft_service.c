#include "soft_service.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "device_status.h"
#include "std_wrapper.h"

struct soft_service_t {
    int need_sync;
    int64_t seq_num;
    int64_t ack;

    size_t module_count;
    regina_module_update_info_t *modules;
};

static void soft_service_clear_modules(soft_service_t *svc)
{
    size_t i;
    for (i = 0; i < svc->module_count; ++i) {
        regina_free(svc->modules[i].name);
        regina_free(svc->modules[i].url);
        regina_free(svc->modules[i].version);
    }

    regina_free(svc->modules);
    svc->modules = NULL;
    svc->module_count = 0;
}

int soft_service_create(soft_service_t **svc)
{
    *svc = (soft_service_t*)regina_malloc(sizeof(soft_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(soft_service_t));

    (*svc)->ack = -1;
    (*svc)->need_sync = 1;

    return REGINA_ERR_NONE;
}

void soft_service_destroy(soft_service_t *svc)
{
    if (svc) {
        soft_service_clear_modules(svc);
        regina_free(svc);
    }
}

int soft_service_extract_req(soft_service_t *svc, int sync, regina_union_t **req)
{
    if (!svc->need_sync && !sync) {
        *req = regina_union_soft_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_soft_sync_request_t *soft_req;

        REGINA_LOG_DEBUG0("[SOFT SVC] Extracting SYNC request.");

        *req = regina_union_soft_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        soft_req = regina_soft_sync_request_create();
        REGINA_RETURN_IF_NIL(soft_req, REGINA_ERR_NOMEM);

        (*req)->data = soft_req;
        soft_req->seq_num = svc->seq_num;

        svc->need_sync = 0;

        REGINA_LOG_DEBUG0("[SOFT SVC] Complete to extract SYNC request.");
    }
    return REGINA_ERR_NONE;
}

int soft_service_extract_ack(soft_service_t *svc, regina_union_t **ack)
{
    if (-1 == svc->ack) {
        *ack = regina_union_long_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);
    }
    else {
        int64_t *id;

        REGINA_LOG_DEBUG1("[SOFT SVC] Extract software update ack: %d", svc->ack);

        *ack = regina_union_long_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);

        id = (int64_t*)regina_malloc(sizeof(int64_t));
        REGINA_RETURN_IF_NIL(id, REGINA_ERR_NOMEM);

        (*ack)->data = id;
        *id = svc->ack;
        svc->ack = -1;
    }

    return REGINA_ERR_NONE;
}

int soft_service_handle_sync_resp(soft_service_t *svc, regina_union_t *resp)
{
    int err = REGINA_ERR_NONE;

    if ((REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_soft_sync_response_t *sf_resp;

        sf_resp = (regina_soft_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG2("[SOFT SVC] Receive software update ack, local sn=%ld "
                          "remote sn=%ld", svc->seq_num, sf_resp->seq_num);

        if (sf_resp->seq_num == svc->seq_num)
            ++(svc->seq_num);

        soft_service_clear_modules(svc);
        svc->module_count = regina_list_get_size(sf_resp->software_update_info);
        if (svc->module_count) {
            regina_list_node_t *cursor;
            int i = 0;

            svc->modules = (regina_module_update_info_t*)regina_malloc(
                sizeof(regina_module_update_info_t) * svc->module_count);

            if (!svc->modules) {
                svc->module_count = 0;
                return REGINA_ERR_NOMEM;
            }
            regina_memset(svc->modules, 0,
                          sizeof(regina_module_update_info_t) * svc->module_count);

            for (cursor = regina_list_begin(sf_resp->software_update_info);
                 cursor; cursor = regina_list_next(cursor)) {
                regina_software_update_info_t *info;
                info = (regina_software_update_info_t*)regina_list_get_data(cursor);

                if ((NULL == info->module_name->data) ||
                    (NULL == info->url->data) ||
                    (NULL == info->version->data) ||
                    (16 != info->md5->size)) {
                    err = REGINA_ERR_PROTOCOL_ERROR;
                    break;
                }

                svc->modules[i].name = regina_strdup(info->module_name->data);
                svc->modules[i].url = regina_strdup(info->url->data);
                svc->modules[i].version = regina_strdup(info->version->data);
                regina_memcpy(svc->modules[i].md5, info->md5->buffer, 16);

                if (!svc->modules[i].name || !svc->modules[i].url || !svc->modules[i].version) {
                    err = REGINA_ERR_NOMEM;
                    break;
                }
                ++i;
            }

            if (REGINA_ERR_NONE != err) {
                soft_service_clear_modules(svc);
            }
        }
    }

    return err;
}

int soft_service_handle_soft_update(soft_service_t *svc, void *ctx,
                                    software_update_callback su_callback)
{
    if (svc->module_count) {
        REGINA_LOG_DEBUG0("[SOFT SVC] Handle software update by callback function.");
        su_callback(ctx, svc->modules, svc->module_count);
        soft_service_clear_modules(svc);
        return 1;
    }
    return 0;
}

void soft_service_handle_close(soft_service_t *svc)
{
    REGINA_LOG_DEBUG0("[SOFT SVC] Handle close.");
    soft_service_clear_modules(svc);
}

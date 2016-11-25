#include "notification_service.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "extent.h"
#include "device_status.h"
#include "std_wrapper.h"
#include "acc_dev_gen.h"

#define UUID_LENGTH 36 /* 8-4-4-4-12 */

#define RT_NOTIFICATION_SN  1

struct notification_service_t {
    int need_sync;

    int not_handled;
    char not_id[UUID_LENGTH + 1];
    regina_extent_t not_body;

    int cmd_handled;
    char cmd_id[UUID_LENGTH + 1];
    regina_extent_t cmd_body;

    int waiting_confirmation_ack;
    regina_response_result_t confirmation_ack;
};


typedef enum notification_type_t {
    NT_NORMAL = 0xc6,
    NT_CMD    = 0xc8
} notification_type_t;

static void notification_service_clear_notification(notification_service_t *svc)
{
    regina_free(svc->not_body.base);
    svc->not_body.base = NULL;
    svc->not_body.len = 0;
    svc->not_handled = 1;

    regina_free(svc->cmd_body.base);
    svc->cmd_body.base = NULL;
    svc->cmd_body.len = 0;
    svc->cmd_handled = 1;
}

int notification_service_create(notification_service_t **svc)
{
    REGINA_RETURN_IF_NIL(svc, REGINA_ERR_BADPARAM);

    *svc = (notification_service_t*)regina_malloc(sizeof(notification_service_t));
    REGINA_RETURN_IF_NIL(*svc, REGINA_ERR_NOMEM);

    regina_memset(*svc, 0, sizeof(notification_service_t));
    (*svc)->need_sync = 1;
    (void)notification_service_get_and_reset_confirmation_ack(*svc);
    return REGINA_ERR_NONE;
}

void notification_service_destroy(notification_service_t *svc)
{
    if (svc) {
        notification_service_clear_notification(svc);
        regina_free(svc);
    }
}

int notification_service_extract_req(notification_service_t *svc,
                                     regina_union_t **req)
{
    REGINA_RETURN_IF_NIL2(svc, req, REGINA_ERR_BADPARAM);

    if (!svc->need_sync) {
        *req = regina_union_notify_sync_request_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);
    }
    else {
        regina_notify_sync_request_t *not_req;

        REGINA_LOG_DEBUG0("[NOTI SVC] Extracting SYNC request.");

        *req = regina_union_notify_sync_request_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(*req, REGINA_ERR_NOMEM);

        not_req = regina_notify_sync_request_create();
        REGINA_RETURN_IF_NIL(not_req, REGINA_ERR_NOMEM);

        (*req)->data = not_req;
        not_req->seq_num = device_status_get_notification_sn();
        svc->need_sync = 0;

        REGINA_LOG_DEBUG0("[NOTI SVC] Complete to extract SYNC request.");
    }

    return REGINA_ERR_NONE;
}

int notification_service_extract_confirmation(notification_service_t *svc,
                                              const regina_notification_id_t *id,
                                              const char *confirmation,
                                              size_t length,
                                              regina_sync_request_t *req)
{
    if (!confirmation || !length ) {
        req->notify_confirm = regina_union_notify_confirm_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(req->notify_confirm, REGINA_ERR_NOMEM);

        req->cmd_confirm = regina_union_cmd_confirm_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(req->cmd_confirm, REGINA_ERR_NOMEM);

        return REGINA_ERR_NONE;
    }

    if (!id->uuid || ((NT_NORMAL != id->internal) && (NT_CMD != id->internal)))
        return REGINA_ERR_BADPARAM;

    if (NT_NORMAL == id->internal) {
        regina_notify_confirm_t *cfm_req;

        req->cmd_confirm = regina_union_cmd_confirm_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(req->cmd_confirm, REGINA_ERR_NOMEM);

        REGINA_LOG_DEBUG1("[NOTI SVC] Extracting confirmation of the notification(%s).",
                          id->uuid);

        req->notify_confirm = regina_union_notify_confirm_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(req->notify_confirm, REGINA_ERR_NOMEM);

        cfm_req = regina_notify_confirm_create();
        REGINA_RETURN_IF_NIL(cfm_req, REGINA_ERR_NOMEM);
        req->notify_confirm->data = cfm_req;

        cfm_req->uuid = regina_string_move_create(id->uuid, NULL);
        cfm_req->resp_nf_body = regina_bytes_move_create(
            (const uint8_t*)confirmation, length, NULL);

        REGINA_RETURN_IF_NIL2(cfm_req->uuid, cfm_req->resp_nf_body, REGINA_ERR_NOMEM);

        REGINA_LOG_DEBUG0("[NOTI SVC] Complete to extract confirmation.");
    }
    else {
        regina_cmd_confirm_t *cmd_req;

        req->notify_confirm = regina_union_notify_confirm_or_null_branch_1_create();
        REGINA_RETURN_IF_NIL(req->notify_confirm, REGINA_ERR_NOMEM);

        REGINA_LOG_DEBUG1("[NOTI SVC] Extracting confirmation of the command(%s).",
                          id->uuid);
        req->cmd_confirm = regina_union_cmd_confirm_or_null_branch_0_create();
        REGINA_RETURN_IF_NIL(req->cmd_confirm, REGINA_ERR_NOMEM);

        cmd_req = regina_cmd_confirm_create();
        REGINA_RETURN_IF_NIL(cmd_req, REGINA_ERR_NOMEM);
        req->cmd_confirm->data = cmd_req;

        cmd_req->uuid = regina_string_move_create(id->uuid, NULL);
        cmd_req->resp_nf_body = regina_bytes_move_create(
            (const uint8_t*)confirmation, length, NULL);

        REGINA_RETURN_IF_NIL2(cmd_req->uuid, cmd_req->resp_nf_body, REGINA_ERR_NOMEM);
        REGINA_LOG_DEBUG0("[NOTI SVC] Complete to extract confirmation.");
    }

    svc->waiting_confirmation_ack = 1;
    return REGINA_ERR_NONE;
}

int notification_service_extract_syncack(notification_service_t *svc,
                                         regina_union_t **ack)
{
    REGINA_RETURN_IF_NIL2(svc, ack, REGINA_ERR_BADPARAM);

    if (('\0' != svc->not_id[0]) && svc->not_handled) {
        regina_string_t *uuid;

        REGINA_LOG_DEBUG1("[NOTI SVC] Extracting ack: %s.", svc->not_id);

        *ack = regina_union_null_or_string_branch_1_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);

        uuid = regina_string_copy_create(svc->not_id);
        REGINA_RETURN_IF_NIL(uuid, REGINA_ERR_NOMEM);

        (*ack)->data = uuid;
        svc->not_id[0] = '\0';

        REGINA_LOG_DEBUG0("[NOTI SVC] Complete to extract ack.");
    }
    else {
        *ack = regina_union_null_or_string_branch_0_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

int notification_service_extract_cmdack(notification_service_t *svc,
                                        regina_union_t **ack)
{
    if (('\0' != svc->cmd_id[0]) && svc->cmd_handled) {
        regina_string_t *uuid;

        REGINA_LOG_DEBUG1("[NOTI SVC] Extracting cmd ack: %s.", svc->cmd_id);

        *ack = regina_union_null_or_string_branch_1_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);

        uuid = regina_string_copy_create(svc->cmd_id);
        REGINA_RETURN_IF_NIL(uuid, REGINA_ERR_NOMEM);

        (*ack)->data = uuid;
        svc->cmd_id[0] = '\0';

        REGINA_LOG_DEBUG0("[NOTI SVC] Complete to extract ack.");
    }
    else {
        *ack = regina_union_null_or_string_branch_0_create();
        REGINA_RETURN_IF_NIL(*ack, REGINA_ERR_NOMEM);
    }

    return REGINA_ERR_NONE;
}

int notification_service_is_waiting_confirmation_ack(notification_service_t *svc)
{
    return svc->waiting_confirmation_ack;
}

int notification_service_handle_sync_resp(notification_service_t *svc,
                                          regina_union_t *resp)
{
    if ((REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_notify_sync_response_t *nt_resp;
        int64_t sn;
        char body_type;

        nt_resp = (regina_notify_sync_response_t*)resp->data;
        sn = device_status_get_notification_sn();

        REGINA_LOG_DEBUG2("[NOTI SVC] Receive notification sync response, "
                          "local sn=%ld, remote sn=%ld.",
                          sn, nt_resp->seq_num);

        body_type = nt_resp->notification_body->type;
        if ((REGINA_UNION_BYTES_OR_NULL_BRANCH_0 == body_type) &&
            nt_resp->notification_body->data) {

            regina_bytes_t *body = (regina_bytes_t*)nt_resp->notification_body->data;
            regina_string_t *uuid;
            size_t uuid_len;

            REGINA_LOG_DEBUG1("[NOTI SVC] the size of the notification is %lu.", body->size);
            if ((nt_resp->uuid->type == REGINA_UNION_STRING_OR_NULL_BRANCH_1) ||
                !nt_resp->uuid->data) {
                REGINA_LOG0(FATAL, "Bug: invalid uuid of the notification.");
                return REGINA_ERR_PROTOCOL_ERROR;
            }

            uuid = (regina_string_t*)nt_resp->uuid->data;
            uuid_len = regina_strlen(uuid->data);
            if (UUID_LENGTH != uuid_len) {
                REGINA_LOG1(FATAL, "Bug: invalid uuid of the notification,"
                            " length=%d.", uuid_len);
                return REGINA_ERR_PROTOCOL_ERROR;
            }

            regina_free(svc->not_body.base);
            svc->not_body.base = (char*)regina_malloc(body->size);
            REGINA_RETURN_IF_NIL(svc->not_body.base, REGINA_ERR_NOMEM);
            regina_memcpy(svc->not_body.base, body->buffer, body->size);
            svc->not_body.len = body->size;

            regina_strcpy(svc->not_id, uuid->data);
            svc->not_handled = 0;
            REGINA_LOG_DEBUG0("[NOTI SVC] handle notification successfully.");
        }

        /* need to sync with new sequence number. */
        if (RT_NOTIFICATION_SN == nt_resp->seq_num) {
            svc->need_sync = 1;
        }
        else if (nt_resp->seq_num > sn) {
            svc->need_sync = 1;
            device_status_set_notification_sn(nt_resp->seq_num);
        }
    }

    return REGINA_ERR_NONE;
}

int notification_service_handle_cmd_resp(notification_service_t *svc,
                                         regina_union_t *resp)
{
    if ((REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_cmd_sync_response_t *cmd_resp;
        size_t uuid_len;


        cmd_resp = (regina_cmd_sync_response_t*)resp->data;

        REGINA_LOG_DEBUG0("[NOTI SVC] Receive CMD sync response");

        uuid_len = regina_strlen(cmd_resp->uuid->data);
        if (UUID_LENGTH != uuid_len) {
            REGINA_LOG1(FATAL, "Bug: invalid uuid of the notification,"
                        " length=%d.", uuid_len);
            return REGINA_ERR_PROTOCOL_ERROR;
        }

        regina_free(svc->cmd_body.base);
        svc->cmd_body.base = (char*)regina_malloc(cmd_resp->cmd_body->size);
        REGINA_RETURN_IF_NIL(svc->cmd_body.base, REGINA_ERR_NOMEM);
        regina_memcpy(svc->cmd_body.base, cmd_resp->cmd_body->buffer,
                      cmd_resp->cmd_body->size);
        svc->cmd_body.len = cmd_resp->cmd_body->size;

        regina_strcpy(svc->cmd_id, cmd_resp->uuid->data);
        svc->cmd_handled = 0;
        REGINA_LOG_DEBUG0("[NOTI SVC] handle CMD successfully.");
    }

    return REGINA_ERR_NONE;
}

int notification_service_handle_confirmation_ack(notification_service_t *svc,
                                                 regina_union_t *resp)
{
    if ((REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_notify_confirm_ack_t *ack;
        ack = (regina_notify_confirm_ack_t*)resp->data;

        if (!ack->notify_confirm_ack->data) {
            REGINA_LOG0(FATAL, "Bug: the uuid in the notification confirmation ack is NULL.");
            return REGINA_ERR_INTERNAL;
        }

        REGINA_LOG_DEBUG1("[NOTI SVC] Receive notification confirmation ack, UUID=(%s).",
                          ack->notify_confirm_ack->data);

        svc->confirmation_ack = ack->result;
        svc->waiting_confirmation_ack = 0;
    }

    return REGINA_ERR_NONE;
}

int notification_service_handle_cmdconfirm_ack(notification_service_t *svc,
                                               regina_union_t *resp)
{
    if ((REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_BRANCH_0 == resp->type) &&
        resp->data) {
        regina_cmd_confirm_ack_t *ack;
        ack = (regina_cmd_confirm_ack_t*)resp->data;

        if (!ack->cmd_confirm_ack->data) {
            REGINA_LOG0(FATAL, "Bug: the uuid in the cmd confirmation ack is NULL.");
            return REGINA_ERR_INTERNAL;
        }

        REGINA_LOG_DEBUG1("[NOTI SVC] Receive cmd confirmation ack, UUID=(%s).",
                          ack->cmd_confirm_ack->data);

        svc->confirmation_ack = ack->result;
        svc->waiting_confirmation_ack = 0;
    }

    return REGINA_ERR_NONE;
}

int notification_service_handle_notification(notification_service_t *svc,
                                             void *ctx, notification_callback nt_callback)
{
    int has_ack = 0;
    regina_notification_id_t id;

    if (svc->not_body.base && svc->not_body.len) {
        REGINA_LOG_DEBUG0("[NOTI SVC] handle notifcation by callback handler.");

        id.uuid = svc->not_id;
        id.internal = NT_NORMAL;

        nt_callback(ctx, &id, svc->not_body.base, svc->not_body.len);
        has_ack = 1;
    }

    if (svc->cmd_body.base && svc->cmd_body.len) {
        REGINA_LOG_DEBUG0("[NOTI SVC] handle cmd by callback handler.");

        id.uuid = svc->cmd_id;
        id.internal = NT_CMD;

        nt_callback(ctx, &id, svc->cmd_body.base, svc->cmd_body.len);
        has_ack = 1;
    }

    notification_service_clear_notification(svc);
    return has_ack;
}

regina_response_result_t notification_service_get_and_reset_confirmation_ack(
    notification_service_t *svc)
{
    regina_response_result_t result;
    result = svc->confirmation_ack;
    svc->confirmation_ack = ENUM_RESPONSE_RESULT_SUCCESS;
    return result;
}

void notification_service_handle_close(notification_service_t *svc)
{
    REGINA_LOG_DEBUG0("[NOTI SVC] Handle close.");
    notification_service_clear_notification(svc);
    svc->need_sync = 1;
}

void notification_service_sync(notification_service_t *svc)
{
    svc->need_sync = 1;
}

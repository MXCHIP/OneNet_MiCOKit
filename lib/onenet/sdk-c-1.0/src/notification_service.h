#ifndef __REGINA_NOTIFICATION_SERVICE_H__
#define __REGINA_NOTIFICATION_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "regina/device.h"
#include "acc_dev_gen.h"

typedef struct notification_service_t notification_service_t;
struct regina_extent_t;
struct regina_notification_id_t;

int notification_service_create(notification_service_t **svc);

void notification_service_destroy(notification_service_t *svc);

int notification_service_extract_req(notification_service_t *svc,
                                     regina_union_t **req);
int notification_service_extract_syncack(notification_service_t *svc,
                                         regina_union_t **ack);
int notification_service_extract_cmdack(notification_service_t *svc,
                                        regina_union_t **ack);
int notification_service_extract_confirmation(notification_service_t *svc,
                                              const struct regina_notification_id_t *id,
                                              const char *confirmation,
                                              size_t length,
                                              regina_sync_request_t *req);

int notification_service_is_waiting_confirmation_ack(notification_service_t *svc);
int notification_service_handle_sync_resp(notification_service_t *svc,
                                          regina_union_t *resp);
int notification_service_handle_cmd_resp(notification_service_t *svc,
                                         regina_union_t *resp);
int notification_service_handle_confirmation_ack(notification_service_t *svc,
                                                 regina_union_t *resp);
int notification_service_handle_cmdconfirm_ack(notification_service_t *svc,
                                               regina_union_t *resp);
void notification_service_sync(notification_service_t *svc);

/**
 * return none-zero if has ack to send, otherwise return 0.
 */
int notification_service_handle_notification(notification_service_t *svc, void *ctx,
                                             notification_callback cf_callback);

regina_response_result_t notification_service_get_and_reset_confirmation_ack(
    notification_service_t *svc);

void notification_service_handle_close(notification_service_t *svc);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __REGINA_NOTIFICATION_SERVICE_H__ */

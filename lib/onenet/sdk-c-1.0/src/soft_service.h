#ifndef __REGINA_SOFT_SERVICE_H__
#define __REGINA_SOFT_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "regina/device.h"
#include "common.h"
#include "acc_dev_gen.h"

typedef struct soft_service_t soft_service_t;

int soft_service_create(soft_service_t **svc);
void soft_service_destroy(soft_service_t *svc);

int soft_service_extract_req(soft_service_t *svc, int sync, regina_union_t **req);
int soft_service_extract_ack(soft_service_t *svc, regina_union_t **ack);
int soft_service_handle_sync_resp(soft_service_t *svc, regina_union_t *resp);
int soft_service_handle_soft_update(soft_service_t *svc, void *ctx,
                                    software_update_callback su_callback);
void soft_service_handle_close(soft_service_t *svc);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __REGINA_SOFT_SERVICE_H__ */

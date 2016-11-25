#ifndef __REGINA_CONFIGURATION_SERVICE_H__
#define __REGINA_CONFIGURATION_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "regina/device.h"
#include "common.h"
#include "acc_dev_gen.h"

typedef struct configuration_service_t configuration_service_t;

int configuration_service_create(configuration_service_t **svc);
void configuration_service_destroy(configuration_service_t *svc);

/* @conf is created by SDK, client need to destroy it */
int configuration_service_get_config(configuration_service_t *svc,
                                     char **conf, size_t *size);

int configuration_service_extract_req(configuration_service_t *svc,
                                      regina_union_t **req);

int configuration_service_extract_ack(configuration_service_t *svc,
                                      regina_union_t **ack);

int configuration_service_handle_sync_resp(configuration_service_t *svc,
                                           regina_union_t *resp);
int configuration_service_handle_config(configuration_service_t *svc, void *ctx,
                                        configuration_callback cf_callback);
void configuration_service_handle_close(configuration_service_t *svc);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_CONFIGURATION_SERVICE_H__ */

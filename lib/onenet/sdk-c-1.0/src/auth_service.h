#ifndef __REGINA_AUTH_SERVICE_H__
#define __REGINA_AUTH_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "acc_dev_gen.h"

typedef struct auth_service_t auth_service_t;
struct regina_module_info_t;

int auth_service_create(auth_service_t **svc);
void auth_service_destroy(auth_service_t *svc);

int auth_service_extract_auth_req(auth_service_t *svc,
                                  const struct regina_module_info_t *modules,
                                  size_t module_count,
                                  regina_union_t **req);
int auth_service_extract_reg_req(auth_service_t *svc,
                                 const struct regina_module_info_t *modules,
                                 size_t module_count,
                                 regina_union_t **req);
int auth_service_handle_reg_resp(auth_service_t *svc,
                                 regina_union_t *resp);
int auth_service_handle_auth_resp(auth_service_t *svc,
                                  regina_union_t *resp);
void auth_service_handle_close(auth_service_t *svc);

int auth_service_is_waiting_auth(auth_service_t *svc);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __REGINA_AUTH_SERVICE_H__ */

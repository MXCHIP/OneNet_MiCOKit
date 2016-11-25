#ifndef __REGINA_PROFILE_SERVICE_H__
#define __REGINA_PROFILE_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "acc_dev_gen.h"

typedef struct profile_service_t profile_service_t;

int profile_service_create(profile_service_t **svc);
void profile_service_destroy(profile_service_t *svc);

int profile_service_extract_req(profile_service_t *svc, const char *profile,
                                size_t length, regina_union_t **req);
int profile_service_is_waiting_ack(profile_service_t *svc);
int profile_service_handle_sync_resp(profile_service_t *svc,
                                     regina_union_t *resp);
regina_response_result_t profile_service_get_and_reset_ack(profile_service_t *svc);
void profile_service_handle_close(profile_service_t *svc);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_PROFILE_SERVICE_H__ */

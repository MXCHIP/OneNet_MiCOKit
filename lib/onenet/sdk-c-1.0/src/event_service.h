#ifndef __REGINA_EVENT_SERVICE_H__
#define __REGINA_EVENT_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "regina/device.h"
#include "acc_dev_gen.h"

typedef struct event_service_t event_service_t;
struct regina_event_t;

int event_service_create(event_service_t **svc);
void event_service_destroy(event_service_t *svc);

int event_service_extract_req(event_service_t *svc,
                              const struct regina_event_t *event,
                              regina_union_t **req);
int event_service_is_waiting_ack(event_service_t *svc);
int event_service_handle_sync_resp(event_service_t *svc,
                                   regina_union_t *resp);
regina_response_result_t event_service_get_and_reset_ack(event_service_t *svc);
void event_service_handle_close(event_service_t *svc);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_EVENT_SERVICE_H__ */

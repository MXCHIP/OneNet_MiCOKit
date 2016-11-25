#ifndef __REGINA_THIRDPARTY_SERVICE_H__
#define __REGINA_THIRDPARTY_SERVICE_H__

#include "regina/device.h"
#include "acc_dev_gen.h"


typedef struct thirdparty_service_t thirdparty_service_t;

typedef struct thirdparty_service_req_t {
    const char *body;
    size_t size;
    regina_thirdparty_service_qos_t qos;
} thirdparty_service_req_t;

int thirdparty_service_create(thirdparty_service_t **svc);
void thirdparty_service_destroy(thirdparty_service_t *svc);

int thirdparty_service_extract_req(thirdparty_service_t *svc,
                                   thirdparty_service_req_t *thrd_req,
                                   regina_union_t **req);
int thirdparty_service_handle_resp(thirdparty_service_t *svc,
                                   regina_union_t *resp);
void thirdparty_service_handle_close(thirdparty_service_t *svc);
int thirdparty_service_is_waiting_ack(thirdparty_service_t *svc);
/**
 * resp must be destroyed by regina_free() if no use.
 */
regina_response_result_t thirdparty_service_get_and_reset_ack(thirdparty_service_t *svc,
                                                              char **resp, size_t *size);



#endif /* __REGINA_THIRDPARTY_SERVICE_H__ */

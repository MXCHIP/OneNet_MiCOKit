#ifndef __REGINA_SENSOR_DATA_SERVICE_H__
#define __REGINA_SENSOR_DATA_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "acc_dev_gen.h"

typedef struct sensor_data_service_t sensor_data_service_t;
struct regina_sensor_data_t;

int sensor_data_service_create(sensor_data_service_t **svc);
void sensor_data_service_destroy(sensor_data_service_t *svc);

int sensor_data_service_extract_req(sensor_data_service_t *svc,
                                    const struct regina_sensor_data_t *data,
                                    size_t count,
                                    regina_union_t **req);
int sensor_data_service_is_waiting_ack(sensor_data_service_t *svc);
int sensor_data_service_handle_sync_resp(sensor_data_service_t *svc,
                                         regina_union_t *resp);

/**
 * @remark results must be destroyed by regina_free if no use.
 */
void sensor_data_service_get_and_reset_ack(sensor_data_service_t *svc,
                                           regina_response_result_t **results,
                                           size_t *count);

void sensor_data_service_handle_close(sensor_data_service_t *svc);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_SENSOR_DATA_SERVICE_H__ */

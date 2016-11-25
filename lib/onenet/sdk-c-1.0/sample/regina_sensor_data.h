# ifndef REGINA_SENSOR_DATA_H_
# define REGINA_SENSOR_DATA_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    float speed;
    float temperature;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p12_t4_sensor_data_t;

p12_t4_sensor_data_t *p12_t4_sensor_data_create();
p12_t4_sensor_data_t *p12_t4_sensor_data_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
# ifndef REGINA_SENSOR_DATA_H_
# define REGINA_SENSOR_DATA_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    int32_t temperature;
    int32_t humidity;
    int32_t light_sensor;
    int32_t infrared;
    int8_t dc_motor;
    float hues;
    float saturation;
    float brightness;
    int8_t lamp_switch;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p72416_t4_sensordata_t;

p72416_t4_sensordata_t *p72416_t4_sensordata_create();
p72416_t4_sensordata_t *p72416_t4_sensordata_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
# ifndef SDK_CONF_GEN_H_
# define SDK_CONF_GEN_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    int32_t protocol;
    regina_string_t * host;
    int32_t port;
    regina_bytes_t * public_key;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_bootstrap_access_point_t;

regina_bootstrap_access_point_t *regina_bootstrap_access_point_create();
regina_bootstrap_access_point_t *regina_bootstrap_access_point_deserialize(avro_reader_t reader);

typedef struct {
    int32_t max_size;
    int32_t min_interval;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sensor_data_limit_t;

regina_sensor_data_limit_t *regina_sensor_data_limit_create();
regina_sensor_data_limit_t *regina_sensor_data_limit_deserialize(avro_reader_t reader);

typedef struct {
    int32_t max_size;
    int32_t max_count_per_day;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_event_limit_t;

regina_event_limit_t *regina_event_limit_create();
regina_event_limit_t *regina_event_limit_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * sdk_uuid;
    regina_list_t * bootstrap_access_points;
    regina_sensor_data_limit_t * sensor_data_limit;
    regina_event_limit_t * event_limit;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sdk_config_t;

regina_sdk_config_t *regina_sdk_config_create();
regina_sdk_config_t *regina_sdk_config_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
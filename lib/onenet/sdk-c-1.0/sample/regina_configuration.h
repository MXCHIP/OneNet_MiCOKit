# ifndef REGINA_CONFIGURATION_H_
# define REGINA_CONFIGURATION_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    int32_t sample_interval;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p12_t1_configuration_t;

p12_t1_configuration_t *p12_t1_configuration_create();
p12_t1_configuration_t *p12_t1_configuration_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
# ifndef REGINA_EVENT_H_
# define REGINA_EVENT_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    float pression;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p12_t2_event_t;

p12_t2_event_t *p12_t2_event_create();
p12_t2_event_t *p12_t2_event_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
# ifndef REGINA_EVENT_H_
# define REGINA_EVENT_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    int8_t is_sensor;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p72416_t2_event_t;

p72416_t2_event_t *p72416_t2_event_create();
p72416_t2_event_t *p72416_t2_event_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
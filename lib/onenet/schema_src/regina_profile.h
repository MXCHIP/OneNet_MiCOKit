# ifndef REGINA_PROFILE_H_
# define REGINA_PROFILE_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    regina_string_t * mac;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p72416_t0_profile_t;

p72416_t0_profile_t *p72416_t0_profile_create();
p72416_t0_profile_t *p72416_t0_profile_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
# ifndef REGINA_NOTIFICATION_H_
# define REGINA_NOTIFICATION_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

# ifndef P12_T3_UNION_NULL_OR_STRING_H_
# define P12_T3_UNION_NULL_OR_STRING_H_

# define P12_T3_UNION_NULL_OR_STRING_BRANCH_0    0
# define P12_T3_UNION_NULL_OR_STRING_BRANCH_1    1

regina_union_t *p12_t3_union_null_or_string_branch_0_create();
regina_union_t *p12_t3_union_null_or_string_branch_1_create();

regina_union_t *p12_t3_union_null_or_string_deserialize(avro_reader_t reader);

# endif /* P12_T3_UNION_NULL_OR_STRING_H_ */

typedef struct {
    regina_string_t * news;
    regina_union_t * confirmation;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p12_t3_notification_t;

p12_t3_notification_t *p12_t3_notification_create();
p12_t3_notification_t *p12_t3_notification_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
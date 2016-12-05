# ifndef REGINA_NOTIFICATION_H_
# define REGINA_NOTIFICATION_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

# ifndef P72416_T3_UNION_NULL_OR_FLOAT_H_
# define P72416_T3_UNION_NULL_OR_FLOAT_H_

# define P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_0    0
# define P72416_T3_UNION_NULL_OR_FLOAT_BRANCH_1    1

regina_union_t *p72416_t3_union_null_or_float_branch_0_create();
regina_union_t *p72416_t3_union_null_or_float_branch_1_create();

regina_union_t *p72416_t3_union_null_or_float_deserialize(avro_reader_t reader);

# endif /* P72416_T3_UNION_NULL_OR_FLOAT_H_ */

# ifndef P72416_T3_UNION_NULL_OR_BOOLEAN_H_
# define P72416_T3_UNION_NULL_OR_BOOLEAN_H_

# define P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_0    0
# define P72416_T3_UNION_NULL_OR_BOOLEAN_BRANCH_1    1

regina_union_t *p72416_t3_union_null_or_boolean_branch_0_create();
regina_union_t *p72416_t3_union_null_or_boolean_branch_1_create();

regina_union_t *p72416_t3_union_null_or_boolean_deserialize(avro_reader_t reader);

# endif /* P72416_T3_UNION_NULL_OR_BOOLEAN_H_ */

typedef struct {
    regina_union_t * hues;
    regina_union_t * saturation;
    regina_union_t * brightness;
    regina_union_t * dc_motor;
    regina_union_t * lamp_switch;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} p72416_t3_notification_t;

p72416_t3_notification_t *p72416_t3_notification_create();
p72416_t3_notification_t *p72416_t3_notification_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
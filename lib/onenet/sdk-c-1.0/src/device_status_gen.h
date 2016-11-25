# ifndef DEVICE_STATUS_GEN_H_
# define DEVICE_STATUS_GEN_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct {
    int32_t mode;
    regina_string_t * reg_code;
    regina_string_t * auth_code;
    int64_t device_id;
    int64_t notification_sn;
    regina_bytes_t * profile_md5;
    regina_bytes_t * conf_md5;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_device_status_t;

regina_device_status_t *regina_device_status_create();
regina_device_status_t *regina_device_status_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
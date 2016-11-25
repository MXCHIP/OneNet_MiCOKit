# ifndef BS_DEV_GEN_H_
# define BS_DEV_GEN_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif


typedef enum {
    ENUM_SESSION_KEY_TYPE_SESSION_KEY_NONE,
    ENUM_SESSION_KEY_TYPE_SESSION_KEY_AES
} regina_session_key_type_t;

#ifdef GENC_ENUM_DEFINE_ALIASES
#define SESSION_KEY_NONE ENUM_SESSION_KEY_TYPE_SESSION_KEY_NONE
#define SESSION_KEY_AES ENUM_SESSION_KEY_TYPE_SESSION_KEY_AES
# endif /* GENC_ENUM_DEFINE_ALIASES */

#ifdef GENC_ENUM_STRING_LITERALS
const char* REGINA_SESSION_KEY_TYPE_SYMBOLS[2] = {
    "SESSION_KEY_NONE",
    "SESSION_KEY_AES"};
# endif /* GENC_ENUM_STRING_LITERALS */


typedef enum {
    ENUM_DEVICE_MODE_NORMAL,
    ENUM_DEVICE_MODE_DEBUG,
    ENUM_DEVICE_MODE_MIX
} regina_device_mode_t;

#ifdef GENC_ENUM_DEFINE_ALIASES
#define NORMAL ENUM_DEVICE_MODE_NORMAL
#define DEBUG ENUM_DEVICE_MODE_DEBUG
#define MIX ENUM_DEVICE_MODE_MIX
# endif /* GENC_ENUM_DEFINE_ALIASES */

#ifdef GENC_ENUM_STRING_LITERALS
const char* REGINA_DEVICE_MODE_SYMBOLS[3] = {
    "NORMAL",
    "DEBUG",
    "MIX"};
# endif /* GENC_ENUM_STRING_LITERALS */

# ifndef REGINA_UNION_NULL_OR_BYTES_H_
# define REGINA_UNION_NULL_OR_BYTES_H_

# define REGINA_UNION_NULL_OR_BYTES_BRANCH_0    0
# define REGINA_UNION_NULL_OR_BYTES_BRANCH_1    1

regina_union_t *regina_union_null_or_bytes_branch_0_create();
regina_union_t *regina_union_null_or_bytes_branch_1_create();

regina_union_t *regina_union_null_or_bytes_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_NULL_OR_BYTES_H_ */

typedef struct {
    regina_union_t * key;
    regina_bytes_t * token;
    int32_t token_live_time;
    regina_bytes_t * ip;
    int32_t port;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_acceptor_node_t;

regina_acceptor_node_t *regina_acceptor_node_create();
regina_acceptor_node_t *regina_acceptor_node_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * sdk_uuid;
    regina_device_mode_t mode;
    regina_union_t * session_key;
    regina_session_key_type_t acc_session_key_type;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_access_request_t;

regina_access_request_t *regina_access_request_create();
regina_access_request_t *regina_access_request_deserialize(avro_reader_t reader);

typedef struct {
    regina_list_t * acc_list;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_access_response_t;

regina_access_response_t *regina_access_response_create();
regina_access_response_t *regina_access_response_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
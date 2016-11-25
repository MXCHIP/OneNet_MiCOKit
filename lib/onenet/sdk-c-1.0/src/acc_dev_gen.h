# ifndef ACC_DEV_GEN_H_
# define ACC_DEV_GEN_H_

# include "regina/regina_common_schema.h"

# ifdef __cplusplus
extern "C" {
# endif


typedef enum {
    ENUM_RESPONSE_RESULT_SUCCESS,
    ENUM_RESPONSE_RESULT_INTERNAL_ERROR,
    ENUM_RESPONSE_RESULT_INVALID_DATA,
    ENUM_RESPONSE_RESULT_FREQ_LIMIT,
    ENUM_RESPONSE_RESULT_COUNT_LIMIT,
    ENUM_RESPONSE_RESULT_DATA_SIZE_LIMIT,
    ENUM_RESPONSE_RESULT_UNSUPPORT
} regina_response_result_t;

#ifdef GENC_ENUM_DEFINE_ALIASES
#define SUCCESS ENUM_RESPONSE_RESULT_SUCCESS
#define INTERNAL_ERROR ENUM_RESPONSE_RESULT_INTERNAL_ERROR
#define INVALID_DATA ENUM_RESPONSE_RESULT_INVALID_DATA
#define FREQ_LIMIT ENUM_RESPONSE_RESULT_FREQ_LIMIT
#define COUNT_LIMIT ENUM_RESPONSE_RESULT_COUNT_LIMIT
#define DATA_SIZE_LIMIT ENUM_RESPONSE_RESULT_DATA_SIZE_LIMIT
#define UNSUPPORT ENUM_RESPONSE_RESULT_UNSUPPORT
# endif /* GENC_ENUM_DEFINE_ALIASES */

#ifdef GENC_ENUM_STRING_LITERALS
const char* REGINA_RESPONSE_RESULT_SYMBOLS[7] = {
    "SUCCESS",
    "INTERNAL_ERROR",
    "INVALID_DATA",
    "FREQ_LIMIT",
    "COUNT_LIMIT",
    "DATA_SIZE_LIMIT",
    "UNSUPPORT"};
# endif /* GENC_ENUM_STRING_LITERALS */


typedef enum {
    ENUM_RESPONSE_TYPE_NOTIFY,
    ENUM_RESPONSE_TYPE_CONFIG,
    ENUM_RESPONSE_TYPE_SOFTWARE
} regina_response_type_t;

#ifdef GENC_ENUM_DEFINE_ALIASES
#define NOTIFY ENUM_RESPONSE_TYPE_NOTIFY
#define CONFIG ENUM_RESPONSE_TYPE_CONFIG
#define SOFTWARE ENUM_RESPONSE_TYPE_SOFTWARE
# endif /* GENC_ENUM_DEFINE_ALIASES */

#ifdef GENC_ENUM_STRING_LITERALS
const char* REGINA_RESPONSE_TYPE_SYMBOLS[3] = {
    "NOTIFY",
    "CONFIG",
    "SOFTWARE"};
# endif /* GENC_ENUM_STRING_LITERALS */

# ifndef REGINA_UNION_NULL_OR_STRING_H_
# define REGINA_UNION_NULL_OR_STRING_H_

# define REGINA_UNION_NULL_OR_STRING_BRANCH_0    0
# define REGINA_UNION_NULL_OR_STRING_BRANCH_1    1

regina_union_t *regina_union_null_or_string_branch_0_create();
regina_union_t *regina_union_null_or_string_branch_1_create();

regina_union_t *regina_union_null_or_string_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_NULL_OR_STRING_H_ */

# ifndef REGINA_UNION_LONG_OR_NULL_H_
# define REGINA_UNION_LONG_OR_NULL_H_

# define REGINA_UNION_LONG_OR_NULL_BRANCH_0    0
# define REGINA_UNION_LONG_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_long_or_null_branch_0_create();
regina_union_t *regina_union_long_or_null_branch_1_create();

regina_union_t *regina_union_long_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_LONG_OR_NULL_H_ */

typedef struct {
    regina_union_t * cmd_ack;
    regina_union_t * notify_ack;
    regina_union_t * config_ack;
    regina_union_t * software_ack;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sync_response_ack_t;

regina_sync_response_ack_t *regina_sync_response_ack_create();
regina_sync_response_ack_t *regina_sync_response_ack_deserialize(avro_reader_t reader);

typedef struct {
    regina_bytes_t * profile_body;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_profile_sync_request_t;

regina_profile_sync_request_t *regina_profile_sync_request_create();

typedef struct {
    regina_response_result_t result;

    destroy_fn   destroy;
} regina_profile_sync_response_t;

regina_profile_sync_response_t *regina_profile_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_notify_sync_request_t;

regina_notify_sync_request_t *regina_notify_sync_request_create();

typedef struct {
    regina_string_t * uuid;
    regina_bytes_t * cmd_body;

    destroy_fn   destroy;
} regina_cmd_sync_response_t;

regina_cmd_sync_response_t *regina_cmd_sync_response_deserialize(avro_reader_t reader);

# ifndef REGINA_UNION_STRING_OR_NULL_H_
# define REGINA_UNION_STRING_OR_NULL_H_

# define REGINA_UNION_STRING_OR_NULL_BRANCH_0    0
# define REGINA_UNION_STRING_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_string_or_null_branch_0_create();
regina_union_t *regina_union_string_or_null_branch_1_create();

regina_union_t *regina_union_string_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_STRING_OR_NULL_H_ */

# ifndef REGINA_UNION_BYTES_OR_NULL_H_
# define REGINA_UNION_BYTES_OR_NULL_H_

# define REGINA_UNION_BYTES_OR_NULL_BRANCH_0    0
# define REGINA_UNION_BYTES_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_bytes_or_null_branch_0_create();
regina_union_t *regina_union_bytes_or_null_branch_1_create();

regina_union_t *regina_union_bytes_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_BYTES_OR_NULL_H_ */

typedef struct {
    int64_t seq_num;
    regina_union_t * uuid;
    regina_union_t * notification_body;

    destroy_fn   destroy;
} regina_notify_sync_response_t;

regina_notify_sync_response_t *regina_notify_sync_response_deserialize(avro_reader_t reader);


typedef enum {
    ENUM_EVENT_LEVEL_RESERVED,
    ENUM_EVENT_LEVEL_ERROR,
    ENUM_EVENT_LEVEL_WARNING,
    ENUM_EVENT_LEVEL_INFOMATION,
    ENUM_EVENT_LEVEL_DEBUG,
    ENUM_EVENT_LEVEL_USER_DEFINITION_TYPE
} regina_event_level_t;

#ifdef GENC_ENUM_DEFINE_ALIASES
#define RESERVED ENUM_EVENT_LEVEL_RESERVED
#define ERROR ENUM_EVENT_LEVEL_ERROR
#define WARNING ENUM_EVENT_LEVEL_WARNING
#define INFOMATION ENUM_EVENT_LEVEL_INFOMATION
#define DEBUG ENUM_EVENT_LEVEL_DEBUG
#define USER_DEFINITION_TYPE ENUM_EVENT_LEVEL_USER_DEFINITION_TYPE
# endif /* GENC_ENUM_DEFINE_ALIASES */

#ifdef GENC_ENUM_STRING_LITERALS
const char* REGINA_EVENT_LEVEL_SYMBOLS[6] = {
    "RESERVED",
    "ERROR",
    "WARNING",
    "INFOMATION",
    "DEBUG",
    "USER_DEFINITION_TYPE"};
# endif /* GENC_ENUM_STRING_LITERALS */

typedef struct {
    int64_t seq_num;
    int64_t at;
    regina_event_level_t level;
    regina_bytes_t * body;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_event_sync_request_t;

regina_event_sync_request_t *regina_event_sync_request_create();

typedef struct {
    int64_t seq_num;
    regina_response_result_t result;

    destroy_fn   destroy;
} regina_event_sync_response_t;

regina_event_sync_response_t *regina_event_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t at;
    regina_bytes_t * data;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sensor_data_entry_t;

regina_sensor_data_entry_t *regina_sensor_data_entry_create();

typedef struct {
    int64_t seq_num;
    regina_list_t * sensor_data_entries;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sensor_data_sync_request_t;

regina_sensor_data_sync_request_t *regina_sensor_data_sync_request_create();

typedef struct {
    int64_t seq_num;
    regina_list_t * sensor_data_delivery_status;

    destroy_fn   destroy;
} regina_sensor_data_sync_response_t;

regina_sensor_data_sync_response_t *regina_sensor_data_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;
    regina_bytes_t * md5;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_config_sync_request_t;

regina_config_sync_request_t *regina_config_sync_request_create();

typedef struct {
    int64_t seq_num;
    regina_union_t * config_body;

    destroy_fn   destroy;
} regina_config_sync_response_t;

regina_config_sync_response_t *regina_config_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_soft_sync_request_t;

regina_soft_sync_request_t *regina_soft_sync_request_create();

typedef struct {
    regina_string_t * module_name;
    regina_string_t * url;
    regina_string_t * version;
    regina_bytes_t * md5;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_software_update_info_t;

regina_software_update_info_t *regina_software_update_info_create();
regina_software_update_info_t *regina_software_update_info_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * module_name;
    regina_string_t * version;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_software_info_t;

regina_software_info_t *regina_software_info_create();
regina_software_info_t *regina_software_info_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;
    regina_list_t * software_update_info;

    destroy_fn   destroy;
} regina_soft_sync_response_t;

regina_soft_sync_response_t *regina_soft_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * uuid;
    regina_bytes_t * resp_nf_body;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_cmd_confirm_t;

regina_cmd_confirm_t *regina_cmd_confirm_create();

typedef struct {
    regina_string_t * uuid;
    regina_bytes_t * resp_nf_body;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_notify_confirm_t;

regina_notify_confirm_t *regina_notify_confirm_create();

typedef struct {
    regina_string_t * cmd_confirm_ack;
    regina_response_result_t result;

    destroy_fn   destroy;
} regina_cmd_confirm_ack_t;

regina_cmd_confirm_ack_t *regina_cmd_confirm_ack_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * notify_confirm_ack;
    regina_response_result_t result;

    destroy_fn   destroy;
} regina_notify_confirm_ack_t;

regina_notify_confirm_ack_t *regina_notify_confirm_ack_deserialize(avro_reader_t reader);

typedef struct {
    regina_string_t * regiter_code;
    regina_string_t * sdk_uuid;
    regina_list_t * software_info;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_register_sync_request_t;

regina_register_sync_request_t *regina_register_sync_request_create();
regina_register_sync_request_t *regina_register_sync_request_deserialize(avro_reader_t reader);

typedef struct {
    regina_response_result_t result;
    regina_union_t * device_id;
    regina_union_t * auth_code;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_register_sync_response_t;

regina_register_sync_response_t *regina_register_sync_response_create();
regina_register_sync_response_t *regina_register_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t device_id;
    regina_string_t * sdk_uuid;
    regina_string_t * auth_code;
    regina_list_t * software_info;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_authen_sync_request_t;

regina_authen_sync_request_t *regina_authen_sync_request_create();
regina_authen_sync_request_t *regina_authen_sync_request_deserialize(avro_reader_t reader);

typedef struct {
    regina_response_result_t result;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_authen_sync_response_t;

regina_authen_sync_response_t *regina_authen_sync_response_create();
regina_authen_sync_response_t *regina_authen_sync_response_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;
    int32_t qos;
    regina_bytes_t * data;
    regina_union_t * ext_data;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_thirdparty_service_request_t;

regina_thirdparty_service_request_t *regina_thirdparty_service_request_create();
regina_thirdparty_service_request_t *regina_thirdparty_service_request_deserialize(avro_reader_t reader);

typedef struct {
    int64_t seq_num;
    regina_response_result_t result;
    regina_union_t * data;
    regina_union_t * ext_data;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_thirdparty_service_response_t;

regina_thirdparty_service_response_t *regina_thirdparty_service_response_create();
regina_thirdparty_service_response_t *regina_thirdparty_service_response_deserialize(avro_reader_t reader);

# ifndef REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_register_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_register_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_REGISTER_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_authen_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_authen_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_AUTHEN_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_profile_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_profile_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_PROFILE_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_notify_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_notify_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_NOTIFY_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_event_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_event_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_EVENT_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_sensor_data_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_sensor_data_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_SENSOR_DATA_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_config_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_config_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_CONFIG_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_H_
# define REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_H_

# define REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_soft_sync_request_or_null_branch_0_create();
regina_union_t *regina_union_soft_sync_request_or_null_branch_1_create();

# endif /* REGINA_UNION_SOFT_SYNC_REQUEST_OR_NULL_H_ */

# ifndef REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_H_
# define REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_H_

# define REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_0    0
# define REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_sync_response_ack_or_null_branch_0_create();
regina_union_t *regina_union_sync_response_ack_or_null_branch_1_create();

# endif /* REGINA_UNION_SYNC_RESPONSE_ACK_OR_NULL_H_ */

# ifndef REGINA_UNION_CMD_CONFIRM_OR_NULL_H_
# define REGINA_UNION_CMD_CONFIRM_OR_NULL_H_

# define REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_0    0
# define REGINA_UNION_CMD_CONFIRM_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_cmd_confirm_or_null_branch_0_create();
regina_union_t *regina_union_cmd_confirm_or_null_branch_1_create();

# endif /* REGINA_UNION_CMD_CONFIRM_OR_NULL_H_ */

# ifndef REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_H_
# define REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_H_

# define REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_0    0
# define REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_notify_confirm_or_null_branch_0_create();
regina_union_t *regina_union_notify_confirm_or_null_branch_1_create();

# endif /* REGINA_UNION_NOTIFY_CONFIRM_OR_NULL_H_ */

# ifndef REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_H_
# define REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_H_

# define REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_0    0
# define REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_thirdparty_service_request_or_null_branch_0_create();
regina_union_t *regina_union_thirdparty_service_request_or_null_branch_1_create();

# endif /* REGINA_UNION_THIRDPARTY_SERVICE_REQUEST_OR_NULL_H_ */

typedef struct {
    regina_union_t * register_sync_request;
    regina_union_t * authen_sync_request;
    regina_union_t * profile_sync_request;
    regina_union_t * notify_sync_request;
    regina_union_t * event_sync_request;
    regina_union_t * sensor_data_sync_request;
    regina_union_t * config_sync_request;
    regina_union_t * soft_sync_request;
    regina_union_t * sync_response_ack;
    regina_union_t * cmd_confirm;
    regina_union_t * notify_confirm;
    regina_union_t * thirdparty_service_request;

    serialize_fn serialize;
    get_size_fn  get_size;
    destroy_fn   destroy;
} regina_sync_request_t;

regina_sync_request_t *regina_sync_request_create();

# ifndef REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_register_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_REGISTER_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_authen_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_AUTHEN_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_profile_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_PROFILE_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_cmd_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_CMD_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_notify_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_NOTIFY_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_event_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_EVENT_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_sensor_data_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_SENSOR_DATA_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_config_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_CONFIG_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_H_
# define REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_H_

# define REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_soft_sync_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_SOFT_SYNC_RESPONSE_OR_NULL_H_ */

# ifndef REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_H_
# define REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_H_

# define REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_BRANCH_0    0
# define REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_cmd_confirm_ack_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_CMD_CONFIRM_ACK_OR_NULL_H_ */

# ifndef REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_H_
# define REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_H_

# define REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_BRANCH_0    0
# define REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_notify_confirm_ack_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_NOTIFY_CONFIRM_ACK_OR_NULL_H_ */

# ifndef REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_H_
# define REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_H_

# define REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_BRANCH_0    0
# define REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_BRANCH_1    1

regina_union_t *regina_union_thirdparty_service_response_or_null_deserialize(avro_reader_t reader);

# endif /* REGINA_UNION_THIRDPARTY_SERVICE_RESPONSE_OR_NULL_H_ */

typedef struct {
    regina_union_t * register_sync_response;
    regina_union_t * authen_sync_response;
    regina_union_t * profile_sync_response;
    regina_union_t * cmd_sync_response;
    regina_union_t * notify_sync_response;
    regina_union_t * event_sync_response;
    regina_union_t * sensor_data_sync_response;
    regina_union_t * config_sync_response;
    regina_union_t * soft_sync_response;
    regina_union_t * cmd_confirm_ack;
    regina_union_t * notify_confirm_ack;
    regina_union_t * thirdparty_service_response;

    destroy_fn   destroy;
} regina_sync_response_t;

regina_sync_response_t *regina_sync_response_deserialize(avro_reader_t reader);

#ifdef __cplusplus
}      /* extern "C" */
#endif
#endif
#ifndef __REGINA_DEVICE_H__
#define __REGINA_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"


#define REGINA_MAJOR_VERSION        1
#define REGINA_MINOR_VERSION        0
#define REGINA_SUBMINOR_VERSION     0




typedef struct regina_device_t regina_device_t;

typedef enum {
    REGINA_EVENT_LEVEL_RESERVED,
    REGINA_EVENT_LEVEL_ERROR,
    REGINA_EVENT_LEVEL_WARNING,
    REGINA_EVENT_LEVEL_INFOMATION,
    REGINA_EVENT_LEVEL_DEBUG,
    REGINA_EVENT_LEVEL_USER_DEFINITION_TYPE
} regina_enum_event_level_t;

typedef struct regina_module_update_info_t {
    char *name;
    char *url;
    char *version;
    char md5[16];
} regina_module_update_info_t;

typedef struct regina_module_info_t {
    char *name;
    char *version;
} regina_module_info_t;

typedef struct regina_sensor_data_t {
    char *data;
    size_t len;
    int64_t timestamp; /**< timestamp of the sensor data, if 0,
                            the platform uses the default timestamp. */
} regina_sensor_data_t;

typedef struct regina_event_t {
    char *event;
    size_t len;
    regina_enum_event_level_t level;
    int64_t timestamp; /**< timestamp of the sensor data, if 0,
                            the platform uses the default timestamp. */
} regina_event_t;

typedef struct regina_notification_id_t {
    char *uuid;
    int32_t internal;
} regina_notification_id_t;

typedef struct regina_uplink_data_t {
    regina_sensor_data_t *sensor_data; /**< Points to the sensor datas to send. */
    size_t sd_count; /**< The count of the sensor datas. */

    regina_event_t event;

    regina_notification_id_t ntf_id;
    char *ntf_confirmation;
    size_t cf_len;

    int check_software_update;
} regina_uplink_data_t;

typedef enum {
    REGINA_ACK_SUCCESS,
    REGINA_ACK_INTERNAL_ERROR,
    REGINA_ACK_INVALID_DATA,
    REGINA_ACK_FREQ_LIMIT,
    REGINA_ACK_COUNT_LIMIT,
    REGINA_ACK_DATA_SIZE_LIMIT,
    REGINA_ACK_UNSUPPORTED
} regina_enum_ack_t;

typedef struct regina_uplink_data_ack_t {
    regina_enum_ack_t *sensor_data_ack; /**< Points to user defined memory, and must
                                             have @see regina_uplink_data_t.sd_count
                                             entries at least. */
    regina_enum_ack_t event_ack;
    regina_enum_ack_t nt_confirmation_ack;
} regina_uplink_data_ack_t;

/**
 * The quality level of the  request thirdparty service.
 */
typedef enum {
    REGINA_THIRDPARTY_SERVICE_QOS0, /**< Just send the request to the server. */
    REGINA_THIRDPARTY_SERVICE_QOS1  /**< Send the request to the server and receive
                                         the response of the request. */
} regina_thirdparty_service_qos_t;


typedef void (*notification_callback)(void *context, const regina_notification_id_t *id,
                                      const char *notification, size_t size);

typedef void (*software_update_callback)(void *context, const regina_module_update_info_t *modules, int count);
/**
 * @param handle_configuration return 0 the configuration will be saved, and can be retrieved by
 *   @see regina_device_get_configuration.
 */
typedef int (*configuration_callback)(void *context, const char *configuration, size_t size);

/**
 * Open the device.
 *
 * @param conf the configuration of the device, call regina_conf_get() to get it.
 * @param size the size of the concifuration, use the REGINA_CONF_SIZE constant.
 * @param register_code
 * @param profile points to the profile of the device.
 * @param prof_len the length of the profile.
 * @param modules points to the array contains the version information of all modules.
 * @param module_count the count of the modules.
 * @param data points the data to send to the server, may be null.
 * @param ack points to the object stores the ack of the data, may be null.
 * @return REGINA_ERR_NONE if succeeds.
 * @remark call @see regina_device_close() to close the device when no use.
 */
REGINA_API int regina_device_open(const char *conf, size_t size,
                                  const char *register_code,
                                  const char *profile,
                                  size_t prof_len,
                                  const regina_module_info_t *modules,
                                  size_t module_count,
                                  const regina_uplink_data_t *data,
                                  regina_uplink_data_ack_t *ack);

/**
 * Check the connection and keep it alive.
 *
 * @return REGINA_ERR_NONE if succeeds.
 */
REGINA_API int regina_device_check_and_keepalive();
/**
 * Send the data to the server.
 *
 * @param data points the data to send to the server, may be null.
 * @ack points to the object stores the ack of the data, maybe null.
 * @return REGINA_ERR_NONE if succeeds.
 */
REGINA_API int regina_device_send(const regina_uplink_data_t *data,
                                  regina_uplink_data_ack_t *ack);
/**
 * Handle the notification, configuration or software update information.
 *
 * @param context points to the context used by the callback functions.
 * @param nt_callback handle notification.
 * @param cf_callback handle configuration.
 * @param su_callback handle software update.
 * @return REGINA_ERR_NONE if succeeds.
 */
REGINA_API int regina_device_handle_downlink_data(void *context,
                                                  notification_callback nt_callback,
                                                  configuration_callback cf_callback,
                                                  software_update_callback su_callback);
/**
 * Request the thirdparty service.
 *
 * @param qos
 * @param request points to the request of the thirdparty service.
 * @param req_size the bytes of the thirdparty service.
 * @param ack [OUT] the acknowledgement of the request.
 * @param resonse [OUT] stores the pointer points to the response of the request,
 *        if qos is @see REGINA_THIRDPART_SERVICE_QOS1, call regina_free() to destroy
 *        it when no use.
 * @param resp_size [OUT] stores the bytes of the response.
 */
REGINA_API int regina_device_request_thirdparty_service(regina_thirdparty_service_qos_t qos,
                                                        const char *request, size_t req_size,
                                                        regina_enum_ack_t *ack,
                                                        char **response, size_t *resp_size);

/**
 * Get the id and access token of the device
 *
 * @param dev_id points a buffer to store the device id, whose maximum length is 20
 * @param access_token points to store the access token whose maximum length is 8.
 * @return REGINA_ERR_NONE if succeeds.
 */
REGINA_API int regina_device_get_device_id(char *dev_id, char *access_token);

/**
 * Destroy the device.
 */
REGINA_API void regina_device_close();
/**
 * Get the confiuration. It is thread-safety.
 *
 * @remark when conf is no use, call regina_free to free the
 *  memory pointed by *conf. Do not call this interface in the
 *  @see service_listener_t.
 */
REGINA_API int regina_device_get_configuration(char **conf, size_t *size);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_DEVICE_H__ */

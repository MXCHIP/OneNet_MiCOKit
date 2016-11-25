#include "regina/device.h"
#include "regina/allocator.h"
#include "std_wrapper.h"
#include "common.h"
#include "util.h"
#include "regina_config.h"
#include "channel.h"
#include "bootstrap.h"
#include "acceptor.h"
#include "device_status.h"
#include "security.h"
#include "environment.h"
#include "thread.h"

#define DEVICE_STATUS_FILE  "regina_device"

struct regina_device_t {
    bootstrap_t bootstrap[1];
    acceptor_t *acceptor;
    channel_t *channel;
};

static regina_device_t *sgl_device = NULL;

static int regina_device_do_init(regina_device_t *device,
                                 const char *conf, size_t size,
                                 const char *reg_code,
                                 device_mode_t mode)
{
    int err;

    err = regina_config_init(conf, size);
    if (REGINA_ERR_NONE != err) {
        REGINA_LOG0(ERROR, "Failed to parse the configuration of SDK.");
        return err;
    }

    err = device_status_load(DEVICE_STATUS_FILE, mode, reg_code);
    if (REGINA_ERR_NONE != err) {
        REGINA_LOG1(ERROR, "Failed to load the device status, error=%d.", err);
        return err;
    }


    err = channel_create(&device->channel);
    if (REGINA_ERR_NONE != err) {
        device->channel = NULL;
        return err;
    }

    bootstrap_init(device->bootstrap, device->channel);

    err = acceptor_create(&device->acceptor, device->channel);
    if (REGINA_ERR_NONE != err) {
        device->acceptor = NULL;
        return err;
    }

    return REGINA_ERR_NONE;
}


static int regina_device_common_init(const char *conf, size_t size,
                                     const char *reg_code,
                                     device_mode_t mode)
{
    int err;

    if (sgl_device)
        return REGINA_ERR_MULTI_INITTIALIZED;

    if (0 != initialize_environment()) {
        REGINA_LOG0(ERROR, "Failed to intialize the system environment.");
        return REGINA_ERR_SYSTEM_ERROR;
    }

    sgl_device = (regina_device_t*)regina_malloc(sizeof(regina_device_t));
    if (!sgl_device)
        return REGINA_ERR_NOMEM;
    regina_memset(sgl_device, 0, sizeof(regina_device_t));

    err = regina_device_do_init(sgl_device, conf, size, reg_code, mode);
    if (REGINA_ERR_NONE != err)
        regina_device_close();

    return err;
}

static int regina_device_connect(const char *profile,
                                 size_t prof_len,
                                 const regina_module_info_t *modules,
                                 size_t module_count,
                                 const regina_uplink_data_t *data,
                                 regina_uplink_data_ack_t *ack)
{
    int err;
    regina_device_t *dev = sgl_device;

    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);

    regina_config_goto_first_bootstrap();
    while (1) {
        access_point_t *ap = NULL;
        err = bootstrap_get_next_acceptor(dev->bootstrap, &ap);
        if (REGINA_ERR_NONE != err) {
            err = bootstrap_retreive_acceptor(dev->bootstrap);
            if (REGINA_ERR_NONE != err) {
                REGINA_LOG_DEBUG0("Failed to retreive the acceptor.");
                break;
            }

            continue;
        }

        err = acceptor_authenticate(dev->acceptor, ap, profile, prof_len,
                                    modules, module_count, data, ack);

        if ((err <= REGINA_ERR_SOCKET_ERROR) || (REGINA_ERR_TIMEOUT == err))
            continue;

        break;
    }

    return err;
}

/****************************** API IMPLEMENTATION *********************************/

REGINA_API int regina_device_open(const char *conf, size_t size,
                                  const char *register_code,
                                  const char *profile,
                                  size_t prof_len,
                                  const regina_module_info_t *modules,
                                  size_t module_count,
                                  const regina_uplink_data_t *data,
                                  regina_uplink_data_ack_t *ack)
{
    int err;
    device_mode_t mode = DEVICE_MODE_NA;

    REGINA_RETURN_IF_NIL3(conf, size, register_code, REGINA_ERR_BADPARAM);

    if (regina_strlen(register_code) > 2) {
        if (('S' == register_code[0]) && ('E' == register_code[1])) {
            mode = DEVICE_MODE_DEBUG;
            REGINA_LOG_DEBUG0("Device opened on [DEBUG] mode.");
        }
        else if (('N' == register_code[0]) && ('E' == register_code[1])) {
            mode = DEVICE_MODE_NORMAL;
            REGINA_LOG_DEBUG0("Device opened on [NORMAL] mode.");
        }
    }

    if (DEVICE_MODE_NA == mode)
        return REGINA_ERR_BADPARAM;

    err = security_crypto_init();
    REGINA_RETURN_IF_ERR(err);

    err = regina_device_common_init(conf, size, register_code, mode);
    if (REGINA_ERR_NONE == err) {
        err = regina_device_connect(profile, prof_len, modules,
                                    module_count, data, ack);
    }

    return err;
}

REGINA_API void regina_device_close()
{
    if (sgl_device) {
        security_crypto_deinit();
        cleanup_environment();

        regina_config_destroy();
        device_status_destroy();
        bootstrap_destroy(sgl_device->bootstrap);
        acceptor_destroy(sgl_device->acceptor);
        channel_destroy(sgl_device->channel);

        regina_free(sgl_device);

        sgl_device = NULL;
    }
}


/* REGINA_API int regina_device_disconnect() */
/* { */
/*     REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED); */
/*     channel_close(sgl_device->channel); */
/*     return REGINA_ERR_NONE; */
/* } */

REGINA_API int regina_device_check_and_keepalive()
{
    int err;

    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);
    err = acceptor_check_and_keepalive(sgl_device->acceptor);
    return err;
}

REGINA_API int regina_device_handle_downlink_data(void *context,
                                                  notification_callback nt_callback,
                                                  configuration_callback cf_callback,
                                                  software_update_callback su_callback)
{
    int err;

    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);
    err = acceptor_handle_downlink_data(sgl_device->acceptor, context,
                                        nt_callback, cf_callback, su_callback);
    return err;
}

REGINA_API int regina_device_send(const regina_uplink_data_t *data,
                                  regina_uplink_data_ack_t *ack)
{
    int err;
    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);
    err = acceptor_send(sgl_device->acceptor, data, ack);
    return err;
}

REGINA_API int regina_device_get_configuration(char **conf, size_t *size)
{
    int err;
    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);

    err = acceptor_get_configuration(sgl_device->acceptor, conf, size);
    if (REGINA_ERR_NONE != err) {
        regina_free(*conf);
        *conf = NULL;
        *size = 0;
    }
    return err;
}

REGINA_API int regina_device_request_thirdparty_service(regina_thirdparty_service_qos_t qos,
                                                        const char *request, size_t req_size,
                                                        regina_enum_ack_t *ack,
                                                        char **response, size_t *resp_size)
{
    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);

    return acceptor_request_thirdparty_service(sgl_device->acceptor, qos, request, req_size,
                                               ack, response, resp_size);
}

REGINA_API int regina_device_get_device_id(char *dev_id, char *access_token)
{
    const char *auth_code;
    REGINA_RETURN_IF_NIL(sgl_device, REGINA_ERR_UNINITIALIZED);

    regina_snprintf(dev_id, 21, "%ld", device_status_get_device_id());

    regina_memset(access_token, 0, 9);
    auth_code = device_status_get_auth_code();
    if (auth_code) {
        size_t auth_code_len = regina_strlen(auth_code);

        regina_memcpy(access_token, auth_code, (auth_code_len >= 8 ? 8 : auth_code_len));
    }

    return REGINA_ERR_NONE;
}


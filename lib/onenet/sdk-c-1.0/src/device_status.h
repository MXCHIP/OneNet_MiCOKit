#ifndef __REGINA_DEVICE_STATUS_H__
#define __REGINA_DEVICE_STATUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "std_wrapper.h"

typedef enum {
    DEVICE_MODE_NORMAL,
    DEVICE_MODE_DEBUG,

    DEVICE_MODE_NA
} device_mode_t;


int device_status_load(const char *path, device_mode_t mode, const char *reg_code);
device_mode_t device_status_get_mode();
int device_status_is_new();
const char *device_status_get_reg_code();
int device_status_set_auth_code(const char *auth_code);
const char *device_status_get_auth_code();
int device_status_set_device_id(int64_t id);
int64_t device_status_get_device_id();
void device_status_set_notification_sn(int64_t sn);
const char *device_status_get_profile_md5();
void device_status_set_profile_md5(const char *md5);
int64_t device_status_get_notification_sn();
void device_status_set_conf_md5(const char *md5);
const char *device_status_get_conf_md5();

int device_status_save();
void device_status_destroy();

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_DEVICE_STATUS_H__ */

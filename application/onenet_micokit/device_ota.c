#include "onenet.h"
#include "mico_app_define.h"
#include "ota_server.h"
#include "debug.h"
#include "device_sensor.h"

#define device_ota_log(format, ...)  custom_log("device", format, ##__VA_ARGS__)

static bool version_compara( char *new_ver )
{
    char version[30];
    int version_len = 0;
    int compara_len = 0;
    int old_num = 0;
    int new_num = 0;

    compara_len = strlen(APP_VERSION);
    version_len = get_firmware_version(version);
    new_num = atoi(new_ver + version_len-compara_len);
    old_num = atoi(APP_VERSION);

    if ( (0 == memcmp( version, new_ver, version_len - compara_len - 1 )) && (new_num > old_num) )
    {
        return true;
    }
    return false;
}

static void ota_server_status_handler(OTA_STATE_E state, float progress)
{
    switch ( state )
    {
        case OTA_LOADING:
            device_ota_log("ota server is loading, progress %.2f%%", progress);
            break;
        case OTA_SUCCE:
            device_ota_log("ota server daemons success");
            break;
        case OTA_FAIL:
            device_ota_log("ota server daemons failed");
            break;
        default:
            break;
    }
}

void onenet_ota_server_callback( char *version, char *md5, char *url )
{
    device_ota_log("update firmare version: %s", version);
    if( version_compara(version) == true){
        ota_server_start(url, md5, ota_server_status_handler);
    }
}


#ifndef __DEVICE_SENSOR_H
#define __DEVICE_SENSOR_H

#include "common.h"

#pragma pack(1)
typedef struct _sensor_data_t{
    uint8_t temperature;
    uint8_t humidity;
    uint16_t light_sensor;
    uint16_t infrared;
} sensor_data_t;

typedef struct _sensor_state_t{
    bool dc_motor;
    float hues;
    float saturation;
    float brightness;
    bool lamp_switch;
}sensor_state_t;

typedef struct _event_t{
    bool is_sensor;
} event_t;

int get_firmware_version(char *version);

#endif

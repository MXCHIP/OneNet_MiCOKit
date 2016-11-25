/**
 ******************************************************************************
 * @file    onenet.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    24-Nov-2016
 * @brief   This file provides all the headers of OneNet operation provided by MICO.
 ******************************************************************************
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */
#ifndef __ONENET_H_
#define __ONENET_H_

#include "mico.h"
#include "mico_app_define.h"
#include "regina_profile.h"
#include "regina_sensor_data.h"
#include "regina_configuration.h"
#include "regina_event.h"
#include "regina_notification.h"

#ifndef PRODUCT_REGISTER_CODE
#error "must define PRODUCT_REGISTER_CODE at application"
#endif

/**
  * @brief  OneNet SDK version
  */
#define ONENET_VER_MAJOR    (1)
#define ONENET_VER_MINOR    (0)
#define ONENET_VER_REVISION (0)

/**
  * @brief  error code
  */
#define ONENET_QUEUE_FULL    -500

/**
  * @brief  message types
  */
enum onenet_msg_type_e{
    ONENET_EVENT, //!< ONENET_EVENT
    ONENET_SENSOR,//!< ONENET_SENSOR
};

typedef uint8_t onenet_msg_type_e;

/**
  * @brief  OneNet configuration should write to FLASH
  */
typedef struct _onenet_config_t{
    uint8_t              onenet_file[1024];    /** OneNet SDK save **/
    uint8_t              device_id[21];        /** device id **/
    uint8_t              access_token[9];      /** device access token **/
} onenet_config_t;

/**
  * @brief  upload message struct
  */
typedef struct _onenet_upload_msg_t{
    void                 *sensor_buf;          /** sensor data buffer **/
    int                  sensor_data_len;      /** sensor data buffer size **/
    void                 *event_buf;           /** event data buffer **/
    int                  event_data_len;       /** event data buffer sizes **/
} onenet_upload_msg_t;

/**
  * @brief  OneNet run status context
  */
typedef struct _onenet_status_t{
    onenet_upload_msg_t  msg;                  /** see  onenet_upload_msg_t **/
    bool                 onenet_connect;       /** OneNet cloud link state **/
    bool                 upload_state;         /** message upload state **/
} onenet_status_t;

/**
  * @brief  OneNet global context
  */
typedef struct _onenet_context_t{
    onenet_config_t      *onenet_config;       /** see onenet_config_t struct **/
    onenet_status_t      onenet_status;        /** see onenet_status_t struct **/
} onenet_context_t;

/** @brief   Initialize OneNet context
  *          Must be called before mico_system_context_init function
  *
  * @param   sensor_buf_size : set sensor data buffer size, must be greater than 0.
  * @param   event_buf_size  : set event buffer size, must be greater than or equal to 0.
  *                            if there is no event data, the value can be 0.
  *
  * @return  kNoErr          : on success.
  * @return  kGeneralErr     : if an error occurred
  */
OSStatus onenet_context_init( int sensor_buf_size, int event_buf_size );


/** @brief   Get the address of the OneNet context data.
  *          Depend on the function onenet_context_init.
  *
  * @param   NO
  *
  * @return  onenet_context_t   : Address of OneNet context data.
  */
onenet_context_t *onenet_server_context_get( void );

/** @brief   Save OneNet parameters to FLASH, Depend on the function onenet_context_init
  *
  * @param   NO
  *
  * @return  NO
  */
void onenet_server_para_storage( void );

/** @brief   Default OneNet parameters to FLASH, Depend on the function onenet_context_init
  *
  * @param   NO
  *
  * @return  NO
  */
void onenet_server_para_default( void );

/** @brief   Start OneNet Cloud service
  *
  * @param   NO
  *
  * @return  kNoErr        : on success
  * @return  kGeneralErr   : if an error occurred
  */
OSStatus onenet_server_start( void );

/** @brief   Connection status callback
  *
  * @param   state         : true is connected, false is disconnected
  *
  * @return  NO
  */
void onenet_connect_server_state_callback( bool state );

/** @brief   callback function if there is a OTA firmware
  *
  * @param   version       : firmware version
  * @param   md5           : bin file md5 check value
  * @param   url           : bin file download address
  *
  * @return  NO
  */
void onenet_ota_server_callback( char *version, char *md5, char *url );

/** @brief   callback function Get the profile
  *
  * @param   ieme          : profile data, must MAC address
  * @param   profile       : the profile of the device
  * @param   size          : the length of the profile
  *
  * @return  NO
  */
void onenet_profile_callback( const char *ieme, char **profile, size_t *size );

/** @brief   callback function Set the configuration
  *
  * @param   configuration : the configuration of the device
  * @param   size          : the length of the configuration
  *
  * @return  true          : on success.
  * @return  false         : if an error occurred
  */
int onenet_configuration_callback( const char *configuration, size_t size );

/** @brief   callback function if there is a notification data
  *
  * @param   notification_conf : the notice configuration of the device
  * @param   notification      : the notification of the device
  * @param   reader            : see avro_reader_t struct
  * @param   len               : the length of the notice configuration
  *
  * @return  NO
  */
void onenet_notification_callback( void **notification_conf, void **notification, avro_reader_t reader, int *len );

/** @brief   callback function depend onenet_notification_callback, destroy some memory
  *
  * @param   writer            : writer notice configuration to OneNet SDK
  * @param   notification_conf : the notice configuration of the device base on onenet_notification_callback
  * @param   notification      : the notification of the device base on onenet_notification_callback
  *
  * @return  NO
  */
void onenet_notification_destroy( void *writer, void *notification_conf, void *notification );

/** @brief   callback function if used onenet_message_send,  message type is ONENET_EVENT
  *
  * @param   event         : the event data of the OneNet cloud
  * @param   len           : the event data length of the OneNet cloud
  * @param   data          : the event data of used onenet_message_send function
  * @param   datalen       : the event data length of used onenet_message_send function
  *
  * @return  NO
  */
void onenet_event_callback( void **event, int *len, uint8_t *data, uint32_t datalen );

/** @brief   callback function depend onenet_event_callback, destroy some memory
  *
  * @param   writer        : see avro_writer_t struct
  * @param   event         : the event data of the device base on onenet_event_callback
  *
  * @return  NO
  */
void onenet_event_destroy( avro_writer_t writer, void *event );

/** @brief   callback function if used onenet_message_send, message type is ONENET_SENSOR
  *
  * @param   sensor        : the sensor data of the OneNet cloud
  * @param   len           : the sensor data length of the OneNet cloud
  * @param   data          : the sensor data of used onenet_message_send function
  * @param   datalen       : the sensor data length of used onenet_message_send function
  *
  * @return  NO
  */
void onenet_sensor_data_callback( void **sensor, int *len, uint8_t *data, uint32_t datalen );

/** @brief   callback function depend onenet_sensor_data_callback, destroy some memory
  *
  * @param   writer        : see avro_writer_t struct
  * @param   sensor        : the sensor data of the device base on onenet_sensor_data_callback
  *
  * @return  NO
  */
void onenet_seneor_data_destroy( avro_writer_t writer, void *sensor );

/** @brief   send a message to OneNet Cloud
  *
  * @param   msg_type      : message type, event or sensor
  * @param   buf           : data buffer
  * @param   len           : data length
  * @param   time_out      : time out
  *
  * @return  kNoErr        : on success
  * @return  kGeneralErr   : if an error occurred
  */
OSStatus onenet_message_send( onenet_msg_type_e msg_type, uint8_t *buf, uint32_t len, uint32_t time_out );

#endif

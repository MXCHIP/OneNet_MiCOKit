/**
 ******************************************************************************
 * @file    mico_main.c
 * @author  QQ ding
 * @version V1.0.0
 * @date    21-May-2015
 * @brief   MiCO application
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2016 MXCHIP Inc.
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
 *
 ******************************************************************************
 */

#include "mico.h"
#include "onenet.h"
#include "app_para_storage.h"
#include "mico_app_define.h"

#define onenet_main_log(format, ...)  custom_log("onenet", format, ##__VA_ARGS__)

extern void device_sensor_int( void );
extern void device_oled_update( void );

/* MICO system callback: Restore default configuration provided by application */
void appRestoreDefault_callback(void * const user_config_data, uint32_t size)
{
    app_context_t *app_context = app_context_get( );

    memset(user_config_data, 0x0, size);
    app_context->device_config->acquisition_frequency = 6;
    onenet_main_log("restore default");
}

int application_start( void )
{
    OSStatus err = kNoErr;
    mico_Context_t *mico_context;

    /* Initialization context of OneNet
     * Must be called before mico_system_context_init function
     */
    err = onenet_context_init( 1024, 10 );
    require_noerr( err, exit );

    /* Initialization context of application
     * Must be called before mico_system_context_init function
     */
    err = app_context_init( );
    require_noerr( err, exit );

    /* Initialization context of MiCO system */
    mico_context = mico_system_context_init( app_para_data_size_get( ) );

    /* Initialization sensor and start sensor data acquisition thread */
    device_sensor_int( );

    /* Start MiCO system functions according to mico_config.h*/
    mico_system_init( mico_context );

    /* Update OLED play */
    device_oled_update( );

    /* Start OneNet service */
    onenet_server_start( );

exit:
  return 0;
}



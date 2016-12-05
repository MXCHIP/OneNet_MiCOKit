/**
 ******************************************************************************
 * @file    mico_app_define.h
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
#pragma once

#include "mico.h"
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  firmware version
  */
#define MANUFACTURER_NAME "MXCHIP"
#define PRODUCT_TYPE "MiCO"
#define PRODUCT_MODEL "Kit"
#define APP_VERSION "2" //must number

/**
  * @brief  product register code, OneNet cloud define
  */
#define PRODUCT_REGISTER_CODE "NEzjiM1G3v6tN8J3"

#define SEND_DATA_SIZE                          1024
#define SEND_DATA_TIME_OUT                      2000

#ifdef DEBUG
#define DEVICE_SENSOR_THREAD_STACK_SIZE         0x800
#else
#define DEVICE_SENSOR_THREAD_STACK_SIZE         0x700
#endif

/**
  * @brief  Device configuration write to FLASH
  */
typedef struct _device_config_t{
    int acquisition_frequency;        /** Sensor data acquisition frequency **/
} device_config_t;

/**
  * @brief  Application global context
  */
typedef struct _app_context_t{
    device_config_t *device_config;   /** see device_config_t struct **/
} app_context_t;

/** @brief   Initialize application context
  *          Must be called before mico_system_context_init function
  *
  * @param   NO
  *
  * @return  kNoErr          : on success
  * @return  kGeneralErr     : if an error occurred
  */
OSStatus app_context_init( void );

/** @brief   Get application context
  *
  * @param   NO
  *
  * @return  app_context_t
  */
app_context_t *app_context_get( void );

/** @brief   Storage application parameter
  *
  * @param   context         : app_context_t
  *
  * @return  NO
  */
void app_para_storage( app_context_t *context );

#ifdef __cplusplus
} /*extern "C" */
#endif

/**
 ******************************************************************************
 * @file    onenet_conf.h
 * @author  QQ Ding
 * @version V1.0.0
 * @date    24-Nov-2016
 * @brief   OneNet operation configuration
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

#include "mico_app_define.h"
#include "mico_config.h"

/**
  * @brief  OneNet debug switch
  */
#define ONENET_DEBUG                 (0)

/**
  * @brief  OneNet discovery switch
  */
#define ONENET_USE_BONJOUR           (1)

/**
  * @brief  OneNet discovery service name
  */
#define ONENET_BONJOUR_SERVICE       "_easylink._tcp.local."

/**
  * @brief  OneNet depth of the sensor queue
  */
#define SENSOR_MESSAGES_NUM            (3)

/**
  * @brief  OneNet depth of the event queue
  */
#define EVENT_MESSAGES_NUM             (3)

/**
  * @brief  OneNet service thread stack size
  */
#define ONENET_THREAD_STACK_SIZE     0x3000


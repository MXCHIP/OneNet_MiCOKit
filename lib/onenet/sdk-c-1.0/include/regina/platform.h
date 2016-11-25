#ifndef __REGINA_PLATFORM_H__
#define __REGINA_PLATFORM_H__


/*
 * all interger types used by sdk list below:
 *   int8_t uint8_t int16_t uint16_t int32_t uint32_t int64_t uint64_t
 */

#if defined(REGINA_PLATFORM_WINDOWS) || defined(REGINA_PLATFORM_UNIX)
#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for interger types */
#elif defined(REGINA_PLATFORM_UCOSII)
typedef long long              int64_t;
typedef unsigned long long     uint64_t;
typedef signed int             int32_t;
typedef unsigned int           uint32_t;
typedef signed short           int16_t;
typedef unsigned short         uint16_t;
typedef signed char            int8_t;
typedef unsigned char          uint8_t;
#elif defined(_OS_THREADX) || defined(REGINA_PLATFORM_SPREADTRUM)
#include "os_api.h"
#include "sci_types.h"
typedef int8         int8_t;
typedef uint8        uint8_t;
typedef int16        int16_t;
typedef uint16       uint16_t;
typedef signed int   int32_t;
typedef unsigned int uint32_t;
typedef int64        int64_t;
typedef uint64       uint64_t;
#define sockaddr sci_sockaddr
#define socklen_t int
#elif defined(REGINA_PLATFORM_STM32_KYLIN)
#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for interger types */
#include <stdlib.h>
#include <string.h>
#elif defined(REGINA_PLATFORM_MICO)
#include <stddef.h>  /* for size_t */
#include <stdint.h>  /* for interger types */
#endif


#if defined(REGINA_PLATFORM_WINDOWS)
#ifdef __BUILD_REGINA__
#define REGINA_API __declspec(dllexport)
#else
#define REGINA_API __declspec(dllimport)
#endif /* #ifdef __BUILD_REGINA__ */
#else
#define REGINA_API
#endif /* REGINA_PLATFORM_WINDOWS */

#endif /* #ifndef __REGINA_PLATFORM_H__ */

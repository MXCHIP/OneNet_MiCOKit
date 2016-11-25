#ifndef __REGINA_UTIL_H__
#define __REGINA_UTIL_H__

#include "regina/log.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define TYPE_CAST(ptr, type, mem)            \
    ((type*)((char*)ptr - (char*)(&(((type*)0)->mem))))

#if defined(REGINA_DEBUG)

#define REGINA_LOG_DEBUG0(format) REGINA_LOG0(DEBUG, format)
#define REGINA_LOG_DEBUG1(format, arg1) REGINA_LOG1(DEBUG, format, arg1)
#define REGINA_LOG_DEBUG2(format, arg1, arg2) REGINA_LOG2(DEBUG, format, arg1, arg2)
#define REGINA_LOG_DEBUG3(format, arg1, arg2, arg3) REGINA_LOG3(DEBUG, format, arg1, arg2, arg3)
#define REGINA_LOG_DEBUG4(format, arg1, arg2, arg3, arg4) REGINA_LOG4(DEBUG, format, arg1, arg2, arg3, arg4)
#define REGINA_LOG_DEBUG5(format, arg1, arg2, arg3, arg4, arg5) REGINA_LOG5(DEBUG, format, arg1, arg2, arg3, arg4, arg5)

#else

#define REGINA_LOG_DEBUG0(format)
#define REGINA_LOG_DEBUG1(format, arg1)
#define REGINA_LOG_DEBUG2(format, arg1, arg2)
#define REGINA_LOG_DEBUG3(format, arg1, arg2, arg3)
#define REGINA_LOG_DEBUG4(format, arg1, arg2, arg3, arg4)
#define REGINA_LOG_DEBUG5(format, arg1, arg2, arg3, arg4, arg5)

#endif /* REGINA_DEBUG */

#define REGINA_RANDOM(x) ((1103515245 * (uint32_t)x + 12345) % (1 << 31))

#endif /* __REGINA_UTIL_H__ */

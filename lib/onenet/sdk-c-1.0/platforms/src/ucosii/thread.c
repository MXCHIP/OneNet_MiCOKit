#include "thread.h"
#if defined(REGINA_PLATFORM_UNIX)
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#endif

#include <errno.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "std_wrapper.h"

#ifndef  OS_MASTER_FILE
#include <ucos_ii.h>
#endif

extern void delay_ms(u16 nms);
int regina_sleep(int milliseconds)
{
    delay_ms(milliseconds);
    return REGINA_ERR_NONE;
}

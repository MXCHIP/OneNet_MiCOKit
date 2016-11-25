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

int regina_sleep(int milliseconds)
{
    vTaskDelay(milliseconds);
    return REGINA_ERR_NONE;
}

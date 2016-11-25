#include "thread.h"
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "std_wrapper.h"

int regina_sleep(int milliseconds)
{
    usleep(milliseconds * 1000);
    return REGINA_ERR_NONE;
}

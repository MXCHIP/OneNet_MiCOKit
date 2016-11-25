#include "mico_rtos.h"

#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "std_wrapper.h"

int regina_sleep(int milliseconds)
{
    mico_thread_msleep(milliseconds);
    return REGINA_ERR_NONE;
}

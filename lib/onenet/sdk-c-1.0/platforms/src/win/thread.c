#include "thread.h"
#include "regina/allocator.h"
#include "regina/error.h"
#include <Windows.h>
#include <process.h>
#include <stdlib.h>

int regina_sleep( int milliseconds )
{
    Sleep( milliseconds );
    return REGINA_ERR_NONE;
}
#include <stdlib.h>
#include "std_wrapper.h"


REGINA_API void *regina_malloc(size_t size)
{
    return malloc(size);
}

REGINA_API void regina_free(void *ptr)
{
    free(ptr);
}

REGINA_API void *regina_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

REGINA_API void *regina_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

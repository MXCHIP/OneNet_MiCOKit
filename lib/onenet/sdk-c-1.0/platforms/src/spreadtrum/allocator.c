#include "std_wrapper.h"

REGINA_API void *regina_malloc(size_t size)
{
    if(size == 0)
    {
        REGINA_LOG0(FATAL, "regina : regina_malloc error size = 0");
        return NULL;
    }

    return malloc(size);
}

REGINA_API void regina_free(void *ptr)
{
    if(ptr)
    {
        free(ptr);
        ptr = NULL;  //ָ�븴NULL
    }
}

REGINA_API void *regina_calloc(size_t nmemb, size_t size)
{
    if(size == 0)
    {
        REGINA_LOG0(FATAL, "regina : regina_calloc error size = 0");
        return NULL;
    }

    return calloc(nmemb, size);
}

REGINA_API void *regina_realloc(void *ptr, size_t size)
{
    if(size == 0)
    {
        REGINA_LOG0(FATAL, "regina : regina_realloc error size = 0");
        return NULL;
    }

    return realloc(ptr, size);
}

#ifndef __REGINA_ALLOCATOR_H__
#define __REGINA_ALLOCATOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

REGINA_API void *regina_malloc(size_t size);
REGINA_API void regina_free(void *ptr);
REGINA_API void *regina_calloc(size_t nmemb, size_t size);
REGINA_API void *regina_realloc(void *ptr, size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __REGINA_ALLOCATOR_H__ */

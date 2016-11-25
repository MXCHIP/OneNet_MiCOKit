#ifndef __REGINA_EXTENT_H__
#define __REGINA_EXTENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "regina/platform.h"


typedef struct regina_extent_t {
    char *base;
    size_t len;
} regina_extent_t;

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_EXTENT_H__ */

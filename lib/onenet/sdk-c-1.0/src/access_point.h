#ifndef __REGINA_ACCESS_POINT_H__
#define __REGINA_ACCESS_POINT_H__

#include "extent.h"
#include "std_wrapper.h"
#include "tcp_util.h"

typedef struct access_point_t {
    int16_t protocol_version;
    ip_address_t *address;
    struct regina_extent_t *key;
    struct regina_extent_t *token;
    int32_t token_expired_time;
} access_point_t;

#endif /* __REGINA_ACCESS_POINT_H__ */

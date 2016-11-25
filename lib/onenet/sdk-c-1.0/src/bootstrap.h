#ifndef __REGINA_BOOTSTRAP_H__
#define __REGINA_BOOTSTRAP_H__

#include "regina/platform.h"
#include "access_point.h"

#define MAX_ACC_POINTS_COUNT 3

struct regina_stream_t;
struct channel_t;

typedef struct bootstrap_t {
    struct channel_t *channel;

    access_point_t bs_ap[MAX_ACC_POINTS_COUNT];
    size_t bs_ap_count;
    size_t bs_ap_cursor;

    access_point_t acc_ap[MAX_ACC_POINTS_COUNT];
    size_t acc_ap_count;
    size_t acc_ap_cursor;
} bootstrap_t;

void bootstrap_init(bootstrap_t *bootstrap, struct channel_t *channel);
void bootstrap_destroy(bootstrap_t *bootstrap);
int bootstrap_retreive_acceptor(bootstrap_t *bootstrap);
int bootstrap_get_next_acceptor(bootstrap_t *bootstrap, access_point_t **ap);

#endif /* __REGINA_BOOTSTRAP_H__ */

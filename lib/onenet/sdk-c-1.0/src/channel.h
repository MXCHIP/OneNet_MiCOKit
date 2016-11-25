#ifndef __REGINA_CHANNEL_H__
#define __REGINA_CHANNEL_H__

#include "access_point.h"

#define TCP_PROTOCOL_VERSION  0x01

typedef struct channel_t channel_t;
struct regina_stream_t;

typedef struct {
    void *context;
    void (*handle_close)(void *context, int err);
    int (*handle_conn_ack)(void *context, const char *payload, size_t size);

    int (*handle_sync)(void *context, const char *payload, size_t size);
} channel_hook_t;

int channel_create(channel_t **channel);
void channel_destroy(channel_t *channel);
int channel_set_access_point(channel_t *channel, access_point_t *ap, channel_hook_t *hook);
int channel_open(channel_t *channel);
int channel_close(channel_t *channel);
int channel_bootstrap_auth(channel_t *channel);
int channel_acceptor_auth(channel_t *channel, struct regina_stream_t *payload);
int channel_acceptor_send(channel_t *channel, struct regina_stream_t *payload);
int channel_poll(channel_t *channel);
int channel_check_and_keepalive(channel_t *channel);

void channel_set_timeout(channel_t *channel, int seconds);


#endif /* __REGINA_CHANNEL_H__ */

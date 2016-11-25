#include "channel.h"
#include "regina/allocator.h"
#include "common.h"
#include "util.h"
#include "extent.h"
#include "stream.h"
#include "regina_tcp_protocol.h"
#include "thread.h"

#define CHANNEL_KEEP_ALIVE  120  /* seconds */
#define INBOUND_BUF_SIZE    1024 /* bytes */

typedef enum {
    CHANNEL_STATUS_CLOSED = 0,
    CHANNEL_STATUS_CLOSING,
    CHANNEL_STATUS_DISCONNECTING,
    CHANNEL_STATUS_OPENING,
    CHANNEL_STATUS_OPENED,
    CHANNEL_STATUS_AUTHENTICATING,
    CHANNEL_STATUS_AUTHENTICATED
} channel_status_t;

struct channel_t {
    volatile channel_status_t status;

    channel_hook_t *hook;
    regina_tcp_pro_t *protocol;
    regina_tcp_pro_handler_t pro_handler;

    uint16_t keep_alive;
    regina_time_t expired_time;
    regina_time_t next_ping_time;
    regina_time_t timeout;

    access_point_t *acc_point;
    socket_fd_t *fd;

    regina_stream_t inbound_buf[1];
    regina_stream_t outbound_buf[1];
};

#define channel_delay_next_ping_time(channel) do { \
        channel->next_ping_time = regina_time(NULL) + channel->keep_alive / 2; \
    } while(0)

#define channel_delay_expired_time(channel) do { \
        channel->expired_time = regina_time(NULL) + channel->keep_alive * 3 / 2; \
    } while(0)


#define channel_transform_status(channel, s)                            \
    do {                                                                \
        channel_t *chnl = (channel);                                    \
        REGINA_LOG_DEBUG2("Transfrom channel status: %d --> %d.", chnl->status, s); \
        chnl->status = s;                                               \
    } while(0)



static void channel_internal_close(channel_t *channel, int reason)
{
    channel_transform_status(channel, CHANNEL_STATUS_CLOSED);
    REGINA_LOG_DEBUG1("Channel closed, error=%d.", reason);

    if (channel->fd) {
        tcp_util_close(channel->fd);
        channel->fd = NULL;
    }

    if (channel->hook && channel->hook->handle_close)
        channel->hook->handle_close(channel->hook->context, reason);
}

static int channel_handle_conn_ack(void *context, regina_tcp_pro_connack_t ack,
                                    const char *payload, size_t size)
{
    channel_t *channel = (channel_t*)context;

    switch(ack) {
    case REGINA_TCP_PRO_CONNACK_PROTOCOL_VERSION_ERROR:
        channel_internal_close(channel, REGINA_ERR_BAD_PROTOCOL_VERSION);
        return REGINA_ERR_BAD_PROTOCOL_VERSION;

    case REGINA_TCP_PRO_CONNACK_SERVER_UNUSABLE:
        channel_internal_close(channel, REGINA_ERR_SERVER_UNUSABLE);
        return REGINA_ERR_SERVER_UNUSABLE;

    case REGINA_TCP_PRO_CONNACK_TOKEN_INVALID:
        channel_internal_close(channel, REGINA_ERR_NOAUTHORITY);
        REGINA_LOG0(ERROR, "Failed to connect to the server for token failure.");
        return REGINA_ERR_NOAUTHORITY;

    case REGINA_TCP_PRO_CONNACK_SUCCESS:
        break;

    default:
        channel_internal_close(channel, REGINA_ERR_PROTOCOL_ERROR);
        return REGINA_ERR_PROTOCOL_ERROR;
    }

    if (channel->hook && channel->hook->handle_conn_ack) {
        int err;
        err = channel->hook->handle_conn_ack(channel->hook->context, payload, size);
        if (REGINA_ERR_NONE != err) {
            channel_internal_close(channel, err);
            return err;
        }
    }

    channel_transform_status(channel, CHANNEL_STATUS_AUTHENTICATED);
    return REGINA_ERR_NONE;
}

static void channel_handle_ping_resp(void *context)
{
    (void)context;
}

static int channel_handle_sync(void *context,
                                const char *payload, size_t size)
{
    channel_t *channel = (channel_t*)context;
    if (channel->hook && channel->hook->handle_sync) {
        int err;
        err = channel->hook->handle_sync(channel->hook->context, payload, size);
        if (REGINA_ERR_NONE != err)
            channel_internal_close(channel, err);
        return err;
    }

    return REGINA_ERR_NONE;
}

static void channel_handle_disconnect(void *context,
                                      regina_tcp_pro_disconn_reason_t reason)
{
    int err;
    channel_t *channel = (channel_t*)context;

    REGINA_LOG1(INFO, "Receive DISCONNECT message, reason=%d.", reason);

    switch(reason) {
    case REGINA_TCP_PRO_DISCONN_NONE:
        err = REGINA_ERR_NONE;
        break;

    case REGINA_TCP_PRO_DISCONN_TIMEOUT:
    case REGINA_TCP_PRO_DISCONN_SERVER_INTERNAL_ERROR:
    case REGINA_TCP_PRO_DISCONN_SERVICE_NOT_AVAILABLE:
        err = REGINA_ERR_SERVER_UNUSABLE;
        break;

    case REGINA_TCP_PRO_DISCONN_DUP_LOGIN:
        err = REGINA_ERR_NOAUTHORITY;
        break;

    case REGINA_TCP_PRO_DISCONN_PROTOCOL_ERROR:
        err = REGINA_ERR_PROTOCOL_ERROR;
        break;

    case REGINA_TCP_PRO_DISCONN_INVALID_DATA:
        err = REGINA_ERR_DATA_CORRUPTION;
        break;


    default:
        err = REGINA_ERR_INTERNAL;
        break;
    }

    channel_internal_close(channel, err);
}

static int channel_init(channel_t *channel)
{
    int err;
    regina_tcp_pro_handler_t *pro_handler = NULL;

    channel->status = CHANNEL_STATUS_CLOSED;
    channel->keep_alive = CHANNEL_KEEP_ALIVE;
    channel->hook = NULL;
    channel->fd = NULL;

    regina_stream_init(channel->inbound_buf);
    regina_stream_init(channel->outbound_buf);

    pro_handler = &channel->pro_handler;
    pro_handler->context = channel;
    pro_handler->handle_conn_ack = channel_handle_conn_ack;
    pro_handler->handle_ping_resp = channel_handle_ping_resp;
    pro_handler->handle_sync = channel_handle_sync;
    pro_handler->handle_disconnect = channel_handle_disconnect;
    err = regina_tcp_pro_create(&channel->protocol, pro_handler);
    REGINA_RETURN_IF_ERR(err);

    return REGINA_ERR_NONE;
}

static int channel_send(channel_t *channel)
{
    int err = REGINA_ERR_NONE;

    if (regina_stream_get_size(channel->outbound_buf) == 0)
        return REGINA_ERR_NONE;

    while (CHANNEL_STATUS_CLOSED != channel->status) {
        regina_extent_t ext[1];
        size_t count = 1;
        unsigned int bytes_sent;

        regina_stream_extract(channel->outbound_buf, ext, &count);

        if (0 == count)
            break;

        err = tcp_util_send(channel->fd, ext->base, ext->len, &bytes_sent);
        if (REGINA_ERR_NONE != err)
            return err;

        regina_stream_erase(channel->outbound_buf, bytes_sent);
        if (bytes_sent != ext->len) /* the buffer may be full wait amoment */
            regina_sleep(10);

        if (channel->timeout && (regina_time(NULL) >= channel->timeout)) {
            err = REGINA_ERR_TIMEOUT;
            break;
        }
    }

    if (REGINA_ERR_NONE != err) {
        REGINA_LOG1(WARNING, "Failed to send, remaining %lu bytes.",
                    regina_stream_get_size(channel->outbound_buf));
    }

    channel_delay_next_ping_time(channel);
    return err;
}

/**
 * @return zero if failed, otherwise non-zero
 */
static int channel_connect_to_server(channel_t *channel)
{
    int err;

    regina_time_t expired_time;
    expired_time = regina_time(NULL) + channel->keep_alive / 2 + 1; /* avoid 0 */

    channel->fd = NULL;
    while (1) {
        if (CHANNEL_STATUS_OPENING != channel->status) {
            err = REGINA_ERR_NONE;
            break;
        }

        if (regina_time(NULL) >= expired_time) {
            err = REGINA_ERR_TIMEOUT;
            break;
        }

        err = tcp_util_open(&channel->fd, channel->acc_point->address);
        if (REGINA_ERR_NONE == err)
            continue;

        if (REGINA_ERR_SOCKET_INPROGRESS == err) {
            regina_sleep(20);
            continue;
        }

        if (REGINA_ERR_SOCKET_ISCONN == err) {
            err = REGINA_ERR_NONE;
            break;
        }

        break;
    }

    if (REGINA_ERR_NONE == err) {
        channel_transform_status(channel, CHANNEL_STATUS_OPENED);

        channel_delay_expired_time(channel);
        channel_delay_next_ping_time(channel);
    }
    else {
        channel->fd = NULL;
    }

    return err;
}

static int channel_recv(channel_t *channel)
{
    while (CHANNEL_STATUS_CLOSED != channel->status) {
        unsigned int bytes_read;
        char *buf;
        int err;

        err = regina_stream_reserve_next(channel->inbound_buf,
                                         &buf, INBOUND_BUF_SIZE);
        REGINA_RETURN_IF_ERR(err);

        err = tcp_util_recv(channel->fd, buf, INBOUND_BUF_SIZE, &bytes_read);
        REGINA_RETURN_IF_ERR(err);

        if (bytes_read != INBOUND_BUF_SIZE) { /* no data to receive. */
            regina_stream_revert(channel->inbound_buf, INBOUND_BUF_SIZE - bytes_read);
            break;
        }
    }

    return REGINA_ERR_NONE;
}

static int channel_handle_recv_packets(channel_t *channel)
{
    while (CHANNEL_STATUS_CLOSED != channel->status) {
        int err;

        err = regina_tcp_pro_handle_inbound_pkt(channel->protocol,
                                                channel->inbound_buf);
        if (err < 0)
            return err;

        if (0 == err)
            break;

        channel_delay_expired_time(channel);
    }

    return REGINA_ERR_NONE;
}

int channel_create(channel_t **channel)
{
    int err;

    *channel = (channel_t*)regina_malloc(sizeof(channel_t));
    if (!*channel)
        return REGINA_ERR_NOMEM;

    regina_memset(*channel, 0, sizeof(channel_t));
    err = channel_init(*channel);
    if (REGINA_ERR_NONE != err) {
        channel_destroy(*channel);
        return err;
    }

    return REGINA_ERR_NONE;
}

void channel_destroy(channel_t *channel)
{
    if (!channel)
        return;

    channel->hook = NULL; /* avoid call the hook */
    channel_internal_close(channel, REGINA_ERR_NONE);

    regina_stream_destroy(channel->inbound_buf);
    regina_stream_destroy(channel->outbound_buf);
    regina_tcp_pro_destroy(channel->protocol);

    regina_free(channel);
}

int channel_set_access_point(channel_t *channel, access_point_t *acc_point, channel_hook_t *hook)
{
    REGINA_RETURN_IF_NIL(acc_point, REGINA_ERR_BADPARAM);

    if (TCP_PROTOCOL_VERSION != acc_point->protocol_version) {
        REGINA_LOG_DEBUG1("Unsupport protocol: %d", acc_point->protocol_version);
        return REGINA_ERR_BAD_PROTOCOL_VERSION;
    }

    channel->acc_point = acc_point;
    channel->hook = hook;

    return REGINA_ERR_NONE;
}

int channel_open(channel_t *channel)
{
    if (CHANNEL_STATUS_CLOSED != channel->status) {
        channel_internal_close(channel, REGINA_ERR_NONE);
    }

    regina_stream_reset(channel->outbound_buf);
    regina_stream_reset(channel->inbound_buf);

    channel_delay_expired_time(channel);
    channel_delay_next_ping_time(channel);

    channel_transform_status(channel, CHANNEL_STATUS_OPENING);

    return channel_connect_to_server(channel);
}

int channel_close(channel_t *channel)
{
    channel_transform_status(channel, CHANNEL_STATUS_DISCONNECTING);
    return REGINA_ERR_NONE;
}

int channel_bootstrap_auth(channel_t *channel)
{
    int err;

    err = regina_tcp_pro_set_session_key(channel->protocol, NULL, 0,
                                         REGINA_TCP_PRO_CRYPTO_ALG_AES);

    if (REGINA_ERR_NONE == err) {
        err = regina_tcp_pro_pack_bootstrap_connectpkt(channel->protocol,
                                                       channel->acc_point->key->base,
                                                       channel->acc_point->key->len,
                                                       ENUM_SESSION_KEY_TYPE_SESSION_KEY_AES,
                                                       channel->outbound_buf);
    }

    if (REGINA_ERR_NONE != err) {
        REGINA_LOG1(ERROR, "Failed to pack the BOOTSTRAP authenticate"
                    " packet, error=%d.", err);
    }

    return err;
}

int channel_acceptor_auth(channel_t *channel, regina_stream_t *payload)
{
    int err;

    err = regina_tcp_pro_set_session_key(channel->protocol,
                                         channel->acc_point->key->base,
                                         channel->acc_point->key->len,
                                         REGINA_TCP_PRO_CRYPTO_ALG_AES);

    if (REGINA_ERR_NONE == err) {
        err = regina_tcp_pro_pack_acceptor_connectpkt(channel->protocol,
                                                      channel->keep_alive,
                                                      channel->acc_point->token->base,
                                                      channel->acc_point->token->len,
                                                      payload,
                                                      channel->outbound_buf);
    }

    return err;
}

int channel_acceptor_send(channel_t *channel, regina_stream_t *payload)
{
    int err;
    if (CHANNEL_STATUS_AUTHENTICATED != channel->status)
        return REGINA_ERR_BAD_STATE;

    err = regina_tcp_pro_pack_sync_pkt(channel->protocol, 0, 1,
                                       payload, channel->outbound_buf);
    return err;
}

int channel_poll(channel_t *channel)
{
    int err;
    regina_time_t cur_time;

    if (CHANNEL_STATUS_CLOSED == channel->status)
        return REGINA_ERR_BAD_STATE;

    if (CHANNEL_STATUS_DISCONNECTING == channel->status) {
        channel_internal_close(channel, REGINA_ERR_NONE);
        return REGINA_ERR_SOCKET_IO_EOF;
    }

    cur_time = regina_time(NULL);
    if ((channel->timeout && cur_time >= channel->timeout) ||
        (cur_time >= channel->expired_time)) {
        channel_internal_close(channel, REGINA_ERR_TIMEOUT);
        return REGINA_ERR_TIMEOUT;
    }

    err = channel_recv(channel);
    if (REGINA_ERR_NONE != err) {
        channel_internal_close(channel, err);
        return err;
    }

    err = channel_send(channel);
    if (REGINA_ERR_NONE != err) {
        channel_internal_close(channel, err);
        return err;
    }

    if (regina_stream_get_size(channel->inbound_buf)) {
        err = channel_handle_recv_packets(channel);
        if (REGINA_ERR_NONE != err) {
            channel_internal_close(channel, err);
            return err;
        }
    }

    if ((CHANNEL_STATUS_OPENING != channel->status) &&
        (cur_time >= channel->next_ping_time)) {
        if (REGINA_ERR_NONE == err) {
            REGINA_LOG_DEBUG0("Send ping...");
            err = regina_tcp_pro_pack_ping_pkt(channel->protocol,
                                               channel->outbound_buf);
            channel_delay_next_ping_time(channel);
        }
    }

    return err;
}

void channel_set_timeout(channel_t *channel, int seconds)
{
    channel->timeout = seconds > 0 ? (regina_time(NULL) + seconds) : 0;
}

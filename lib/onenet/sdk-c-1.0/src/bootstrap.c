#include "bootstrap.h"
#include "regina/allocator.h"
#include "extent.h"
#include "std_wrapper.h"
#include "common.h"
#include "util.h"
#include "regina_config.h"
#include "channel.h"
#include "stream.h"
#include "thread.h"
#include "bs_dev_gen.h"


static void bootstrap_clear_acc_point(access_point_t *ap)
{
    tcp_util_ip_address_destroy(ap->address);
    regina_free(ap->key);
    regina_free(ap->token);

    ap->address = NULL;
    ap->key = NULL;
    ap->token = NULL;

    regina_memset(ap, 0, sizeof(access_point_t));
}

static void bootstrap_clear_bs_ap(bootstrap_t *bs)
{
    size_t i;
    for (i = 0; i < bs->bs_ap_count; ++i)
        bootstrap_clear_acc_point(&bs->bs_ap[i]);

    bs->bs_ap_count = 0;
    bs->bs_ap_cursor = 0;
}

static void bootstrap_clear_acceptor_ap(bootstrap_t *bs)
{
    size_t i;
    for (i = 0; i < bs->acc_ap_count; ++i)
        bootstrap_clear_acc_point(&bs->acc_ap[i]);

    bs->acc_ap_count = 0;
    bs->acc_ap_cursor = 0;
}

/* use regina_free to free the iovec instance. */
static regina_extent_t *bootstrap_copy_create_extent(const char *data, size_t size)
{
    regina_extent_t *ext = (regina_extent_t*)regina_malloc(sizeof(regina_extent_t) + size);
    if (ext) {
        ext->base = (char*)ext + sizeof(regina_extent_t);
        ext->len = size;

        regina_memcpy(ext->base, data, size);
    }
    return ext;
}

static int bootstrap_parse_servers(bootstrap_t *bs,
                                   const regina_bootstrap_access_point_t *acc_point)
{
    ip_address_t *addresses[MAX_ACC_POINTS_COUNT];
    unsigned int count = MAX_ACC_POINTS_COUNT;
    size_t i;
    int err;

    bootstrap_clear_bs_ap(bs);

    REGINA_LOG_DEBUG2("BOOTSTRAP server: %s:%d.",
                      acc_point->host->data, acc_point->port);

    regina_memset(addresses, 0, sizeof(addresses));
    err = tcp_util_getaddrinfo(acc_point->host->data,
                               acc_point->port,
                               0, addresses, &count);
    REGINA_RETURN_IF_ERR(err);

    if (0 == count)
        return REGINA_ERR_NOSERVER;

    for (i = 0; i < count; ++i) {
        bs->bs_ap[i].key = bootstrap_copy_create_extent(
            (char*)acc_point->public_key->buffer, acc_point->public_key->size);

        if (!bs->bs_ap[i].key) {
            err = REGINA_ERR_NOMEM;
            break;
        }

        bs->bs_ap[i].address = addresses[i];
        bs->bs_ap[i].protocol_version = TCP_PROTOCOL_VERSION;
    }

    if (REGINA_ERR_NONE != err) {
        int moved_count = i;

        for (; (int)i < count; ++i) {
            tcp_util_ip_address_destroy(addresses[i]);
        }

        while (moved_count > 0) {
            --moved_count;
            tcp_util_ip_address_destroy(bs->bs_ap[moved_count].address);
            regina_free(bs->bs_ap[moved_count].key);
            regina_memset(bs->bs_ap + moved_count, 0, sizeof(access_point_t));
        }

        REGINA_LOG0(ERROR, "No memory to store the access information of server.");
        return err;
    }

    bs->bs_ap_count = i;
    bs->bs_ap_cursor = 0;

    return REGINA_ERR_NONE;
}

static void bootstrap_handle_close(void *context, int err)
{
    REGINA_LOG_DEBUG1("Connection between BOOTSTRAP closed,"
                      " error=%d.", err);
}

static int bootstrap_handle_conn_ack(void *context, const char *payload, size_t size)
{
    int err = REGINA_ERR_NONE;
    bootstrap_t *bs = (bootstrap_t*)context;
    avro_reader_t reader;
    regina_access_response_t *resp;
    regina_list_node_t *cursor = NULL;

    if (!payload || !size) {
        REGINA_LOG0(ERROR, "Bug: no payload in connack but succcessfully.");
        return REGINA_ERR_PROTOCOL_ERROR;
    }

    reader = avro_reader_memory(payload, size);
    REGINA_RETURN_IF_NIL(reader, REGINA_ERR_NOMEM);

    resp = regina_access_response_deserialize(reader);
    avro_reader_free(reader);

    REGINA_RETURN_IF_NIL(resp, REGINA_ERR_NOMEM);

    REGINA_LOG_DEBUG0("Receive the CONNACK from the bootstrap server.");

    if (resp->acc_list)
        cursor = regina_list_begin(resp->acc_list);

    while (cursor && (bs->acc_ap_count < MAX_ACC_POINTS_COUNT)) {
        access_point_t *ap = bs->acc_ap + bs->acc_ap_count;
        unsigned int addcnt;
        regina_acceptor_node_t *acc_node;
        char ip[16];

        acc_node = (regina_acceptor_node_t*)regina_list_get_data(cursor);

        /* ipv4 only. */
        if (4 != acc_node->ip->size)
            continue;

        ap->protocol_version = TCP_PROTOCOL_VERSION;
        ap->token_expired_time = (int32_t)regina_time(NULL) + acc_node->token_live_time;


        if ((REGINA_UNION_NULL_OR_BYTES_BRANCH_1 == acc_node->key->type) &&
            acc_node->key->data) {
            regina_bytes_t *key = (regina_bytes_t*)acc_node->key->data;
            ap->key = bootstrap_copy_create_extent((char*)key->buffer, key->size);
        }

        ap->token = bootstrap_copy_create_extent(
            (char*)acc_node->token->buffer, acc_node->token->size);

        if (!ap->key || !ap->token) {
            err = REGINA_ERR_NOMEM;
            break;
        }

        regina_snprintf(ip, sizeof(ip), "%u.%u.%u.%u",
                        acc_node->ip->buffer[0], acc_node->ip->buffer[1],
                        acc_node->ip->buffer[2], acc_node->ip->buffer[3]);
        REGINA_LOG_DEBUG3("ACCEPTOR server: %s:%d, token livetime=%d.",
                          ip, acc_node->port, acc_node->token_live_time);

        addcnt = 1;
        err = tcp_util_getaddrinfo(ip, (uint16_t)acc_node->port, 0, &ap->address, &addcnt);

        if (REGINA_ERR_NONE != err) {
            bootstrap_clear_acc_point(ap);
            REGINA_LOG0(FATAL, "Bug: can't parse the address of the accpetor server.");
            break;
        }

        cursor = regina_list_next(cursor);
        ++bs->acc_ap_count;
    }

    resp->destroy(resp);

    return err;
}

static int bootstrap_do_retreive_acceptor(bootstrap_t *bs)
{
    int err = REGINA_ERR_NONE;
    channel_hook_t hook;

    hook.context = bs;
    hook.handle_close = bootstrap_handle_close;
    hook.handle_conn_ack = bootstrap_handle_conn_ack;
    hook.handle_sync = NULL;

    bootstrap_clear_acceptor_ap(bs);

    while (bs->bs_ap_cursor < bs->bs_ap_count) {
        access_point_t *ap = bs->bs_ap + (bs->bs_ap_cursor++);

        if (REGINA_ERR_NONE != channel_set_access_point(bs->channel, ap, &hook))
            continue;

        err = channel_open(bs->channel);
        if (REGINA_ERR_NONE != err) {
            REGINA_LOG1(ERROR, "Failed to connect to the BOOTSTRAP, error=%d.", err);
            continue;
        }

        err = channel_bootstrap_auth(bs->channel);
        if (REGINA_ERR_NONE != err)
            continue;


        channel_set_timeout(bs->channel, 60);
        while (REGINA_ERR_NONE == err) {
            err = channel_poll(bs->channel);
            if (bs->acc_ap_count)
                break;

            regina_sleep(100);
        }
        channel_set_timeout(bs->channel, 0);
    }

    return err;
}

void bootstrap_init(bootstrap_t *bootstrap, channel_t *channel)
{
    regina_memset(bootstrap, 0, sizeof(bootstrap_t));
    bootstrap->channel = channel;
}

void bootstrap_destroy(bootstrap_t *bootstrap)
{
    bootstrap_clear_bs_ap(bootstrap);
    bootstrap_clear_acceptor_ap(bootstrap);
}

int bootstrap_retreive_acceptor(bootstrap_t *bootstrap)
{
    int err = REGINA_ERR_NONE;

    REGINA_LOG_DEBUG0("Begin to retreive acceptor servers.");

    while (1) {
        const regina_bootstrap_access_point_t *ap;

        ap = regina_config_get_next_bootstrap_server(TCP_PROTOCOL_VERSION);
        if (NULL == ap) {
            err = REGINA_ERR_NOSERVER;
            break;
        }

        err = bootstrap_parse_servers(bootstrap, ap);
        if (REGINA_ERR_NONE != err) {
            REGINA_LOG1(ERROR, "Failed to parse the server address, error=%d.", err);
            break;
        }

        err = bootstrap_do_retreive_acceptor(bootstrap);
        if (REGINA_ERR_NONE == err)
            break;
    }
    return err;
}

int bootstrap_get_next_acceptor(bootstrap_t *bootstrap, access_point_t **ap)
{
    if (bootstrap->acc_ap_cursor < bootstrap->acc_ap_count) {
        *ap = bootstrap->acc_ap + (bootstrap->acc_ap_cursor++);
        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_READ_FAILED;
}

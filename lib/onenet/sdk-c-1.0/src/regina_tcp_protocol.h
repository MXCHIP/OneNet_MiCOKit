#ifndef __REGINA_TCP_PROTOCOL_H__
#define __REGINA_TCP_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "std_wrapper.h"
#include "bs_dev_gen.h"

#define TCP_PROTOCOL_VERSION  0x01


typedef struct regina_tcp_pro_t regina_tcp_pro_t;
struct regina_stream_t;

typedef enum {
    REGINA_TCP_PRO_PKT_CONNECT    = 1,
    REGINA_TCP_PRO_PKT_CONNACK    = 2,
    REGINA_TCP_PRO_PKT_DISCONNECT = 3,
    REGINA_TCP_PRO_PKT_PINGREQ    = 4,
    REGINA_TCP_PRO_PKT_PINGRESP   = 5,
    REGINA_TCP_PRO_PKT_SYNC       = 6
} regina_tcp_pro_packet_type_t;

typedef enum {
    REGINA_TCP_PRO_COMP_ALG_NONE = 0x00,
    REGINA_TCP_PRO_COMP_ALG_ZIP  = 0x01
} regina_tcp_pro_comp_alg_t;

typedef enum {
    REGINA_TCP_PRO_CRYPTO_ALG_NONE = 0x00,
    REGINA_TCP_PRO_CRYPTO_ALG_AES  = 0x01
} regina_tcp_pro_crypto_alg_t;

typedef enum {
    REGINA_TCP_PRO_CONNACK_SUCCESS                = 0x00,
    REGINA_TCP_PRO_CONNACK_PROTOCOL_VERSION_ERROR = 0x01,
    REGINA_TCP_PRO_CONNACK_SERVER_UNUSABLE        = 0x02,
    REGINA_TCP_PRO_CONNACK_KEY_TYPE_UNSUPPORTED   = 0x03, /* used by bootstrap. */
    REGINA_TCP_PRO_CONNACK_TOKEN_INVALID          = 0x03  /* used by acceptor. */
} regina_tcp_pro_connack_t;

typedef enum {
    REGINA_TCP_PRO_DISCONN_NONE = 0x00,
    REGINA_TCP_PRO_DISCONN_SERVICE_NOT_AVAILABLE,
    REGINA_TCP_PRO_DISCONN_DUP_LOGIN,
    REGINA_TCP_PRO_DISCONN_PROTOCOL_ERROR,
    REGINA_TCP_PRO_DISCONN_SERVER_INTERNAL_ERROR,
    REGINA_TCP_PRO_DISCONN_INVALID_DATA,
    REGINA_TCP_PRO_DISCONN_TIMEOUT,

    REGINA_TCP_PRO_DISCONN_REASON_COUNT
} regina_tcp_pro_disconn_reason_t;

typedef struct {
    void *context;
    int (*handle_conn_ack)(void *context, regina_tcp_pro_connack_t ack,
                           const char *payload, size_t size);
    void (*handle_ping_resp)(void *context);
    int (*handle_sync)(void *context, const char *payload, size_t size);
    void (*handle_disconnect)(void *context, regina_tcp_pro_disconn_reason_t reason);
} regina_tcp_pro_handler_t;

int regina_tcp_pro_create(regina_tcp_pro_t **protocol, regina_tcp_pro_handler_t *handler);
void regina_tcp_pro_destroy(regina_tcp_pro_t *protocol);
int regina_tcp_pro_set_session_key(regina_tcp_pro_t *protocol,
                                   const char *key, size_t size,
                                   regina_tcp_pro_crypto_alg_t alg);
int regina_tcp_pro_pack_bootstrap_connectpkt(regina_tcp_pro_t *protocol,
                                             const char *pub_key, size_t pbk_len,
                                             regina_session_key_type_t acc_key_type,
                                             struct regina_stream_t *pkt);
int regina_tcp_pro_pack_acceptor_connectpkt(regina_tcp_pro_t *protocol,
                                            uint16_t keepalive,
                                            const char *token, size_t tk_len,
                                            struct regina_stream_t *payload,
                                            struct regina_stream_t *pkt);
int regina_tcp_pro_pack_sync_pkt(regina_tcp_pro_t *protocol,
                                 int comp, int enc,
                                 struct regina_stream_t *payload,
                                 struct regina_stream_t *pkt);
int regina_tcp_pro_pack_ping_pkt(regina_tcp_pro_t *protocol, struct regina_stream_t *pkt);
int regina_tcp_pro_pack_disconnect_pkt(regina_tcp_pro_t *protocol,
                                       regina_tcp_pro_disconn_reason_t reason,
                                       struct regina_stream_t *pkt);
/**
 * handle one packet in the stream.
 * @return positive number if a complete packet handled,
 *         0 if the packet in the stream is not complete,
 *         negative number if the some error occurs.
 */
int regina_tcp_pro_handle_inbound_pkt(regina_tcp_pro_t *protocol,
                                      struct regina_stream_t *stream);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINAL_TCP_PROTOCOL_H__ */

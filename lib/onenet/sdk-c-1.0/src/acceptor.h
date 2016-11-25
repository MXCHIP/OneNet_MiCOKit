#ifndef __REGINA_ACCEPTOR_H__
#define __REGINA_ACCEPTOR_H__

#include "regina/device.h"
#include "access_point.h"

typedef struct acceptor_t acceptor_t;
struct regina_module_info_t;
struct regina_uplink_data_t;
struct regina_uplink_data_ack_t;
struct channel_t;

int acceptor_create(acceptor_t **acc, struct channel_t *channel);
void acceptor_destroy(acceptor_t *acc);
int acceptor_check_and_keepalive(acceptor_t *acc);
int acceptor_authenticate(acceptor_t *acc, access_point_t *ap,
                          const char *profile, size_t prof_len,
                          const regina_module_info_t *modules,
                          size_t module_count,
                          const struct regina_uplink_data_t *data,
                          struct regina_uplink_data_ack_t *ack);
int acceptor_send(acceptor_t *acc,
                  const struct regina_uplink_data_t *data,
                  struct regina_uplink_data_ack_t *ack);
int acceptor_handle_downlink_data(acceptor_t *acc, void *ctx,
                                  notification_callback nt_callback,
                                  configuration_callback cf_callback,
                                  software_update_callback su_callback);
int acceptor_get_configuration(acceptor_t *acc, char **conf, size_t *size);
int acceptor_request_thirdparty_service(acceptor_t *acc, regina_thirdparty_service_qos_t qos,
                                        const char *request, size_t req_size,
                                        regina_enum_ack_t *ack,
                                        char **resonse, size_t *resp_size);

#endif /* __REGINA_ACCEPTOR_H__ */

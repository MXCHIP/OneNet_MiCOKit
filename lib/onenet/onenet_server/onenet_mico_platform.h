#ifndef __ONENET_MICO_PLATFORM__H
#define __ONENET_MICO_PLATFORM__H

#include "mico.h"

#define ONENET_MICO_OK          0
#define ONENET_MICO_SOCKET_ERR  -600
#define ONENET_MICO_CONNECT_ERR -601
#define ONENET_MICO_DNS_ERR     -602

void onenet_mico_platform_para_read(char *buf, unsigned short addr, int len);

void onenet_mico_platform_para_write(char *buf, unsigned short addr, int len);

int onenet_mico_platform_tcp_dns(char *host, char *ip, int len);

int onenet_mico_platform_tcp_open(int *fd, char *ip, int port);

int onenet_mico_platform_tcp_send(int socket, void *buffer, size_t size, int flags);

int onenet_mico_platform_tcp_recv(int socket, void *buffer, size_t size, int flags);

void onenet_mico_platform_tcp_close( int socket );

uint32_t onenet_mico_platform_get_time( void );

#endif

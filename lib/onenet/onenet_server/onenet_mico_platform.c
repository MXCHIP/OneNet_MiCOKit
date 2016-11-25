#include "onenet_mico_platform.h"
#include "onenet.h"

//#define onenet_log(format, ...)  custom_log("onenet", format, ##__VA_ARGS__)
#define onenet_log(format, ...)

void onenet_mico_platform_para_read(char *buf, unsigned short addr, int len)
{
    onenet_context_t *onenet_context = onenet_server_context_get( );
    memcpy(buf, &onenet_context->onenet_config->onenet_file[addr], len);
}

void onenet_mico_platform_para_write(char *buf, unsigned short addr, int len)
{
    onenet_context_t *onenet_context = onenet_server_context_get( );
    memcpy(&onenet_context->onenet_config->onenet_file[addr], buf,  len);
    onenet_server_para_storage();
}

int onenet_mico_platform_tcp_dns(char *host, char *ip, int len)
{
    struct hostent* hostent_content = NULL;
    char **pptr = NULL;
    struct in_addr in_addr;

    hostent_content = gethostbyname(host);
    if( hostent_content == NULL )
        return ONENET_MICO_DNS_ERR;

    pptr=hostent_content->h_addr_list;
    in_addr.s_addr = *(uint32_t *)(*pptr);
    strcpy( ip, inet_ntoa(in_addr));

   return ONENET_MICO_OK;
}

int onenet_mico_platform_tcp_open(int *fd, char *ip, int port)
{
    struct sockaddr_in addr;

    *fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( *fd < 0 ){
        onenet_mico_platform_tcp_close(*fd);
        return ONENET_MICO_SOCKET_ERR;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if ( -1 == connect(*fd, (struct sockaddr *)&addr, sizeof(addr)) ) {
        onenet_mico_platform_tcp_close(*fd);
        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }
    return ONENET_MICO_OK;
}

int onenet_mico_platform_tcp_send(int socket, void *buffer, size_t size, int flags)
{
    int res = 0;
    res = send(socket, buffer, size, flags);
    return res;
}

int onenet_mico_platform_tcp_recv(int socket, void *buffer, size_t size, int flags)
{
    int res = 0;
    struct timeval t;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(socket, &readfds);

    t.tv_sec = 0;
    t.tv_usec = 10*1000;

    select(socket + 1, &readfds, NULL, NULL, &t);

    if (FD_ISSET(socket, &readfds)){
        res = recv(socket, buffer, size, flags);
    }

    return res;
}

void onenet_mico_platform_tcp_close( int socket )
{
    if( socket != -1 ){
        close(socket);
    }
}

uint32_t onenet_mico_platform_get_time( void )
{
    return mico_rtos_get_time();
}


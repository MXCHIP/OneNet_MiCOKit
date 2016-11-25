#include "tcp_util.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include <sockets.h>
#include "err.h"
#include "api.h"
#include "regina/log.h"
#define socklen_t int32_t

struct socket_fd_t {
    int fd;
};

struct ip_address_t {
    socklen_t len;
    struct sockaddr *addr;
};

static int tcp_util_ip_address_create(ip_address_t **address,
                                      const char *addr,
                                      socklen_t len, int port)
{
    struct sockaddr_in *sa = NULL;
    struct ip_addr ipaddr;
    *address = (ip_address_t*)regina_malloc(sizeof(ip_address_t) + sizeof(struct sockaddr_in));
    if (!*address)
        return REGINA_ERR_NOMEM;

    memset((char *)(*address),0,sizeof(ip_address_t) +sizeof(struct sockaddr_in));
    (*address)->addr = (struct sockaddr *)((char *)(*address) + sizeof(ip_address_t));
    sa=(struct sockaddr_in *)((*address)->addr);
    /* set up sockaddr */
    ipaddr.addr=ipaddr_addr(addr);
    inet_addr_from_ipaddr(&sa->sin_addr, &ipaddr);
    sa->sin_family = AF_INET;
    sa->sin_len = sizeof(struct sockaddr_in);
    sa->sin_port = htons((u16_t)port);
    (*address)->len=sizeof(struct sockaddr_in);

    return REGINA_ERR_NONE;
}

void tcp_util_ip_address_destroy(ip_address_t *address)
{
    if (address)
        regina_free(address);
}

// host 只支持IP格式
int tcp_util_getaddrinfo(const char *host, unsigned int port, int ipv6,
                         ip_address_t **addresses, unsigned int *addrcnt)
{
    *addrcnt = 1;
    return tcp_util_ip_address_create(addresses,host,strlen(host),port);
}

int tcp_util_open(socket_fd_t **fd, ip_address_t *address)
{
    int err;
    struct ip_addr ipaddr;
    int count = 20;
    static ip_addr_t loca_ipaddr;
    static u16_t loca_port;
    struct sockaddr_in sa ;

    struct netconn * tcp_clientconn=NULL;
    
    if (!fd || !address)
        return REGINA_ERR_BADPARAM;

    if (NULL == *fd) {
        int flags;

        *fd = (socket_fd_t*)regina_malloc(sizeof(socket_fd_t));
        if (!*fd) {
            return REGINA_ERR_NOMEM;
        }

        (*fd)->fd = socket(AF_INET, SOCK_STREAM, 0);
        if ((*fd)->fd < 0) {
            tcp_util_close(*fd);
            return REGINA_ERR_SOCKET_ERROR;
        }

        flags = fcntl((*fd)->fd, F_GETFL, 0);
        if (-1 == fcntl((*fd)->fd, F_SETFL, flags | O_NONBLOCK)) {
            REGINA_LOG1(ERROR, "Failed to set the socket to non blocking mode: %s",
                       strerror(errno));
            tcp_util_close(*fd);
            return REGINA_ERR_SOCKET_ERROR;
        }
    }

    if (-1 == connect((*fd)->fd, address->addr, address->len)) {
        if (EINPROGRESS == errno)
            return REGINA_ERR_SOCKET_INPROGRESS;
        
        if (EALREADY==errno)
           return REGINA_ERR_SOCKET_ISCONN;

        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }
    return REGINA_ERR_NONE;
}

int tcp_util_send(socket_fd_t *fd, const char *buf,
                  unsigned int size, unsigned int *bytes_sent)
{
    int result;

    if (!buf || !size || !bytes_sent || !fd)
        return REGINA_ERR_BADPARAM;

    result = send(fd->fd, buf, size, MSG_DONTWAIT);
    
    if (result >= 0) {
        *bytes_sent = (int)result;
        return REGINA_ERR_NONE;
    }

    if ((EAGAIN == errno) || (EWOULDBLOCK == errno)|| 
        (EINPROGRESS == errno)|| (ECONNABORTED == errno)) {
        *bytes_sent = 0;
        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_SOCKET_IO_ERROR;
}

int tcp_util_recv(socket_fd_t *fd, char *buf,
                  unsigned int size, unsigned int *bytes_read)
{
    int result;

    if (!buf || !size || !bytes_read || !fd)
        return REGINA_ERR_BADPARAM;

    result = recv(fd->fd, buf, size,MSG_DONTWAIT);
    
    if (result > 0) {
        *bytes_read = (int)result;
        return REGINA_ERR_NONE;
    }

    if (!result)
        return REGINA_ERR_SOCKET_IO_EOF;

    if ((EAGAIN == errno) || (EWOULDBLOCK == errno)|| (EINPROGRESS == errno)) {
        *bytes_read = 0;
        return REGINA_ERR_NONE;
    }        
    return REGINA_ERR_SOCKET_IO_ERROR;
}

void tcp_util_close(socket_fd_t *fd)
{
    if (fd) {
        close(fd->fd);
        regina_free(fd);
    }
}
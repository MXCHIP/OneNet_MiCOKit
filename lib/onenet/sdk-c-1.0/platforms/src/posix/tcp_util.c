#include "tcp_util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"

struct socket_fd_t {
    int fd;
};

struct ip_address_t {
    socklen_t len;
    struct sockaddr *addr;
};

static int tcp_util_ip_address_create(ip_address_t **address,
                                      struct sockaddr *addr,
                                      socklen_t len)
{
    *address = (ip_address_t*)regina_malloc(sizeof(ip_address_t) + len);
    if (!*address)
        return REGINA_ERR_NOMEM;

    (*address)->addr = (struct sockaddr*)((char*)(*address) + sizeof(ip_address_t));
    memcpy((*address)->addr, addr, len);
    (*address)->len = len;
    return REGINA_ERR_NONE;
}

void tcp_util_ip_address_destroy(ip_address_t *address)
{
    if (address)
        regina_free(address);
}


int tcp_util_getaddrinfo(const char *host, unsigned int port, int ipv6,
                         ip_address_t **addresses, unsigned int *addrcnt)
{
    struct addrinfo hints;
    char service[6];
    int err;
    int count;
    struct addrinfo *resolve_result = NULL;
    struct addrinfo *cursor;

    if (!host || !addresses || !addrcnt || !*addrcnt || (port > 65535))
        return REGINA_ERR_BADPARAM;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    if (!ipv6)
        hints.ai_family = AF_INET;

    snprintf(service, sizeof(service), "%d", port);
    err = getaddrinfo(host, service, &hints, &resolve_result);

    if (err || !resolve_result)
        return REGINA_ERR_RESOLVE_HOST_FAILED;

    count = 0;
    cursor = resolve_result;
    while (cursor && (count < *addrcnt)) {
        err = tcp_util_ip_address_create(addresses + count,
                                         cursor->ai_addr, cursor->ai_addrlen);

        if (REGINA_ERR_NONE != err)
            return err;

        cursor = cursor->ai_next;
        ++count;
    }

    *addrcnt = count;
    freeaddrinfo(resolve_result);
    return REGINA_ERR_NONE;
}

int tcp_util_open(socket_fd_t **fd, ip_address_t *address)
{
    if (!fd || !address)
        return REGINA_ERR_BADPARAM;


    if (NULL == *fd) {
        int flags;

        *fd = (socket_fd_t*)regina_malloc(sizeof(socket_fd_t));
        if (!*fd) {
            return REGINA_ERR_NOMEM;
        }


        (*fd)->fd = socket(address->addr->sa_family, SOCK_STREAM, 0);
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
        if (EISCONN == errno)
            return REGINA_ERR_SOCKET_ISCONN;

        if ((EINPROGRESS == errno) || (EALREADY == errno)){
            return REGINA_ERR_SOCKET_INPROGRESS;
        }

        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }

    return REGINA_ERR_NONE;
}

int tcp_util_send(socket_fd_t *fd, const char *buf,
                  unsigned int size, unsigned int *bytes_sent)
{
    ssize_t result;

    if (!buf || !size || !bytes_sent || !fd)
        return REGINA_ERR_BADPARAM;

    result = send(fd->fd, buf, size, 0);

    if (result >= 0) {
        *bytes_sent = (int)result;
        return REGINA_ERR_NONE;
    }

    if ((EAGAIN == errno) || (EWOULDBLOCK == errno)) {
        *bytes_sent = 0;
        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_SOCKET_IO_ERROR;
}

int tcp_util_recv(socket_fd_t *fd, char *buf,
                  unsigned int size, unsigned int *bytes_read)
{
    ssize_t result;

    if (!buf || !size || !bytes_read || !fd)
        return REGINA_ERR_BADPARAM;

    result = recv(fd->fd, buf, size, 0);

    if (result > 0) {
        *bytes_read = (int)result;
        return REGINA_ERR_NONE;
    }

    if (!result)
        return REGINA_ERR_SOCKET_IO_EOF;

    if ((EAGAIN == errno) || (EWOULDBLOCK == errno)) {
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

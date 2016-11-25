#define _GNU_SOURCE  /* for sighandler_t */

#include "tcp_util.h"
#include "regina/error.h"
#include "regina/allocator.h"

#include "socket_types.h"
#include "socket_api.h"

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

// host 只支持IP格式

int tcp_util_getaddrinfo(const char *host, unsigned int port, int ipv6,
                         ip_address_t **addresses, unsigned int *addrcnt)
{
    struct sockaddr sa = {0};

    SCI_TRACE_LOW("regina: tcp_util_getaddrinfo %s,%d",host,port);

	sa.family  = AF_INET;
	sa.port    = htons(port);
    inet_aton(host, &sa.ip_addr);

    *addrcnt = 1;

    return tcp_util_ip_address_create(addresses,&sa,sizeof(struct sockaddr));
}

static void connect_alarm(int signo)
{
    /* just interrupt the connect() */
}

int tcp_util_open(socket_fd_t **fd, ip_address_t *address, unsigned int timeout)
{
    int err;

   // extern uint8                   s_nsapi   ;  // record nsapi when PDP activated

    SCI_TRACE_LOW("regina: tcp_util_open");

    if (!fd || !address)
        return REGINA_ERR_BADPARAM;

    *fd = (socket_fd_t*)regina_malloc(sizeof(socket_fd_t));
    if (!*fd) {
        return REGINA_ERR_NOMEM;
    }

    (*fd)->fd = sci_sock_socket(AF_INET, SOCK_STREAM, 0, 5);

    if ((*fd)->fd < 0)
    {
        regina_free(*fd);
        *fd = NULL;
        return REGINA_ERR_SOCKET_ERROR;
    }

    err = sci_sock_connect((*fd)->fd, address->addr, sizeof(struct sockaddr));

    if(-1 == err)
    {
        int sock_error_code = 0;

        sock_error_code = sci_sock_errno((*fd)->fd);

        if(EWOULDBLOCK == sock_error_code || EINPROGRESS == sock_error_code || EISCONN == sock_error_code)
        {
            SCI_TRACE_LOW("regina : sci_sock_connect %d",err);
            err = 0;
        }

    }

    if (-1 == err) {
        tcp_util_close(*fd);
        SCI_TRACE_LOW("regina : sci_sock_connect fail");
        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }

    SCI_TRACE_LOW("regina : sci_sock_connect ok");

    return REGINA_ERR_NONE;
}

int tcp_util_send(socket_fd_t *fd, const char *buf,
                  unsigned int size, unsigned int *bytes_sent)
{
    uint32 unsend_len = 0;
    uint32 send_len = 0;
	int32 sock_error_code    = 0;

    if (!buf || !size || !bytes_sent || !fd)
        return REGINA_ERR_BADPARAM;

    unsend_len =  size;

    SCI_TRACE_LOW("regina : tcp_util_send need to send %d,%s",size,buf);

    while(unsend_len > 0)
    {
        send_len = sci_sock_send(fd->fd, buf + size - unsend_len, unsend_len,0);

        if (send_len > 0)
        {
            unsend_len -= send_len;
            SCI_TRACE_LOW("regina : sci_sock_send %d,%d success",send_len,unsend_len);
            if(unsend_len == 0)
                break;
        }
        else
        {
            sock_error_code = sci_sock_errno(fd->fd);

			if(EPIPE == sock_error_code || ENOBUFS == sock_error_code || EWOULDBLOCK == sock_error_code){

                SCI_TRACE_LOW("regina : sci_sock_send error1 %d",sock_error_code);
            }
            else
            {
                *bytes_sent = 0;

                SCI_TRACE_LOW("regina : sci_sock_send error %d",sock_error_code);
                return REGINA_ERR_SOCKET_IO_ERROR;
            }

        }
     }

    *bytes_sent = size - unsend_len;

    SCI_TRACE_LOW("regina : tcp_util_send %d ok",*bytes_sent);

    return REGINA_ERR_NONE;
}

int tcp_util_recv(socket_fd_t *fd, char *buf,
                  unsigned int size, unsigned int *bytes_read)
{
    int result = 0;

    if (!buf || !size || !bytes_read || !fd)
        return REGINA_ERR_BADPARAM;

    result = sci_sock_recv(fd->fd, buf, size,0);

    SCI_TRACE_LOW("regina : sci_sock_recv %d,%d,%s",size,result,buf);

    if (result > 0) {

        *bytes_read = (int)result;

        SCI_TRACE_LOW("regina : sci_sock_recv ok %d,%d,%s",result,size,buf);

        return REGINA_ERR_NONE;
    }
    else if(result == 0) //服务器没有回应
    {
        *bytes_read = (int)result;

        SCI_TRACE_LOW("regina : sci_sock_recv nothing...");

        return REGINA_ERR_READ_FAILED;
    }
    else
    {
        SCI_TRACE_LOW("regina : sci_sock_recv fail %d,%d",result,sci_sock_errno(fd->fd));

        *bytes_read = 0;

        return REGINA_ERR_SOCKET_IO_EOF;
    }

    return REGINA_ERR_NONE;
}

int tcp_util_shutdown(socket_fd_t *fd)
{
    if (fd) {
        SCI_TRACE_LOW("regina : sci_sock_shutdown ");

        if (0 != sci_sock_shutdown(fd->fd, SD_BOTH))
            return REGINA_ERR_INTERNAL;
    }

    return REGINA_ERR_NONE;
}

void tcp_util_close(socket_fd_t *fd)
{
    if (fd) {
        sci_sock_socketclose(fd->fd);
        fd->fd = 0;
        regina_free(fd);
        fd = NULL;

        SCI_TRACE_LOW("regina : tcp_util_close ");
    }
}

#include "tcp_util.h"
#include "std_wrapper.h"
#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <errno.h>
#include <stdio.h>

typedef SSIZE_T  ssize_t;

struct socket_fd_t
{
    int fd;
};

struct ip_address_t
{
    socklen_t len;
    struct sockaddr *addr;
};

static int tcp_util_ip_address_create( ip_address_t **address,
struct sockaddr *addr,
    socklen_t len )
{
    *address = (ip_address_t*)regina_malloc( sizeof(ip_address_t)+len );
    if ( !*address )
        return REGINA_ERR_NOMEM;

    (*address)->addr = (struct sockaddr*)((char*)(*address) + sizeof(ip_address_t));
    memcpy( (*address)->addr, addr, len );
    (*address)->len = len;
    return REGINA_ERR_NONE;
}

void tcp_util_ip_address_destroy( ip_address_t *address )
{
    if ( address )
        regina_free( address );
}


int tcp_util_getaddrinfo( const char *host, unsigned int port, int ipv6,
                          ip_address_t **addresses, unsigned int *addrcnt )
{
    struct addrinfo hints;
    char service[6];
    int err;
    int count;
    struct addrinfo *resolve_result = NULL;
    struct addrinfo *cursor;

    if ( !host || !addresses || !addrcnt || !*addrcnt || (port > 65535) )
        return REGINA_ERR_BADPARAM;

    memset( &hints, 0, sizeof(hints) );
    hints.ai_socktype = SOCK_STREAM;
    if ( !ipv6 )
        hints.ai_family = AF_INET;

    sprintf_s( service, sizeof(service), "%d", port );
    err = getaddrinfo( host, service, &hints, &resolve_result );

    if ( err || !resolve_result )
        return REGINA_ERR_RESOLVE_HOST_FAILED;

    count = 0;
    cursor = resolve_result;
    while ( cursor && (count < *addrcnt) )
    {
        err = tcp_util_ip_address_create( addresses + count,
                                          cursor->ai_addr, cursor->ai_addrlen );

        if ( REGINA_ERR_NONE != err )
            return err;

        cursor = cursor->ai_next;
        ++count;
    }

    *addrcnt = count;
    freeaddrinfo( resolve_result );
    return REGINA_ERR_NONE;
}

int tcp_util_open( socket_fd_t **fd, ip_address_t *address )
{
    if ( !fd || !address )
        return REGINA_ERR_BADPARAM;


    if ( NULL == *fd )
    {
        u_long flags = 1;
        int ret = NO_ERROR;

        *fd = (socket_fd_t*)regina_malloc( sizeof(socket_fd_t) );
        if ( !*fd )
        {
            return REGINA_ERR_NOMEM;
        }

        (*fd)->fd = WSASocket( address->addr->sa_family, SOCK_STREAM, 0, NULL, 0, 0 );
        if ( (*fd)->fd < 0 )
        {
            tcp_util_close( *fd );
            return REGINA_ERR_SOCKET_ERROR;
        }

        DWORD returned = 0;
        ret = WSAIoctl( (*fd)->fd, FIONBIO, &flags, sizeof(flags), NULL, 0, &returned, NULL, NULL );
        if ( NO_ERROR != ret )
        {
            do
            {
                const regina_log_level_t ll = REGINA_LL_ERROR;
                if ( regina_logger.ll <= ll )
                {
                    void *ctx = regina_logger.context;

                    regina_logger.log( ctx, (ll), "WSAIoctl failed with error: %d!\n", WSAGetLastError() );
                }
            }
            while ( 0 );

            tcp_util_close( *fd );
            return REGINA_ERR_SOCKET_ERROR;
        }
    }

    if ( SOCKET_ERROR == WSAConnect( (*fd)->fd, address->addr, address->len, NULL, NULL, NULL, NULL ) )
    {
        if ( WSAEWOULDBLOCK == WSAGetLastError() )
        {
            // checking to see if socket is writeable
            int ret = 0;
            struct fd_set wset;
            struct timeval tv;

            FD_ZERO( &wset );
            FD_SET( (*fd)->fd, &wset );
            tv.tv_sec = 0;
            tv.tv_usec = 1000; // 1ms
            ret = select(1, NULL, &wset, NULL, &tv);

            if ( ret >= 0 )
            {
                return REGINA_ERR_NONE;
            }
        }
        else if ( WSAEISCONN == WSAGetLastError() )
        {
            return REGINA_ERR_SOCKET_ISCONN;
        }
        else if ( WSAEINPROGRESS == WSAGetLastError() ||
                  WSAEALREADY == WSAGetLastError() )
        {
            return REGINA_ERR_SOCKET_INPROGRESS;
        }
        else
        {
            do
            {
                const regina_log_level_t ll = REGINA_LL_ERROR;
                if ( regina_logger.ll <= ll )
                {
                    void *ctx = regina_logger.context;

                    regina_logger.log( ctx, (ll), "connect failed with error: %d!\n", WSAGetLastError() );
                }
            }
            while ( 0 );
        }

        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }

    return REGINA_ERR_NONE;
}

int tcp_util_send( socket_fd_t *fd, const char *buf,
                   unsigned int size, unsigned int *bytes_sent )
{
    int ret;
    WSABUF wsb;
    DWORD sent = 0;

    if ( !buf || !size || !bytes_sent || !fd )
        return REGINA_ERR_BADPARAM;

    wsb.buf = buf;
    wsb.len = size;
    ret = WSASend( fd->fd,
                   &wsb,
                   1,
                   &sent,
                   0,
                   NULL,
                   NULL );

    if ( NO_ERROR == ret )
    {
        *bytes_sent = (int)sent;

        return REGINA_ERR_NONE;
    }
    else if ( WSAEWOULDBLOCK == WSAGetLastError() )
    {
        *bytes_sent = 0;

        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_SOCKET_IO_ERROR;
}

int tcp_util_recv( socket_fd_t *fd, char *buf,
                   unsigned int size, unsigned int *bytes_read )
{
    int ret;
    WSABUF wsb;
    DWORD recvd = 0;
    DWORD flags = 0;

    if ( !buf || !size || !bytes_read || !fd )
        return REGINA_ERR_BADPARAM;

    wsb.buf = buf;
    wsb.len = size;
    ret = WSARecv( fd->fd,
                   &wsb,
                   1,
                   &recvd,
                   &flags,
                   NULL,
                   NULL );

    if ( NO_ERROR == ret )
    {
        if ( recvd > 0 )
        {
            *bytes_read = (int)recvd;

            return REGINA_ERR_NONE;
        }
        else
        {
            return REGINA_ERR_SOCKET_IO_EOF;
        }
    }
    else if ( WSAEWOULDBLOCK == WSAGetLastError() )
    {
        *bytes_read = 0;

        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_SOCKET_IO_ERROR;
}

void tcp_util_close( socket_fd_t *fd )
{
    if ( fd )
    {
        closesocket( fd->fd );
        regina_free( fd );
    }
}

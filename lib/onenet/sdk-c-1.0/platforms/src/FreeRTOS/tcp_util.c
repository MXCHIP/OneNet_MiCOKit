#define _GNU_SOURCE  /* for sighandler_t */

#include "tcp_util.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include "FreeRTOS.h"
#include "regina/log.h"
#define socklen_t int32_t

struct socket_fd_t {
    int fd;
};

struct ip_address_t {
    socklen_t len;
    char *addr;
	  unsigned int port;
};

static int tcp_util_ip_address_create(ip_address_t **address,
                                     const char *addr,
                                      socklen_t len,int port)
{
	  *address = (ip_address_t*)regina_malloc(sizeof(ip_address_t) + len+1);
    if (!*address)
		{
        return REGINA_ERR_NOMEM;
		}
		memset((char *)(*address),0,sizeof(ip_address_t) + len+1);
    (*address)->addr = (char *)(*address) + sizeof(ip_address_t);
    memcpy((*address)->addr, addr, len);
    (*address)->len = len;
		(*address)->port=port;
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

static void connect_alarm(int signo)
{
    /* just interrupt the connect() */
}

int tcp_util_open(socket_fd_t **fd, ip_address_t *address)
{
   int err;
	 *fd = (socket_fd_t*)regina_malloc(sizeof(socket_fd_t));
   if (!*fd) {
         return REGINA_ERR_NOMEM;
   }
	 (*fd)->fd=1;
	 connect_tcp(address->addr,address->port);
   return REGINA_ERR_SOCKET_ISCONN;
}

int tcp_util_send(socket_fd_t *fd, const char *buf,
                  unsigned int size, unsigned int *bytes_sent)
{
    int result;
		portENTER_CRITICAL();
		USART2_SendData(buf,size);
		*bytes_sent=size;
		vPortExitCritical();
    return REGINA_ERR_NONE;
}

int tcp_util_recv(socket_fd_t *fd, char *buf,
                  unsigned int size, unsigned int *bytes_read)
{
	
   int len_in,i;
		portENTER_CRITICAL();
	  len_in=USART2_GetRcvNum();
	  if(len_in==0)
		{
				*bytes_read=0;
				vPortExitCritical();
        return REGINA_ERR_NONE;
		}
		mDelay(100);
		len_in=USART2_GetRcvNum();
		if(len_in>0)
		{
			*bytes_read=len_in;
			USART2_GetRcvData(buf, len_in);
		}
			vPortExitCritical();
    return REGINA_ERR_NONE;
}

void tcp_util_close(socket_fd_t *fd)
{
    if (fd) {
        regina_free(fd);
    }
}
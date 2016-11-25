#include "tcp_util.h"
#include <string.h>
#include <stdio.h>
#include "regina/error.h"
#include "regina/allocator.h"
#include "regina/log.h"
#include "onenet_mico_platform.h"

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

// host only ip format
int tcp_util_getaddrinfo(const char *host, unsigned int port, int ipv6,
                         ip_address_t **addresses, unsigned int *addrcnt)
{
  int err = 0;
  char ipstr[16];
  err = onenet_mico_platform_tcp_dns((char *)host, ipstr, 16);
  if( err != 0 )
    return REGINA_ERR_RESOLVE_HOST_FAILED;
  
  *addrcnt = 1;
  return tcp_util_ip_address_create(addresses,ipstr,strlen(ipstr),port);
}

int tcp_util_open(socket_fd_t **fd, ip_address_t *address)
{
  int res = 0;
  if (!fd || !address)
    return REGINA_ERR_BADPARAM;
  
  if (NULL == *fd) {
    *fd = (socket_fd_t*)regina_malloc(sizeof(socket_fd_t));
    (*fd)->fd = -1;
    if (!*fd) {
      return REGINA_ERR_NOMEM;
    }
  }
    
    res = onenet_mico_platform_tcp_open(&((*fd)->fd), address->addr, address->port);
    if( res == ONENET_MICO_SOCKET_ERR ){
        return REGINA_ERR_SOCKET_ERROR;
    }else if( res == ONENET_MICO_CONNECT_ERR){
        return REGINA_ERR_SOCKET_CONNECT_ERROR;
    }
    return REGINA_ERR_SOCKET_ISCONN;
}

int tcp_util_send(socket_fd_t *fd, const char *buf,
                  unsigned int size, unsigned int *bytes_sent)
{
  int result = 0;
  
  if (!buf || !size || !bytes_sent || !fd)
    return REGINA_ERR_BADPARAM;
  
  result = onenet_mico_platform_tcp_send(fd->fd,(char *)buf, size, 0);
  
  if (result >= 0) {
    *bytes_sent = (int)result;
    return REGINA_ERR_NONE;
  }
  
  return REGINA_ERR_SOCKET_IO_ERROR;
}

int tcp_util_recv(socket_fd_t *fd, char *buf,
                  unsigned int size, unsigned int *bytes_read)
{
  int result = 0;
      
  if (!buf || !size || !bytes_read || !fd)
    return REGINA_ERR_BADPARAM;
  
  result = onenet_mico_platform_tcp_recv(fd->fd, buf, size, 0);
  
  if (result >= 0) {
    *bytes_read = (int)result;
    return REGINA_ERR_NONE;
  }
      
  return REGINA_ERR_SOCKET_IO_ERROR;
}

void tcp_util_close(socket_fd_t *fd)
{
  if (fd) {
      onenet_mico_platform_tcp_close(fd->fd);
    regina_free(fd);
  }
}


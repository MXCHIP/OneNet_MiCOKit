#ifndef __REGINA_PLATFORM_TCP_UTIL_H__
#define __REGINA_PLATFORM_TCP_UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef struct socket_fd_t socket_fd_t;
typedef struct ip_address_t ip_address_t;


/**
 * Translat an ANSI host name to some addresses.
 *
 * @param host the ANSI host name
 * @param port the port of the host
 * @param ipv6 if non-zeor only ipv6 addresses
 * @param addresses[OUT] save the addresses
 * @param addcnt[INOUT] Before the call, it should be initialized to the count
          of the addresses. Upon return, addcnt is updated to contain the
          actual count of the  translated addresses.
 * @return REGINA_ERR_NONE if succeeds.
 * @remark the ip_address_t objects are allocated on heap, use @see
 *         tcp_util_ip_address_destroy to destroy them when no use.
 */
int tcp_util_getaddrinfo(const char *host, unsigned int port, int ipv6,
                         ip_address_t **addresses, unsigned int *addcnt);
/**
 * Destroy the ip_address_t object.
 */
void tcp_util_ip_address_destroy(ip_address_t *address);
/**
 * Establishes a connection to a specified socket.
 *
 * @param fd save[OUT] the connection file discreptor.
 * @param address
 * @param timeout the time-out interval in seconds.
 * @return REGINA_ERR_NONE if succeeds
 */
void tcp_util_ip_address_destroy(ip_address_t *address);

/**
 * Establishes a connection to a specified socket, and set it to non blocking mode.
 *
 * @param fd [INOUT] the connection file discreptor.
 * @param address
 * @return 1. *fd is null, create the file discreptor and set it to non
 *         blocking mode succcessfully return REGINA_ERR_NONE;
 *         2. *fd is not null, check the connect status, if it is
 *         connecting to the server, return REGINA_ERR_SOCKET_INPROGRESS,
 *         if the connection has established, return REGINA_ERR_SOCKET_ISCONN,
 *         if some other socket error occurs, return REGINA_ERR_SOCKET_CONNECT_ERROR
 */
int tcp_util_open(socket_fd_t **fd, ip_address_t *address);
/**
 * Sends data on a connected socket.
 *
 * @param fd
 * @param buf points to a buffer containing the data to be transmitted.
 * @param size The size, in bytes, of the data in buffer pointed to by
 *        the buf parameter.
 * @param bytes_sent[OUT] save the total number of bytes sent.
 * @return REGINA_ERR_NONE if succeeds, otherwise REGINA_ERR_SOCKET_IO_ERROR
 */
int tcp_util_send(socket_fd_t *fd, const char *buf, unsigned int size,
                  unsigned int *bytes_sent);
/**
 * Receives data from a connected socket.
 *
 * @param fd
 * @param buf points to the buffer to receive the incoming data.
 * @param size The size, in bytes, of the buffer pointed to by the buf parameter.
 * @param bytes_read[OUT] save the total number of bytes received.
 * @return REGINA_ERR_NONE if succeeds, REGINA_ERR_SOCKET_IO_EOF if the connection
 *         is closed, REGINA_ERR_SOCKET_IO_ERROR if others.
 */
int tcp_util_recv(socket_fd_t *fd, char *buf, unsigned int size,
                  unsigned int *bytes_read);
/**
 * Close the connected socket and destroy the resource.
 */
void tcp_util_close(socket_fd_t *fd);


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_PLATFORM_TCP_UTIL_H__ */

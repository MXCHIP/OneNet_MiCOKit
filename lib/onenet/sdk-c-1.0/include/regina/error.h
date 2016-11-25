#ifndef __REGINA_ERROR_H__
#define __REGINA_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REGINA_ERR_NONE             0

/* General errors */
#define REGINA_ERR_NOMEM                   (-1)
#define REGINA_ERR_BADPARAM                (-2)
#define REGINA_ERR_READ_FAILED             (-3)
#define REGINA_ERR_WRITE_FAILED            (-4)
#define REGINA_ERR_NOT_FOUND               (-5)
#define REGINA_ERR_BAD_STATE               (-6)
#define REGINA_ERR_UNSUPPORTED             (-7)
#define REGINA_ERR_BAD_PROTOCOL_VERSION    (-8)
#define REGINA_ERR_TIMEOUT                 (-9)
#define REGINA_ERR_CREATE_THREAD_FAILED    (-10)
#define REGINA_ERR_RESOLVE_HOST_FAILED     (-11)
#define REGINA_ERR_NOAUTHORITY             (-12)
#define REGINA_ERR_PROTOCOL_ERROR          (-13)
#define REGINA_ERR_DATA_CORRUPTION         (-14)
#define REGINA_ERR_SYSTEM_ERROR            (-15)
#define REGINA_ERR_UNINITIALIZED           (-16)
#define REGINA_ERR_MULTI_INITTIALIZED      (-17)
#define REGINA_ERR_INTERNAL                (-18)
#define REGINA_ERR_NOSERVER                (-19)
#define REGINA_ERR_SERVER_UNUSABLE         (-20)
#define REGINA_ERR_REGISTER_FAILED         (-21)


#define REGINA_ERR_SOCKET_ERROR            (-101)
#define REGINA_ERR_SOCKET_CONNECT_ERROR    (-102)
#define REGINA_ERR_SOCKET_INVALID_FAMILY   (-103)
#define REGINA_ERR_SOCKET_IO_EOF           (-104)
#define REGINA_ERR_SOCKET_IO_ERROR         (-105)
#define REGINA_ERR_SOCKET_INPROGRESS       (-106)
#define REGINA_ERR_SOCKET_ISCONN           (-108)


#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* __REGINA_ERROR_H__ */

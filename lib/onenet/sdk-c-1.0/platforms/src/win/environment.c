#include "environment.h"
#include <WinSock2.h>
#include <fcntl.h>
#include <stdlib.h>

/* initialize some environment about current platform */
int initialize_environment( void )
{
    WSADATA wsa;
    int err = 0;

    err = WSAStartup( MAKEWORD(2,2), &wsa );
    if (0 != err)
    {
        return -1;
    }

    _set_fmode( _O_BINARY );

    return 0;
}

/* cleanup some environment abount current platform*/
void cleanup_environment( void )
{
    WSACleanup();
}
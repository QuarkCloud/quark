
#include "fsocket.h"
#include <stdlib.h>
#include <string.h>


LPFN_ACCEPTEX look_up_acceptex(SOCKET& s)
{
    LPFN_ACCEPTEX lpfn = NULL ;
    GUID guidAcceptEx = WSAID_ACCEPTEX ;
    DWORD bytes = 0 ;

    ::_imp_WSAIoctl(s , SIO_GET_EXTENSION_FUNCTION_POINTER ,&guidAcceptEx,sizeof(guidAcceptEx),
        &lpfn , sizeof(lpfn) , &bytes , NULL , NULL) ;

    return lpfn ;
}

void update_contex_acceptex(SOCKET& new_socket , SOCKET&listen_socket)
{
    ::_imp_setsockopt(new_socket , SOL_SOCKET , SO_UPDATE_ACCEPT_EX , (char *)&listen_socket , sizeof(listen_socket)) ;
}

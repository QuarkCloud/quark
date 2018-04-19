
#include "fsocket.h"
#include <stdlib.h>
#include <string.h>


void update_contex_acceptex(SOCKET& new_socket , SOCKET&listen_socket)
{
    ::_imp_setsockopt(new_socket , SOL_SOCKET , SO_UPDATE_ACCEPT_CONTEXT , (char *)&listen_socket , sizeof(listen_socket)) ;
}


bool sockopt_get_keepalive(SOCKET& s , bool &optval)
{
    BOOL val = FALSE ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , SOL_SOCKET , SO_KEEPALIVE , (char *)&val , &len) == 0)
    {
        optval = (val == TRUE) ;
        return true ;
    }
    return false ;
}

bool sockopt_set_keepalive(SOCKET& s , bool optval)
{
    BOOL val = (optval == true?TRUE:FALSE) ;
    return (::_imp_setsockopt(s , SOL_SOCKET , SO_KEEPALIVE , (const char *)&val , sizeof(val)) == 0) ;
}

bool sockopt_get_recv_buffer_size(SOCKET& s , int& optval)
{
    int val = 0 ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , SOL_SOCKET , SO_RCVBUF , (char *)&val , &len) == 0)
    {
        optval = val ;
        return true ;
    }
    return false ;
}

bool sockopt_set_recv_buffer_size(SOCKET& s , int optval)
{
    return (::_imp_setsockopt(s , SOL_SOCKET , SO_RCVBUF , (const char *)&optval , sizeof(optval)) == 0) ;
}

bool sockopt_get_send_buffer_size(SOCKET& s , int& optval)
{
    int val = 0 ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , SOL_SOCKET , SO_SNDBUF , (char *)&val , &len) == 0)
    {
        optval = val ;
        return true ;
    }
    return false ;
}

bool sockopt_set_send_buffer_size(SOCKET& s , int optval)
{
    return (::_imp_setsockopt(s , SOL_SOCKET , SO_RCVBUF , (const char *)&optval , sizeof(optval)) == 0) ;
}

bool sockopt_get_recv_timeout(SOCKET& s , DWORD& optval)
{
    DWORD val = 0 ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , SOL_SOCKET , SO_RCVTIMEO , (char *)&val , &len) == 0)
    {
        optval = val ;
        return true ;
    }
    return false ;
}

bool sockopt_set_recv_timeout(SOCKET& s , DWORD optval)
{
    return (::_imp_setsockopt(s , SOL_SOCKET , SO_RCVTIMEO , (const char *)&optval , sizeof(optval)) == 0) ;
}

bool sockopt_get_send_timeout(SOCKET& s , DWORD& optval)
{
    DWORD val = 0 ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , SOL_SOCKET , SO_SNDTIMEO , (char *)&val , &len) == 0)
    {
        optval = val ;
        return true ;
    }
    return false ;
}

bool sockopt_set_send_timeout(SOCKET& s , DWORD optval)
{
    return (::_imp_setsockopt(s , SOL_SOCKET , SO_SNDTIMEO , (const char *)&optval , sizeof(optval)) == 0) ;
}

bool socket_init(socket_t *& s) 
{
    socket_t * data = (socket_t *)::malloc(sizeof(socket_t)) ;
    if(data == NULL)
        return false ;

    ::memset(data , 0 , sizeof(socket_t)) ;

    data->locker = ::CreateMutex(NULL , FALSE , NULL) ;
    s = data ;
    return true ;
}

bool send_result_init(send_result_t *& result) 
{
    send_result_t * sender = (send_result_t *)::malloc(sizeof(send_result_t)) ;
    if(sender == NULL)
    {
        errno = ENOMEM ;
        return false ;
    }
    ::memset(sender , 0 , sizeof(send_result_t)) ;

    if(ring_buffer_init(&sender->ring_buffer , SNDBUFSIZE) == false)
    {
        ::free(sender) ;
        errno = ENOMEN ;
        return false ;
    }  
    sender->link.type = OVLP_OUTPUT ;

    return true ;
}

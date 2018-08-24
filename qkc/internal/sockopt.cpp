
#include "sockopt.h"
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


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

bool sockopt_get_nodelay(SOCKET&s , bool& optval)
{
    BOOL val = FALSE ;
    int len = sizeof(val) ;
    if(::_imp_getsockopt(s , IPPROTO_TCP , TCP_NODELAY , (char *)&val , &len) == 0)
    {
        optval = (val == TRUE) ;
        return true ;
    }
    return false ;    
}

bool sockopt_set_nodelay(SOCKET&s , bool optval)
{
    BOOL val = (optval==true?TRUE:FALSE) ;
    return (::_imp_setsockopt(s , IPPROTO_TCP , TCP_NODELAY , (char *)&val , sizeof(val)) == 0) ;
}

bool socket_get_readable_size(SOCKET& s , DWORD& size)
{
    u_long value = 0 ;
    if(::_imp_ioctlsocket(s , FIONREAD , &value) != 0)
        return false ;

    size = value ;
    return true ;
}

bool socket_set_nonblock(SOCKET& s , bool enable)
{
    u_long value = (enable==false?0:1) ;
    return (::_imp_ioctlsocket(s , FIONBIO , &value) == 0) ;
}

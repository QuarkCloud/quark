
#include "fsocket.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


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
        errno = ENOMEM ;
        return false ;
    }  
    sender->link.type = OVLP_OUTPUT ;

    return true ;
}

bool recv_result_init(recv_result_t *& result) 
{
    recv_result_t * receiver = (recv_result_t *)::malloc(sizeof(recv_result_t)) ;
    if(receiver == NULL)
    {
        errno = ENOMEM ;
        return false ;
    }
    ::memset(receiver , 0 , sizeof(recv_result_t)) ;
    receiver->link.type = OVLP_INPUT ;
    return true ;
}

bool socket_send(send_result_t * result , int flags) 
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    char * wbuf = NULL ;
    size_t wsize = 0 ;
    if(ring_buffer_refer_stream(&result->ring_buffer , wbuf , wsize) == false || wbuf == NULL || wsize == 0)
    {
        socket_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    result->data.buf = wbuf ;
    result->data.len = wsize ;

    DWORD sent_bytes = 0 ;
    int status = ::_imp_WSASend(result->link.owner->socket  , &result->data , 1 , &sent_bytes , 0 , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            socket_ovlp_unlock(&result->link) ;
            return false ;
        }
    }

    return true ;
}

bool socket_sendto(send_result_t * result , int flags , const struct sockaddr * addr , int addr_len)
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    char * wbuf = NULL ;
    size_t wsize = 0 ;
    if(ring_buffer_refer_message(&result->ring_buffer , wbuf , wsize) == false || wbuf == NULL || wsize == 0)
    {
        socket_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = wbuf ;
    result->data.len = wsize ;

    DWORD sent_bytes = 0 ;
    int status = ::_imp_WSASendTo(result->link.owner->socket , &result->data , 1 , 
        &sent_bytes , 0 , addr , addr_len , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            socket_ovlp_unlock(&result->link) ;
            return false ;
        }
    }

    return true ;
}

bool socket_ovlp_lock(socket_ovlp_t * ovlp) 
{
    return (::InterlockedCompareExchange(&ovlp->counter , 1 , 0) == 0) ;
}

bool socket_ovlp_unlock(socket_ovlp_t * ovlp)
{
    return (::InterlockedCompareExchange(&ovlp->counter , 0 , 1) == 1) ;
}

bool socket_start_recv(recv_result_t * result) 
{
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = NULL ;
    result->data.len = 0 ;

    DWORD recv_bytes = 0 , flag = 0 ;
    int status = ::_imp_WSARecv(result->link.owner->socket , &result->data , 1 ,  &recv_bytes , &flag , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            socket_ovlp_unlock(&result->link) ;
            return false ;
        }    
    }

    return true ;
}

bool socket_start_recvfrom(recv_result_t * result , int flags , struct sockaddr * addr , int * addr_len)
{
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = NULL ;
    result->data.len = 0 ;
    DWORD recv_bytes = 0 , flag = 0 ;
    int status = ::_imp_WSARecvFrom(result->link.owner->socket , &result->data , 1 , &recv_bytes , &flag ,
        addr , (LPINT)addr_len , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            socket_ovlp_unlock(&result->link) ;
            return false ;
        }    
    }

    return true ;
}

bool socket_accept(accept_result_t * result , SOCKET& new_socket , struct sockaddr * local , struct sockaddr * remote)
{
    update_contex_acceptex(result->insocket , result->link.owner->socket) ;

    struct sockaddr * laddr = NULL ;
    ::GetAcceptExSockaddrs(result->address , ACCEPT_ADDRESS_SIZE , sizeof(struct sockaddr) + 16 , sizeof(struct sockaddr) + 16 , local

}

bool socket_start_accept(accept_result_t * result)
{

}


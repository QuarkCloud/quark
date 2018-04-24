
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

bool socket_init(socket_t * s) ;
bool socket_final(socket_t * s) ;

socket_t * socket_new() 
{
    socket_t * data = (socket_t *)::malloc(sizeof(socket_t)) ;
    if(data == NULL)
        return NULL ;

    if(socket_init(data) == true)
        return data ;

    ::free(data) ;
    return NULL ;
}

bool socket_init(socket_t * s) 
{
    ::memset(s , 0 , sizeof(socket_t)) ;
    s->locker = ::CreateMutex(NULL , FALSE , NULL) ;
    if(s->locker == INVALID_HANDLE_VALUE)
        return false ;
    return true ;
}

void socket_free(socket_t * s)
{
    socket_final(s) ;
    if(s != NULL)
        ::free(s) ;
}

bool socket_final(socket_t * s)
{
    if(s == NULL)
        return false ;

    if(s->locker == INVALID_HANDLE_VALUE)
        return false ;

    if(::WaitForSingleObject(s->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    if(s->acceptor != NULL)
    {
        accept_result_final(s->acceptor) ;
        s->acceptor = NULL ;
    }

    if(s->sender != NULL)
    {
        send_result_final(s->sender) ;
        s->sender = NULL ;
    }

    if(s->receiver != NULL)
    {
        recv_result_final(s->receiver) ;
        s->receiver = NULL ;
    }

    ::_imp_closesocket(s->socket) ;
    s->socket = INVALID_SOCKET ;
    ::ReleaseMutex(s->locker) ;

    return true ;
}

bool send_result_init(send_result_t * result) ;
bool send_result_final(send_result_t * result) ;
send_result_t * send_result_new()
{
    send_result_t * result = (send_result_t *)::malloc(sizeof(send_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM;
        return NULL ;
    }

    if(send_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void send_result_free(send_result_t * result)
{
    send_result_final(result) ;
    if(result != NULL)
        ::free(result) ;
}

bool send_result_init(send_result_t * result)
{
    if(result == NULL)
        return false ;

    ::memset(result , 0 , sizeof(send_result_t)) ;

    if(ring_buffer_init(&result->ring_buffer , SNDBUFSIZE) == false)
    {
        errno = ENOMEM ;
        return false ;
    }  
    result->link.type = OVLP_OUTPUT ;
    return true ;
}

bool send_result_final(send_result_t * result)
{
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    ::_imp_WSAGetOverlappedResult(result->link.owner->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    socket_ovlp_unlock(&result->link) ;
    return true ;
}

bool recv_result_init(recv_result_t * result) ;
bool recv_result_final(recv_result_t * result) ;

recv_result_t * recv_result_new()
{
    recv_result_t * result = (recv_result_t *)::malloc(sizeof(recv_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(recv_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void recv_result_free(recv_result_t * result)
{
    if(result == NULL)
        return ;

    recv_result_final(result) ;
    ::free(result) ;
}

bool recv_result_init(recv_result_t * result) 
{
    ::memset(result , 0 , sizeof(recv_result_t)) ;
    result->link.type = OVLP_INPUT ;
    return true ;
}

bool recv_result_final(recv_result_t * result) 
{
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    ::_imp_WSAGetOverlappedResult(result->link.owner->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    socket_ovlp_unlock(&result->link) ;
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

int socket_ovlp_counter(socket_ovlp_t * ovlp) 
{
    return (int)::InterlockedCompareExchange(&ovlp->counter , 0 , 0) ;
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

    struct sockaddr * laddr = NULL , * raddr = NULL ;
    INT lsize = 0 , rsize = 0 ;
    ::_imp_GetAcceptExSockaddrs(result->address , ACCEPT_ADDRESS_SIZE , sizeof(struct sockaddr) + 16 , sizeof(struct sockaddr) + 16 ,
        &laddr , &lsize , &raddr , &rsize) ;

    if(local != NULL && laddr != NULL)
        ::memcpy(local , laddr , sizeof(struct sockaddr)) ;
    if(remote != NULL && raddr != NULL)
        ::memcpy(remote , raddr , sizeof(struct sockaddr)) ;

    new_socket = result->insocket ;
    result->bytes = 0 ;
    result->insocket = INVALID_SOCKET ;

    socket_ovlp_unlock(&result->link) ;
    return true ;
}

bool socket_start_accept(accept_result_t * result)
{
    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    SOCKET s = ::_imp_socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(s == INVALID_SOCKET)
    {
        socket_ovlp_unlock(&result->link) ;
        return false ;
    }

    result->insocket = s ;
    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    DWORD bytes_received = 0 ;
    if(::_imp_AcceptEx(result->link.owner->socket , result->insocket , result->address , ACCEPT_ADDRESS_SIZE , sizeof(struct sockaddr) + 16 , 
        sizeof(struct sockaddr) + 16 , &bytes_received , &result->link.ovlp) == FALSE)
    {
        socket_ovlp_unlock(&result->link) ;
        return false ;
    }

    return true ;
}

bool accept_result_init(accept_result_t * result) ;
bool accept_result_final(accept_result_t * result) ;
accept_result_t * accept_result_new()
{
    accept_result_t * result = (accept_result_t *)::malloc(sizeof(accept_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(accept_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void accept_result_free(accept_result_t * result)
{
    if(result == NULL)
        return ;

    accept_result_final(result) ;
    ::free(result) ;
}

bool accept_result_init(accept_result_t * result)
{
    ::memset(result , 0 , sizeof(accept_result_t)) ;
    result->link.type = OVLP_ACCEPT ;
    return true ;
}

bool accept_result_final(accept_result_t * result)
{
    if(result == NULL)
        return false ;

    if(socket_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    ::_imp_WSAGetOverlappedResult(result->link.owner->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    socket_ovlp_unlock(&result->link) ;

    return true ;
}


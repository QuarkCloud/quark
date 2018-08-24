
#include "fsocket.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

void socket_callback(socket_t *s , int evt , int result) 
{
    if(s == NULL || s->callback == NULL)
        return ;

    socket_callback_t callback = s->callback ;
    callback(s , evt , result) ;
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

    socket_callback(s , kBeforeSocketClose , 0) ;

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

    ::_imp_shutdown(s->socket , SHUT_RDWR) ;
    ::_imp_closesocket(s->socket) ;
    s->socket = INVALID_SOCKET ;
    ::ReleaseMutex(s->locker) ;
    ::CloseHandle(s->locker) ;
    s->locker = INVALID_HANDLE_VALUE ;

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
    result->link.type = OVLP_SOCKET_OUTPUT ;
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
    result->link.type = OVLP_SOCKET_INPUT ;
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
            socket_callback(result->link.owner , kSocketSend , error) ;
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
            socket_callback(result->link.owner , kSocketSendTo , error) ;
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
            socket_callback(result->link.owner , kSocketRecv , error) ;
            return false ;
        }
        else
        {
            socket_callback(result->link.owner , kSocketRecv , WSAEWOULDBLOCK) ;
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
            socket_callback(result->link.owner , kSocketRecvFrom , error) ;
            return false ;
        }
        else
        {
            socket_callback(result->link.owner , kSocketRecvFrom , WSAEWOULDBLOCK) ;
        }
    }

    return true ;
}


void update_contex_acceptex(SOCKET& new_socket , SOCKET&listen_socket)
{
    ::_imp_setsockopt(new_socket , SOL_SOCKET , SO_UPDATE_ACCEPT_CONTEXT , (char *)&listen_socket , sizeof(listen_socket)) ;
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
    result->link.type = OVLP_SOCKET_ACCEPT ;
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


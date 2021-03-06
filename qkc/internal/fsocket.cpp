
#include "fsocket.h"
#include "iocp_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


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

    iocp_item_t * item = (iocp_item_t *)s->addition ;
    if(item != NULL)
        iocp_socket_callback(item , IOCP_EVENT_CLOSE , 0) ;

    if(::WaitForSingleObject(s->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    if(s->acceptor != NULL)
    {
		socket_accept_result_final(s->acceptor) ;
        s->acceptor = NULL ;
    }

    if(s->sender != NULL)
    {
		socket_send_result_final(s->sender) ;
        s->sender = NULL ;
    }

    if(s->receiver != NULL)
    {
		socket_recv_result_final(s->receiver) ;
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

bool socket_send_result_init(socket_send_result_t * result) ;
bool socket_send_result_final(socket_send_result_t * result) ;
socket_send_result_t * socket_send_result_new()
{
	socket_send_result_t * result = (socket_send_result_t *)::malloc(sizeof(socket_send_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM;
        return NULL ;
    }

    if(socket_send_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void socket_send_result_free(socket_send_result_t * result)
{
	socket_send_result_final(result) ;
    if(result != NULL)
        ::free(result) ;
}

bool socket_send_result_init(socket_send_result_t * result)
{
    if(result == NULL)
        return false ;

    ::memset(result , 0 , sizeof(socket_send_result_t)) ;

    if(ring_buffer_init(&result->ring_buffer , SNDBUFSIZE) == false)
    {
        errno = ENOMEM ;
        return false ;
    }  
    result->link.type = OVLP_OUTPUT ;
    return true ;
}

bool socket_send_result_final(socket_send_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    iocp_item_t * item = (iocp_item_t *)result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;

    ::_imp_WSAGetOverlappedResult(s->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    iocp_ovlp_unlock(&result->link) ;
    return true ;
}

bool socket_recv_result_init(socket_recv_result_t * result) ;
bool socket_recv_result_final(socket_recv_result_t * result) ;

socket_recv_result_t * socket_recv_result_new()
{
	socket_recv_result_t * result = (socket_recv_result_t *)::malloc(sizeof(socket_recv_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(socket_recv_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void socket_recv_result_free(socket_recv_result_t * result)
{
    if(result == NULL)
        return ;

	socket_recv_result_final(result) ;
    ::free(result) ;
}

bool socket_recv_result_init(socket_recv_result_t * result)
{
    ::memset(result , 0 , sizeof(socket_recv_result_t)) ;
    result->link.type = OVLP_INPUT ;
    return true ;
}

bool socket_recv_result_final(socket_recv_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    iocp_item_t * item = (iocp_item_t *)result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;

    socket_t * s = (socket_t *)item->addition ;
    if(s->socket == NULL)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    ::_imp_WSAGetOverlappedResult(s->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    iocp_ovlp_unlock(&result->link) ;
    return true ;
}

bool socket_send(socket_send_result_t * result , int flags)
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    char * wbuf = NULL ;
    size_t wsize = 0 ;
    if(ring_buffer_refer_stream(&result->ring_buffer , wbuf , wsize) == false || wbuf == NULL || wsize == 0)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;

    result->data.buf = wbuf ;
    result->data.len = wsize ;
    DWORD sent_bytes = 0 ;

    iocp_item_t * item = (iocp_item_t *)result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;
    if(s == NULL)
        return false ;

    int status = ::_imp_WSASend(s->socket  , &result->data , 1 , &sent_bytes , 0 , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_socket_callback(item , IOCP_EVENT_WRITE , error) ;
            return false ;
        }
    }

    return true ;
}

bool socket_sendto(socket_send_result_t * result , int flags , const struct sockaddr * addr , int addr_len)
{
    //数据已经准备好了，需要判断下，是否在发送中。如果已经在发送中，则退出
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    char * wbuf = NULL ;
    size_t wsize = 0 ;
    if(ring_buffer_refer_message(&result->ring_buffer , wbuf , wsize) == false || wbuf == NULL || wsize == 0)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = wbuf ;
    result->data.len = wsize ;

    DWORD sent_bytes = 0 ;

    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition)
        return false ;
    socket_t * s = (socket_t *)item->addition ;
    
    int status = ::_imp_WSASendTo(s->socket , &result->data , 1 , 
        &sent_bytes , 0 , addr , addr_len , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_socket_callback(item , IOCP_EVENT_WRITE , error) ;
            return false ;
        }
    }

    return true ;
}

bool socket_start_recv(socket_recv_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = NULL ;
    result->data.len = 0 ;

    DWORD recv_bytes = 0 , flag = 0 ;

    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;

    int status = ::_imp_WSARecv(s->socket , &result->data , 1 ,  &recv_bytes , &flag , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_socket_callback(result->link.owner , IOCP_EVENT_READ , error) ;
            return false ;
        }
        else
        {
            iocp_socket_callback(result->link.owner , IOCP_EVENT_READ , WSAEWOULDBLOCK) ;
        }
    }

    return true ;
}

bool socket_start_recvfrom(socket_recv_result_t * result , int flags , struct sockaddr * addr , int * addr_len)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    result->data.buf = NULL ;
    result->data.len = 0 ;
    DWORD recv_bytes = 0 , flag = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;
    int status = ::_imp_WSARecvFrom(s->socket , &result->data , 1 , &recv_bytes , &flag ,
        addr , (LPINT)addr_len , &result->link.ovlp , NULL) ;
    if(status != 0)
    {
        int error = ::_imp_WSAGetLastError() ;
        if(error != WSAEWOULDBLOCK)
        {
            result->link.status = error ;
            iocp_ovlp_unlock(&result->link) ;
            iocp_socket_callback(result->link.owner , IOCP_EVENT_READ , error) ;
            return false ;
        }
        else
        {
            iocp_socket_callback(result->link.owner , IOCP_EVENT_READ , WSAEWOULDBLOCK) ;
        }
    }

    return true ;
}


void update_contex_acceptex(SOCKET& new_socket , SOCKET&listen_socket)
{
    ::_imp_setsockopt(new_socket , SOL_SOCKET , SO_UPDATE_ACCEPT_CONTEXT , (char *)&listen_socket , sizeof(listen_socket)) ;
}

bool socket_accept(socket_accept_result_t * result , SOCKET& new_socket , struct sockaddr * local , struct sockaddr * remote)
{
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;
    update_contex_acceptex(result->insocket , s->socket) ;

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

    iocp_ovlp_unlock(&result->link) ;
    return true ;
}

bool socket_start_accept(socket_accept_result_t * result)
{
    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    SOCKET s = ::_imp_socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(s == INVALID_SOCKET)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    result->insocket = s ;
    ::memset(&result->link.ovlp , 0 , sizeof(result->link.ovlp)) ;
    DWORD bytes_received = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * sk = (socket_t *)item->addition ;

    if(::_imp_AcceptEx(sk->socket , result->insocket , result->address , ACCEPT_ADDRESS_SIZE , sizeof(struct sockaddr) + 16 , 
        sizeof(struct sockaddr) + 16 , &bytes_received , &result->link.ovlp) == FALSE)
    {
        iocp_ovlp_unlock(&result->link) ;
        return false ;
    }

    return true ;
}

bool socket_accept_result_init(socket_accept_result_t * result) ;
bool socket_accept_result_final(socket_accept_result_t * result) ;
socket_accept_result_t * socket_accept_result_new()
{
	socket_accept_result_t * result = (socket_accept_result_t *)::malloc(sizeof(socket_accept_result_t)) ;
    if(result == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }

    if(socket_accept_result_init(result) == false)
    {
        ::free(result) ;
        return NULL ;
    }

    return result ;
}

void socket_accept_result_free(socket_accept_result_t * result)
{
    if(result == NULL)
        return ;

	socket_accept_result_final(result) ;
    ::free(result) ;
}

bool socket_accept_result_init(socket_accept_result_t * result)
{
    ::memset(result , 0 , sizeof(socket_accept_result_t)) ;
    result->link.type = OVLP_INPUT ;
    return true ;
}

bool socket_accept_result_final(socket_accept_result_t * result)
{
    if(result == NULL)
        return false ;

    if(iocp_ovlp_lock(&result->link) == false)
        return false ;

    DWORD bytes_transfer = 0 , flags = 0 ;
    iocp_item_t * item = result->link.owner ;
    if(item == NULL || item->addition == NULL)
        return false ;
    socket_t * s = (socket_t *)item->addition ;

    ::_imp_WSAGetOverlappedResult(s->socket , &result->link.ovlp , & bytes_transfer , TRUE , &flags) ;
    iocp_ovlp_unlock(&result->link) ;

    return true ;
}

int iocp_socket_callback(iocp_item_t * item , int evt , int result)
{
    if(item == NULL || item->addition == NULL)
        return -1 ;

    socket_t * s = (socket_t *)item->addition ;
    uint32_t events = item->data.events ;
    int old_occur = item->occur , new_occur = item->occur;

    if(evt == IOCP_EVENT_OPEN)
        iocp_process_event(events & EPOLLERR , result , new_occur) ;
    else if(evt == IOCP_EVENT_WRITE)
        iocp_process_event(events & EPOLLOUT , result , new_occur) ;
    else if(evt == IOCP_EVENT_READ)
        iocp_process_event(events & EPOLLIN , result , new_occur) ;
    else if(evt == IOCP_EVENT_CLOSE)
        new_occur = 0 ;
    item->occur = new_occur ;

    if((events & EPOLLET) != 0)
    {
        //边缘触发，只在状态翻转时，执行操作
        if(old_occur == 0 && new_occur != 0)
            iocp_mgr_item_ready(item->owner , item) ;
        else if(old_occur != 0 && new_occur == 0)
            iocp_mgr_item_unready(item->owner , item) ;
    }
    else
    {
        if(new_occur != 0)
            iocp_mgr_item_ready(item->owner , item) ;
        else
            iocp_mgr_item_unready(item->owner , item) ;
    }

    return 0 ;
}

void iocp_socket_free(iocp_item_t * item)
{
    if(item != NULL && item->addition != NULL)
    {
        socket_t * s = (socket_t *)item->addition ;
        item->addition = NULL ;
        socket_free(s) ;
    }
}

SOCKET socket_from_ovlp(iocp_ovlp_t * ovlp) 
{
    if(ovlp == NULL || ovlp->owner == NULL)
        return 0 ;

    socket_t * s = (socket_t *)ovlp->owner->addition ;
    if(s == NULL)
        return 0 ;
    else
        return s->socket ;
}


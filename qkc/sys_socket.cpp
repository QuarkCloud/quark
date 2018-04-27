
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <winsock2.h>
#include <sys/socket.h>
#include <wintf/wobj.h>
#include "inner/fsocket.h"

int socket(int domain , int type , int protocol)
{
    SOCKET handle = _imp_socket(domain , type , protocol) ;
    if(handle == INVALID_SOCKET)
    {
        DWORD error_code = ::GetLastError() ;
        return -1 ;
    }

    socket_t * s = NULL ;
    if((s = socket_new()) == NULL)
    {
        ::_imp_closesocket(handle) ;
        errno = ENOMEM ;
        return -1 ;
    }

    s->socket = handle ;
    s->type = type ;

    return wobj_set(WOBJ_SOCK , (HANDLE)handle , s) ;
}

int bind(int fd , const struct sockaddr * addr , socklen_t len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->handle == NULL)
        return -1 ;

    socket_t * s = (socket_t *)obj->addition ;
    if(s == NULL)
        return -1 ;
    if(s->stage != SOCKET_STAGE_VOID)
        return -1 ;
    s->stage = SOCKET_STAGE_BIND ;

    return ::_imp_bind(s->socket , addr , len) ;
}

int getsockname(int fd , struct sockaddr * addr , socklen_t * len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->handle == NULL)
        return -1 ;

    return ::_imp_getsockname((SOCKET)obj->handle , addr , (int *)len) ;
}

int connect(int fd , const struct sockaddr * addr , socklen_t len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->handle == NULL)
        return -1 ;

    socket_t * s = (socket_t *)obj->addition ;
    if(s == NULL)
        return -1 ;
    if(s->stage != SOCKET_STAGE_VOID)
        return -1 ;
    s->stage = SOCKET_STAGE_CONNECT ;

    int result = ::_imp_connect(s->socket , addr , (int)len) ;
    socket_callback(s , kSocketConnect , 0) ;
    return result ;
}

int getpeername(int fd , struct sockaddr * addr , socklen_t * len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->handle == NULL)
        return -1 ;

    return ::_imp_getpeername((SOCKET)obj->handle , addr , (int *)len) ;
}

ssize_t send(int fd , const void * buf , size_t n , int flags)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;
    if(data->noblock == 0)
        return ::_imp_send((SOCKET)obj->handle , (const char *)buf , n , flags) ;

    send_result_t * sender = data->sender ;
    if(sender == NULL)
    {
        if((sender = send_result_new()) == NULL)
            return -1 ;

        sender->link.owner = data ;
        data->sender = sender ;
    }

    //1、先行拷贝到环形缓冲区中
    size_t copy_size = ring_buffer_write_stream(&sender->ring_buffer , buf , n) ;

    //2、因为数据已经准备好了，准备发送
    socket_send(sender , flags) ;

    return copy_size ;
}

ssize_t recv(int fd , void *buf , size_t n , int flags)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;  
    int recv_size = ::_imp_recv((SOCKET)obj->handle , (char *)buf , n , flags) ;
    if(data->noblock == 0)
        return recv_size ;

    recv_result_t * receiver = data->receiver ;
    if(receiver == NULL)
    {
        if((receiver = recv_result_new()) == NULL)
            return -1 ;
        receiver->link.owner = data ;
        data->receiver = receiver ;
    }

    socket_start_recv(receiver) ;
    return recv_size ;
}

ssize_t sendto(int fd , const void *buf , size_t n , int flags , const struct sockaddr * addr , socklen_t addr_len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;
    if(data->noblock == 0)
        return ::_imp_send((SOCKET)obj->handle , (const char *)buf , n , flags) ;

    send_result_t * sender = data->sender ;
    if(sender == NULL)
    {
        if((sender = send_result_new()) == NULL)
            return -1 ;

        sender->link.owner = data ;
        data->sender = sender ;
    }

    //1、先行拷贝到环形缓冲区中
    size_t copy_size = ring_buffer_write_message(&sender->ring_buffer , buf , n) ;

    //2、因为数据已经准备好了，准备发送
    socket_sendto(sender , flags , addr , addr_len) ;

    return copy_size ;
}

ssize_t recvfrom(int fd , void * buf , size_t n , int flags , struct sockaddr * addr , socklen_t * addr_len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;  
    int recv_size = ::_imp_recvfrom(data->socket , (char *)buf , n , flags , addr , (int *)addr_len) ;
    if(data->noblock == 0)
        return recv_size ;

    recv_result_t * receiver = data->receiver ;
    if(receiver == NULL)
    {
        if((receiver = recv_result_new()) == NULL)
            return -1 ;
        receiver->link.owner = data ;
        data->receiver = receiver ;
    }

    socket_start_recvfrom(receiver , flags , addr , (int *)addr_len) ;
    return recv_size ;
}

int getsockopt(int fd , int level , int optname , void * optval , socklen_t * optlen)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    return ::_imp_getsockopt((SOCKET)obj->handle , level , optname , (char *)optval , (int *)optlen) ;
}

int setsockopt(int fd , int level , int optname , const void *optval , socklen_t optlen)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    return ::_imp_setsockopt((SOCKET)obj->handle , level , optname , (const char *)optval , (int)optlen) ;
}

int listen(int fd , int n)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;
    if(data->stage != SOCKET_STAGE_BIND)
        return -1 ;
    data->stage = SOCKET_STAGE_LISTEN ;
    accept_result_t * result = accept_result_new() ;
    if(result != NULL)
    {
        result->link.owner = data ;
        data->acceptor = result ;
    }

    return ::_imp_listen((SOCKET)obj->handle , n) ;
}

int accept(int fd , struct sockaddr * addr , socklen_t * addr_len)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;
    if(data->noblock == 0)
    {
        SOCKET handle = ::_imp_accept(data->socket , addr , (int *)addr_len) ;
        if(handle == INVALID_SOCKET)
            return -1 ;

        socket_t * s = NULL ;
        if((s = socket_new()) == NULL)
        {
            ::_imp_closesocket(handle) ;
            errno = ENOMEM ;
            return -1 ;
        }

        s->socket = handle ;
        s->type = data->type ;
        s->stage = SOCKET_STAGE_CONNECT ;
        return wobj_set(WOBJ_SOCK , (HANDLE)handle , s) ;
    }

    //异步模式的话，需要更新状态，accept_result的状态，同时启动新的accept
    accept_result_t * acceptor = data->acceptor ;
    if(acceptor == NULL)
        return -1 ;

    SOCKET new_socket = INVALID_SOCKET ;
    if(socket_accept(acceptor , new_socket , NULL , addr) == false || new_socket == INVALID_SOCKET)
        return -1 ;

    int result = -1 ;
    socket_t * s = socket_new() ;
    if(s != NULL)
    {
        s->socket = new_socket ;
        s->type = data->type ;
        s->stage = SOCKET_STAGE_CONNECT ;
        result = wobj_set(WOBJ_SOCK , (HANDLE)new_socket , s) ;
    }
    else
    {
        ::_imp_closesocket(new_socket) ;
    }

    socket_start_accept(acceptor) ;
    return result ;
}

int shutdown(int fd , int how)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;
    return ::_imp_shutdown(data->socket , how) ;
}

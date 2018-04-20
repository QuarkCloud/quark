
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
    if(socket_init(s) == false)
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

    return ::_imp_connect(s->socket , addr , (int)len) ;
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
        if(send_result_init(sender) == false)
            return -1 ;

        sender->link.owner = data ;
        data->sender = sender ;
    }

    //1、先行拷贝到环形缓冲区中
    size_t copy_size = ring_buffer_write_stream(&sender->ring_buffer , buf , size) ;

    //2、因为数据已经准备好了，准备发送
    socket_send(&sender , flags) ;

    return copy_size ;
}

ssize_t recv(int fd , void *buf , size_t n , int flags)
{
    wobj_t * obj = wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    socket_t * data = (socket_t *)obj->addition ;  
    int recv_size ::_imp_recv((SOCKET)obj->handle , (char *)buf , n , flags) ;
    if(data->noblock == 0)
        return recv_size ;

    recv_result_t * receiver = data->receiver ;
    if(receiver == NULL)
    {
        if(recv_result_init(&receiver) == false || receiver == NULL)
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
        if(send_result_init(sender) == false)
            return -1 ;

        sender->link.owner = data ;
        data->sender = sender ;
    }

    //1、先行拷贝到环形缓冲区中
    size_t copy_size = ring_buffer_write_message(&sender->ring_buffer , buf , size) ;

    //2、因为数据已经准备好了，准备发送
    socket_send(&sender , flags) ;

    return copy_size ;

    /**
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_sendto(handle , (const char *)buf , n , flags , addr , addr_len) ;
    */
}

ssize_t recvfrom(int fd , void * buf , size_t n , int flags , struct sockaddr * addr , socklen_t * addr_len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_recvfrom(handle , (char *)buf , n , flags , addr , (int *)addr_len) ;
}

int getsockopt(int fd , int level , int optname , void * optval , socklen_t * optlen)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_getsockopt(handle , level , optname , (char *)optval , (int *)optlen) ;
}

int setsockopt(int fd , int level , int optname , const void *optval , socklen_t optlen)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_setsockopt(handle , level , optname , (const char *)optval , (int)optlen) ;
}

int listen(int fd , int n)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_listen(handle , n) ;
}

int accept(int fd , struct sockaddr * addr , socklen_t * addr_len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_accept(handle , addr , (int *)addr_len) ;
}

int shutdown(int fd , int how)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_shutdown(handle , how) ;
}

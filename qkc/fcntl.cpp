
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <wintf/wcrt.h>
#include <wintf/wobj.h>
#include "internal/fsocket.h"

/**
    F_SETFD/F_GETFD只对FD_CLOEXEC有效，判断是否在exec()之后，关闭文件
    F_SETFL只允许O_APPEND、O_NONBLOCK和O_ASYNC，其他标识位不受影响
*/
int fcntl (int fd, int cmd , ...)
{
    if(cmd == F_SETFL)
    {
        int arg = 0 ;
        va_list args ;
        va_start(args , cmd) ;
        arg = va_arg(args , int) ;
        va_end(args) ;

        if((arg & O_NONBLOCK) != O_NONBLOCK)
        {
            errno = ENOSYS ;
            return -1 ;
        }

        wobj_t * obj = wobj_get(fd) ;
        if(obj == NULL)
        {
            errno = EINVAL ;
            return -1 ;
        }

        wobj_type type = obj->type ;
        if(type != WOBJ_SOCK)
        {
            errno = ENOSYS ;
            return -1 ;
        }

        socket_t * sock = (socket_t *)obj->addition ;
        if(sock == NULL)
        {
            errno = EBADFD;
            return -1 ;
        }

        u_long val = 1 ;
        if(::_imp_ioctlsocket(sock->socket , FIONBIO , &val) == 0)
            sock->noblock = 1 ;
        else
            sock->noblock = 0 ;

        return 0 ;
    }
    else if(cmd == F_DUPFD)
    {
        if(_get_osfhandle(fd) == 0) 
        {
            errno = EBADFD ;
            return -1 ;
        }
        return dup(fd) ;
    }
    else if(cmd == F_GETFL)
    {
        wobj_t * obj = wobj_get(fd) ;
        if(obj == NULL)
        {
            errno = EINVAL ;
            return -1 ;
        }

        wobj_type type = obj->type ;
        if(type != WOBJ_SOCK)
        {
            errno = ENOSYS ;
            return -1 ;
        }

        socket_t * sock = (socket_t *)obj->addition ;
        if(sock == NULL)
        {
            errno = EBADFD;
            return -1 ;
        }

        if(sock->noblock == 1)
            return O_NONBLOCK ;
        else
            return 0 ; 
    }
    else
    {
        errno = ENOSYS ;
        return -1 ;
    }
}


int creat(const char * file , mode_t mode) 
{
    return ::_creat(file , mode) ;
}

int open(const char * file , int flag,mode_t mode)
{
    return ::_open(file , flag , mode) ;
}



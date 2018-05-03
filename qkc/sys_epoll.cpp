
#include <sys/epoll.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <winsock2.h>
#include <errno.h>
#include "internal/rlist.h"
#include "internal/rbtree.h"
#include "internal/fsocket.h"

int epoll_create (int size)
{
    HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(handle == INVALID_HANDLE_VALUE)
    {
        errno = EIO ;
        return -1 ;
    }

    return wobj_set(WOBJ_IOCP , handle , NULL) ;
}

bool iocp_add(HANDLE iocp , SOCKET fd , struct epoll_event * ev) ;
bool iocp_del(HANDLE iocp , SOCKET fd) ;
bool iocp_mod(HANDLE iocp , SOCKET fd , struct epoll_event * ev) ;

int epoll_ctl (int epfd, int op, int fd, struct epoll_event * ev)
{
    wobj_t * eobj = ::wobj_get(epfd) ;
    if(eobj == NULL || eobj->type != WOBJ_IOCP)
        return -1 ;

    wobj_t * fobj = ::wobj_get(fd) ;
    if(fobj == NULL || eobj->type != WOBJ_SOCK)
        return -1 ;

    bool result = false ;
    if(op == EPOLL_CTL_ADD)
        result = iocp_add(eobj->handle , (SOCKET)fobj->handle , ev) ;
    else if(op == EPOLL_CTL_DEL)
        result = iocp_del(eobj->handle , (SOCKET)fobj->handle) ;
    else if(op == EPOLL_CTL_MOD)
        result = iocp_mod(eobj->handle , (SOCKET)fobj->handle , ev) ;
        
    return (result?0:-1) ;
}

int epoll_wait (int epfd, struct epoll_event * evs ,  int maxevents, int timeout)
{
    return 0 ;
}

bool iocp_add(HANDLE iocp , SOCKET fd , struct epoll_event * ev)
{
    return false ;
}

bool iocp_del(HANDLE iocp , SOCKET fd)
{
    return true ;
}

bool iocp_mod(HANDLE iocp , SOCKET fd , struct epoll_event * ev)
{
    return false ;
}


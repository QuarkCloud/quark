
#include <sys/epoll.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <winsock2.h>
#include <errno.h>
#include "internal/rlist.h"
#include "internal/rbtree.h"
#include "internal/fsocket.h"
#include "internal/iocp_mgr.h"

int epoll_create (int size)
{
    iocp_mgr_t * mgr = iocp_mgr_new() ;
    if(mgr == NULL)
        return -1 ;
    else
        return mgr->epfd ;
}

iocp_mgr_t * fd_to_iocp(int fd) ;
socket_t *   fd_to_socket(int fd) ;

int epoll_ctl (int epfd, int op, int fd, struct epoll_event * ev)
{
    iocp_mgr_t * mgr = NULL ;
    socket_t * sock = NULL ; 
    if((mgr = fd_to_iocp(epfd)) == NULL || (sock = fd_to_socket(fd)) == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }

    bool result = false ;
    if(op == EPOLL_CTL_ADD)
        result = iocp_mgr_add(mgr , sock , ev) ;
    else if(op == EPOLL_CTL_DEL)
        result = iocp_mgr_del(mgr , sock , ev) ;
    else if(op == EPOLL_CTL_MOD)
        result = iocp_mgr_mod(mgr , sock , ev) ;
        
    return (result == true ? 0 : -1) ;
}

int epoll_wait (int epfd, struct epoll_event * evs ,  int maxevents, int timeout)
{
    iocp_mgr_t * mgr = fd_to_iocp(epfd) ;
    if(mgr == NULL)
    {
        errno = EBADF ;
        return -1 ;
    }

    //尽可能攒齐了再返回，实在没有，只能等了。
    int counter = 0 ;
    while((counter < maxevents) && (iocp_mgr_wait(mgr , 0) == 0)) 
        ++counter ;
    if(counter == 0)
        iocp_mgr_wait(mgr , timeout) ;

    return iocp_mgr_retrieve(mgr , evs , maxevents) ;
}

iocp_mgr_t * fd_to_iocp(int fd)
{
    wobj_t * obj = ::wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_IOCP || obj->addition == NULL)
        return NULL ;
    else
        return (iocp_mgr_t *)obj->addition ;
}

socket_t *   fd_to_socket(int fd)
{
    wobj_t * obj = ::wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_SOCK || obj->addition == NULL)
        return NULL ;
    else
        return (socket_t *)obj->addition ;
}



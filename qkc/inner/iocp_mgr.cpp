
#include "iocp_mgr.h"
#include <errno.h>
#include <wintf/wobj.h>

int iocp_mgr_new()
{
    iocp_mgr_t * mgr = (iocp_mgr_t *)::malloc(sizeof(iocp_mgr_t)) ;
    if(mgr == NULL)
    {
        errno = ENOMEM ;
        return -1 ;
    }

    if(iocp_mgr_init(mgr) == false)
    {
        ::free(mgr) ;
        return -1 ;
    }

    int epfd = wobj_set(WOBJ_IOCP , mgr->iocp , mgr) ;
    if(epfd == INVALID_WOBJ_ID)
        return -1 ;
    mgr->epfd = epfd ;
    return epfd ;
}

int iocp_mgr_free(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return -1 ;

    int oid = mgr->epfd ;
    iocp_mgr_final(mgr) ;
    wobj_del(oid) ;
    ::free(mgr) ;
    return 0 ;
}

bool iocp_mgr_init(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;
    ::memset(mgr , 0 , sizeof(iocp_mgr_t)) ;
    mgr->iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(mgr->iocp == INVALID_HANDLE_VALUE)
    {
        errno = EIO ;
        return false ;
    }

    mgr->locker = ::CreateMutex(NULL , FALSE , NULL) ;

    rlist_init(&mgr->ready) ;
    rlist_init(&mgr->monitored) ;

    return true ;
}

bool iocp_mgr_final(iocp_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;
   
    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    iocp_mgr_items_free(&mgr->monitored) ;
    iocp_mgr_items_free(&mgr->ready) ;

    if(mgr->iocp != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(mgr->iocp) ;
        mgr->iocp = INVALID_HANDLE_VALUE ;
    }

    ::ReleaseMutex(mgr->locker) ;
    ::CloseHandle(mgr->locker) ;

    ::memset(mgr , 0 , sizeof(iocp_mgr_t)) ;
    return true ;
}

bool iocp_mgr_items_free(rlist_t * rlist) 
{
    if(rlist == NULL)
        return false ;
    rlist_t * next = NULL ;
    while((next = rlist->next) != rlist)
    {
        rlist_del(rlist , next) ;
        iocp_mgr_item_free((iocp_item_t *)next) ;
    }
    return true ;
}

bool iocp_mgr_item_free(iocp_item_t * item) 
{
    rlist_del(NULL , &item->link) ;

    socket_t * s = item->socket ;
    item->socket = NULL ;
    s->addition = NULL ;
    socket_free(s) ;
    free(item) ;

    return true ;
}

bool iocp_mgr_add(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev)
{
    if(mgr == NULL || s == NULL || ev == NULL)
        return false ;

    iocp_item_t * item = (iocp_item_t *)::malloc(sizeof(iocp_item_t)) ;
    if(item == NULL)
        return false ;
    ::memset(item , 0 , sizeof(iocp_item_t)) ;

    item->socket = s ;
    rlist_init(&item->link) ;
    ::memcpy(&item->data , ev , sizeof(struct epoll_event)) ;

    s->addition = item ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    rlist_add_tail(&mgr->monitored , &item->link) ;
    ::ReleaseMutex(mgr->locker) ;

    //绑定到iocp中
    if(::CreateIoCompletionPort((HANDLE)s->socket , mgr->iocp , 0 , 0) == NULL)
        return false ;

    if

    if(s->stage == SOCKET_STAGE_LISTEN)
    {
        //启动接收
        socket_start_accept(s->acceptor) ;
    }

    if(s->type == SOCK_STREAM && s->stage == SOCKET_STAGE_CONNECT 

    return true ;
}

bool iocp_mgr_mod(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev)
{
    if(mgr == NULL || s == NULL || ev == NULL || s->addition == NULL)
        return false ;

    iocp_item_t * item = (iocp_item_t *)s->addition ;
    if(item->socket != s)
        return false ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    rlist_del(NULL , &item->link) ;
    ::memcpy(&item->data , ev , sizeof(struct epoll_event)) ;
    rlist_add_tail(&mgr->monitored , &item->link) ;
    ::ReleaseMutex(mgr->locker) ;
    
    return true ;
}

bool iocp_mgr_del(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev)
{
    if(mgr == NULL || s == NULL || ev == NULL || s->addition == NULL)
        return false ;

    iocp_item_t * item = (iocp_item_t *)s->addition ;
    s->addition = NULL ;

    ::WaitForSingleObject(mgr->locker , INFINITE) ;
    rlist_del(NULL , &item->link) ;
    iocp_mgr_item_free(&item) ;
    ::ReleaseMutex(mgr->locker) ;

    return true ;
}



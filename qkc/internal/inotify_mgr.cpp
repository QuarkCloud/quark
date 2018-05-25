
#include "inotify_mgr.h"
#include "wintf/wobj.h"
#include <errno.h>

inotify_mgr_t *  inotify_mgr_new()
{
    size_t size = sizeof(inotify_mgr_t) ;
    inotify_mgr_t * mgr = (inotify_mgr_t *)::malloc(size) ;
    if(mgr == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }
    ::memset(mgr , 0 , size) ;

    if(inotify_mgr_init(mgr) == true)
        return mgr ;
    ::free(mgr) ;
    return NULL ;
}

int  inotify_mgr_free(inotify_mgr_t * mgr)
{
    inotify_mgr_final(mgr) ;
    ::free(mgr) ;
    return 0 ;
}

bool inotify_mgr_init(inotify_mgr_t * mgr)
{
    mgr->iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    mgr->locker = ::CreateMutexA(NULL , FALSE , NULL) ;

    int oid = ::wobj_set(WOBJ_NOTF , mgr->iocp , mgr) ;
    if(oid == INVALID_WOBJ_ID)
        return false ;

    mgr->ifd = oid ;
    return true ;}

bool inotify_mgr_final(inotify_mgr_t * mgr)
{
    wobj_del(mgr->ifd) ;

    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    rb_node_t * node = NULL ;
    while((node = rb_first(&mgr->items)) != NULL)
    {
        inotify_item_t * item = (inotify_item_t *)node;

        HANDLE handle = item->handle ;
        if(handle != NULL)
            ::FindCloseChangeNotification(handle) ;
        rb_erase(&mgr->items , node) ;

        ::free(node) ;
    }

    ::CloseHandle(mgr->iocp) ;
    ::ReleaseMutex(mgr->locker) ;
    ::CloseHandle(mgr->locker) ;

    return true ;
}

bool inotify_items_free(rb_tree_t * items) 
{
    rb_node_t * node = NULL ;
    while((node = rb_first(items)) != NULL)
    {
        inotify_item_free((inotify_item_t *)node) ;
        rb_erase(items , node) ;

        ::free(node) ;
    }
    return true ;
}

bool inotify_item_free(inotify_item_t * item)
{
    HANDLE handle = item->handle ;
    if(handle == NULL)
        return false ;

    ::FindCloseChangeNotification(handle) ;
    return true ;
}

size_t inotify_item_size() 
{
    inotify_item_t dummy ;
    return (size_t)((char *)&dummy - dummy.data.name) ;
}

int inotify_mgr_add(inotify_mgr_t * mgr , const char * name , uint32_t mask)
{
    if(mgr == NULL || name == NULL || mask == 0)
    {
        errno = EINVAL ;
        return -1 ;
    }

    size_t item_size = inotify_item_size() ;
    size_t name_size = 0 ;
    if(name != NULL)
        name_size = ::strlen(name) ;

    size_t size = item_size + name_size + 1;
    inotify_ite_t * item = (inotify_ite_t * )::malloc(size) ;
    if(item == NULL)
    {
        errno = ENOMEM ;
        return -1 ;
    }
    ::memset(item , 0 , size) ;

    item->data.mask = mask ;
    ::memcpy(item->data.name , name , name_size) ;
    item->data.len = name_size + 1 ;

    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
    {
        ::free(item) ;
        return -1 ;
    }
    int wd = ++mgr->last_id ;
    item->data.wd = wd ;

    return 0 ;
}

bool inotify_mgr_del(inotify_mgr_t * mgr , int wd)
{
    return false ;
}


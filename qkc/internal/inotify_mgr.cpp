
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
    rlist_init(&mgr->ready) ;

    int oid = ::wobj_set(WOBJ_NOTF , mgr->iocp , mgr) ;
    if(oid == INVALID_WOBJ_ID)
        return false ;

    mgr->ifd = oid ;
    return true ;}

bool inotify_mgr_final(inotify_mgr_t * mgr)
{
    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;


}

bool inotify_mgr_items_free(rlist_t * rlist) ;
bool inotify_mgr_item_free(inotify_item_t * item) ;
bool inotify_mgr_item_ready(inotify_mgr_t * mgr , inotify_item_t * item) ;
bool inotify_mgr_item_unready(inotify_mgr_t * mgr , inotify_item_t * item) ;

int inotify_mgr_add(inotify_mgr_t * mgr , const char * name , uit32_t mask) ;
bool inotify_mgr_del(inotify_mgr_t * mgr , int wd) ;


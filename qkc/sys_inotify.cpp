
#include <sys/inotify.h>
#include <windows.h>
#include <wintf/wobj.h>
#include "internal/inotify_mgr.h"

int inotify_init (void)
{
    inotify_mgr_t * mgr = inotify_mgr_new() ;
    if(mgr == NULL)
        return -1 ;
    return mgr->ifd ;
}

int inotify_add_watch (int fd, const char *name, uint32_t mask)
{
    wobj_t * iobj = wobj_get(fd) ;
    if(iobj == NULL || iobj->type != WOBJ_NOTF || iobj->handle == NULL || iobj->addition == NULL)
        return -1 ;

    inotify_mgr_t * mgr = (inotify_mgr_t *)iobj->addition ;
    return inotify_mgr_add(mgr , name , mask) ;
}

int inotify_rm_watch (int fd, uint32_t wd)
{
    wobj_t * obj = wobj_get(wd) ;
    if(obj == NULL || obj->type != WOBJ_NOTF || obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    inotify_mgr_t * mgr = (inotify_mgr_t *)obj->addition ;
    return (inotify_mgr_del(mgr , wd) == true?0:-1) ;
}

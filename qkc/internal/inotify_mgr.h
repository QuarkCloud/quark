
#ifndef __QKC_INTERNAL_INOTIFY_MGR_H
#define __QKC_INTERNAL_INOTIFY_MGR_H 1


#include <sys/inotify.h>
#include <windows.h>
#include "rlist.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __st_inotify_item    inotify_item_t ;
typedef struct __st_inotify_mgr     inotify_mgr_t ;

struct __st_inotify_item{
    rlist_t                 link ;
    struct inotify_event    data ;
    uint32_t                occur ;     //已经触发的事件
    inotify_mgr_t *         owner ;
};

struct __st_inotify_mgr{
    int     ifd ;
    HANDLE  iocp ;                  //必须的
    HANDLE  locker ;                //保护下面的列表
    rlist_t ready ;                 //已经就绪的
    int     ready_count ;
} ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_INTERNAL_INOTIFY_MGR_H */

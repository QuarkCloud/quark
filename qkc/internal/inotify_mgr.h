
#ifndef __QKC_INTERNAL_INOTIFY_MGR_H
#define __QKC_INTERNAL_INOTIFY_MGR_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <windows.h>
#include "rlist.h"
#include "rbtree.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __st_inotify_item    inotify_item_t ;
typedef struct __st_inotify_mgr     inotify_mgr_t ;

struct __st_inotify_item{
    rlist_t                 link ;
    rb_node_t               node ;
    HANDLE                  handle ;
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
    rb_tree_t items ;               //注册的事件
} ;

inotify_mgr_t *  inotify_mgr_new() ;
int  inotify_mgr_free(inotify_mgr_t * mgr) ;
bool inotify_mgr_init(inotify_mgr_t * mgr) ;
bool inotify_mgr_final(inotify_mgr_t * mgr) ;

bool inotify_mgr_items_free(rlist_t * rlist) ;
bool inotify_mgr_item_free(inotify_item_t * item) ;
bool inotify_mgr_item_ready(inotify_mgr_t * mgr , inotify_item_t * item) ;
bool inotify_mgr_item_unready(inotify_mgr_t * mgr , inotify_item_t * item) ;

int inotify_mgr_add(inotify_mgr_t * mgr , const char * name , uit32_t mask) ;
bool inotify_mgr_del(inotify_mgr_t * mgr , int wd) ;


#ifdef __cplusplus
}
#endif

#endif /** __QKC_INTERNAL_INOTIFY_MGR_H */

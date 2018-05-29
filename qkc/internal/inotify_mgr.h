
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

/**
    FindFirstChangeNotification不能够提示具体变化情况，用ReadDirectoryChangesW
    2018-05-29
    初步实现原型，等待后续继续完善。根据inotify的应用来调整
*/

typedef struct __st_inotify_item    inotify_item_t ;
typedef struct __st_inotify_mgr     inotify_mgr_t ;
typedef struct __st_inotify_ovlp    inotify_ovlp_t ;

struct __st_inotify_item{
    rb_node_t               node ;
    HANDLE                  handle ;
    uint32_t                occur ;     //已经触发的事件
    inotify_mgr_t *         owner ;
    inotify_ovlp_t *        olvp ;
    struct inotify_event    data ;
};

static const size_t kOvlpBufferSize = 1024 ;
struct __st_inotify_ovlp{
    OVERLAPPED          ovlp ;
    inotify_item_t     *item ;
    char * buffer ;
    char cache[kOvlpBufferSize] ;    
} ;

struct __st_inotify_mgr{
    int     ifd ;
    HANDLE  iocp ;                  //必须的
    HANDLE  locker ;                //保护下面的列表
    rb_tree_t items ;               //注册的事件
    int     count ;
    int     last_id ;
} ;

inotify_mgr_t *  inotify_mgr_new() ;
int  inotify_mgr_free(inotify_mgr_t * mgr) ;
bool inotify_mgr_init(inotify_mgr_t * mgr) ;
bool inotify_mgr_final(inotify_mgr_t * mgr) ;

bool inotify_items_free(rb_tree_t * items) ;
bool inotify_item_free(inotify_item_t * item) ;
size_t inotify_item_size() ;

int inotify_mgr_add(inotify_mgr_t * mgr , const char * name , uint32_t mask) ;
bool inotify_mgr_del(inotify_mgr_t *  mgr , int wd) ;

//TO-DO : 事件的映射
DWORD inotify_from_linux(uint32_t mask) ;
uint32_t inotify_to_linux(DWORD action) ;

int inotify_read(inotify_mgr_t * mgr , char * buffer , int bufsize) ;

int wide_to_char(const wchar_t * wstr , int bytes , char * str , int len) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_INTERNAL_INOTIFY_MGR_H */


#ifndef __QKC_INTERNAL_IOCP_MGR_H
#define __QKC_INTERNAL_IOCP_MGR_H 1

#include <sys/epoll.h>
#include <windows.h>
#include <winsock2.h>
#include "rlist.h"
#include "fsocket.h"
#include "fpipe.h"

__BEGIN_DECLS

typedef struct __st_iocp_item iocp_item_t ;
typedef struct __st_iocp_mgr iocp_mgr_t ;

typedef enum{
    IOCP_ITEM_UNKNOWN ,
    IOCP_ITEM_SOCKET ,
    IOCP_ITEM_PIPE 
} iocp_item_type_t ;

struct __st_iocp_item{
    rlist_t             link ;
    int                 fd ;
    iocp_item_type_t    type ;      //判断哪种类型的item
    struct epoll_event  data ;
    uint32_t            occur ;     //已经触发的事件
    iocp_mgr_t *        owner ;
};

struct __st_iocp_mgr{
    int     epfd ;
    HANDLE  iocp ;                  //必须的
    HANDLE  locker ;                //保护下面的列表
    rlist_t ready ;                 //已经就绪的
    int     ready_count ;
    volatile LONG thread_counter ;  //加入线程数
} ;


iocp_mgr_t *  iocp_mgr_new() ;
int  iocp_mgr_free(iocp_mgr_t * mgr) ;
bool iocp_mgr_init(iocp_mgr_t * mgr) ;
bool iocp_mgr_final(iocp_mgr_t * mgr) ;

bool iocp_mgr_items_free(rlist_t * rlist) ;
bool iocp_mgr_item_free(iocp_item_t * item) ;
bool iocp_mgr_item_ready(iocp_mgr_t * mgr , iocp_item_t * item) ;
bool iocp_mgr_item_unready(iocp_mgr_t * mgr , iocp_item_t * item) ;

bool iocp_mgr_add(iocp_mgr_t * mgr , int fd , struct epoll_event * ev) ;
bool iocp_mgr_mod(iocp_mgr_t * mgr , int fd , struct epoll_event * ev) ;
bool iocp_mgr_del(iocp_mgr_t * mgr , int fd , struct epoll_event * ev) ;

int iocp_mgr_wait(iocp_mgr_t * mgr , int timeout) ;
int iocp_mgr_retrieve(iocp_mgr_t * mgr , struct epoll_event * evs ,  int maxevents);

int iocp_socket_callback(socket_t * s , int evt , int result) ;
int iocp_pipe_callback(pipe_t * p , int evt , int result) ;

__END_DECLS

#endif /** __QKC_INTERNAL_IOCP_MGR_H */

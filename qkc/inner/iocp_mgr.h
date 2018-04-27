
#ifndef __IOCP_MGR_H
#define __IOCP_MGR_H 1

#include <sys/epoll.h>
#include <windows.h>
#include <winsock2.h>
#include "rlist.h"
#include "fsocket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __st_iocp_item iocp_item_t ;
typedef struct __st_iocp_mgr iocp_mgr_t ;

struct __st_iocp_item{
    rlist_t             link ;
    socket_t *          socket ;
    struct epoll_event  data ;
    uint32_t            occur ;     //已经触发的事件
    iocp_mgr_t *        owner ;
};

struct __st_iocp_mgr{
    int     epfd ;
    HANDLE  iocp ;                  //必须的
    HANDLE  locker ;                //保护下面的列表
    rlist_t ready ;                 //已经就绪的
    //rlist_t monitored ;             //被监控的
    volatile LONG thread_counter ;  //加入线程数
} ;


int  iocp_mgr_new() ;
int  iocp_mgr_free(iocp_mgr_t * mgr) ;
bool iocp_mgr_init(iocp_mgr_t * mgr) ;
bool iocp_mgr_final(iocp_mgr_t * mgr) ;

bool iocp_mgr_items_free(rlist_t * rlist) ;
bool iocp_mgr_item_free(iocp_item_t * item) ;
bool iocp_mgr_item_ready(iocp_mgr_t * mgr , iocp_item_t * item) ;
bool iocp_mgr_item_unready(iocp_mgr_t * mgr , iocp_item_t * item) ;

bool iocp_mgr_add(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev) ;
bool iocp_mgr_mod(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev) ;
bool iocp_mgr_del(iocp_mgr_t * mgr , socket_t * s , struct epoll_event * ev) ;

int iocp_mgr_wait(iocp_mgr_t * mgr , int timeout) ;

int iocp_socket_callback(socket_t * s , int evt , int result) ;

#ifdef __cplusplus
}
#endif

#endif /** __IOCP_MGR_H */

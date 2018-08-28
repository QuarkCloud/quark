
#ifndef __QKC_INTERNAL_IOCP_MGR_H
#define __QKC_INTERNAL_IOCP_MGR_H 1

#include <sys/epoll.h>
#include <windows.h>
#include <winsock2.h>
#include "rlist.h"
#include "iocp_item.h"

__BEGIN_DECLS

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
void iocp_process_event(int events , int result , int& occur) ;


__END_DECLS

#endif /** __QKC_INTERNAL_IOCP_MGR_H */

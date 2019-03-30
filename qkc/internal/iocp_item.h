
#ifndef __QKC_INTERNAL_IOCP_ITEM_H
#define __QKC_INTERNAL_IOCP_ITEM_H 1

#include <sys/epoll.h>
#include <windows.h>
#include <winsock2.h>
#include "rlist.h"

__BEGIN_DECLS

/**
    2018-08-29
    匿名管道不支持异步读写，而有名管道和socket没有本质的区别，因此在iocp中不支持管道操作。
	2019-03-29
	为了实现Linux下的管道，使用有名管道，模拟匿名管道，同时支持异步操作，能够被IOCP支持。
	之所以要支持管道，是因为libuv需要支持管道作为信号传输句柄。
*/
typedef struct __st_iocp_item iocp_item_t ;
typedef struct __st_iocp_mgr iocp_mgr_t ;
typedef enum{
    IOCP_ITEM_UNKNOWN ,
    IOCP_ITEM_SOCKET , 
	IOCP_ITEM_FILE ,
	IOCP_ITEM_PIPE
} iocp_item_type_t ;

typedef enum{
    IOCP_EVENT_OPEN     = 1 ,
    IOCP_EVENT_CLOSE    = 2 ,
    IOCP_EVENT_READ     = 3 ,
    IOCP_EVENT_WRITE    = 4 ,
} iocp_event_t;

typedef enum{
    OVLP_VOID   =   0 ,
    OVLP_INPUT  =   1 ,
    OVLP_OUTPUT =   2 
} ovlp_type_t ;

typedef int (*iocp_item_callback_t)(iocp_item_t * item , int evt , int result) ;
typedef void (*iocp_item_free_t)(iocp_item_t * item) ;

struct __st_iocp_item{
    rlist_t                 link ;
    int                     fd ;
    iocp_item_type_t        type ;      //判断哪种类型的item
    struct epoll_event      data ;
    uint32_t                occur ;     //已经触发的事件
    iocp_mgr_t *            owner ;
    iocp_item_callback_t    callback ;
    iocp_item_free_t        free ;
    void *                  addition ;
};

typedef struct __st_iocp_ovlp
{
    OVERLAPPED          ovlp ;
    int                 status ;
    ovlp_type_t         type ;
    volatile LONG       counter ;
    iocp_item_t *       owner ;
} iocp_ovlp_t ;

bool iocp_ovlp_lock(iocp_ovlp_t * ovlp) ;
bool iocp_ovlp_unlock(iocp_ovlp_t * ovlp) ;
int iocp_ovlp_counter(iocp_ovlp_t * ovlp) ;



__END_DECLS

#endif /** __QKC_INTERNAL_IOCP_ITEM_H */

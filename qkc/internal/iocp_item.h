
#ifndef __QKC_INTERNAL_IOCP_ITEM_H
#define __QKC_INTERNAL_IOCP_ITEM_H 1

#include <sys/epoll.h>
#include <windows.h>
#include <winsock2.h>
#include "rlist.h"

__BEGIN_DECLS

typedef struct __st_iocp_item iocp_item_t ;
typedef struct __st_iocp_mgr iocp_mgr_t ;
typedef enum{
    IOCP_ITEM_UNKNOWN ,
    IOCP_ITEM_SOCKET ,
    IOCP_ITEM_PIPE 
} iocp_item_type_t ;

typedef enum{
    OVLP_VOID   =   0 ,
    OVLP_INPUT  =   1 ,
    OVLP_OUTPUT =   2 
} ovlp_type_t ;


struct __st_iocp_item{
    rlist_t             link ;
    int                 fd ;
    iocp_item_type_t    type ;      //判断哪种类型的item
    struct epoll_event  data ;
    uint32_t            occur ;     //已经触发的事件
    iocp_mgr_t *        owner ;
};

typedef struct __st_iocp_ovlp
{
    OVERLAPPED          ovlp ;
    int                 status ;
    ovlp_type_t         type ;
    volatile LONG       counter ;
} iocp_ovlp_t ;


__END_DECLS

#endif /** __QKC_INTERNAL_IOCP_ITEM_H */

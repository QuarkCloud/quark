
#ifndef __QKC_INTERNAL_ADDR_MGR_H
#define __QKC_INTERNAL_ADDR_MGR_H 1

#include <windows.h>
#include "rbtree.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct __st_addr_item{
    rb_node_t link ;
    void * addr ;
    void * addition ;
    void (*free)(void *) ;
} addr_item_t ;

typedef struct __st_addr_mgr{
    rb_root_t   link ;
    SRWLOCK     guard ;
} addr_mgr_t ;

#define ADDR_MGR_INIT {{NULL , NULL} , SRWLOCK_INIT}

void addr_item_free(addr_item_t * item) ;

int addr_compare(const rb_node_t * src , const rb_node_t * dst) ;
bool addr_mgr_init(addr_mgr_t * mgr) ;
bool addr_mgr_final(addr_mgr_t * mgr) ;

bool addr_mgr_add(addr_mgr_t * mgr , void * addr , void * addition , void (*item_free)(void *)) ;
addr_item_t * addr_mgr_find(addr_mgr_t * mgr , const void * addr) ;
bool addr_mgr_del(addr_mgr_t * mgr , addr_item_t * item) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_ADDR_MGR_H */


#include "addr_mgr.h"
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int addr_compare(const rb_node_t * src , const rb_node_t * dst)
{
    intptr_t result = ((intptr_t)src - (intptr_t)dst) ;
    if(result == 0)
        return 0 ;
    else if(result < 0)
        return -1 ;
    else
        return -1 ;
}

bool addr_mgr_init(addr_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;

    mgr->link.root = NULL ;
    mgr->link.key_compare = addr_compare ;
    mgr->guard.Ptr =  NULL ;
    return true ;
}

void addr_item_free(addr_item_t * item) 
{
    if(item == NULL)
        return ;

    if(item->free != NULL && item->addition != NULL)
        item->free(item->addition) ;
}

bool addr_mgr_final(addr_mgr_t * mgr)
{
    if(mgr == NULL)
        return false ;    

    ::AcquireSRWLockExclusive(&mgr->guard) ;

    rb_node_t * cur = rb_first(&mgr->link);
    while(cur != NULL)
    {
        rb_node_t * next = rb_next(cur) ;
        addr_item_t * item = (addr_item_t *)cur ;
        addr_item_free(item) ;
        ::free(item) ;
        cur = next ;
    }

    ::ReleaseSRWLockExclusive(&mgr->guard) ;
    return true ;
}

bool addr_mgr_add(addr_mgr_t * mgr , void * addr , void * addition , void (*item_free)(void *))
{
    addr_item_t * item = (addr_item_t *)::malloc(sizeof(addr_item_t)) ;
    ::memset(item , 0 , sizeof(addr_item_t)) ;
    item->addr = addr ;
    item->addition = addition ;
    item->free = item_free ;

    bool result = false ;

    ::AcquireSRWLockExclusive(&mgr->guard) ;
    result = ::rb_insert(&mgr->link , &item->link) ;
    ::ReleaseSRWLockExclusive(&mgr->guard) ;

    if(result == false)
        ::free(item) ;

    return result ;
}

addr_item_t * addr_mgr_find(addr_mgr_t * mgr , const void * addr)
{

    ::AcquireSRWLockShared(&mgr->guard) ;

    addr_item_t node ;
    node.addr = (void *)addr ;
    addr_item_t * item = (addr_item_t *)::rb_find(&mgr->link , &node.link) ;
    ::ReleaseSRWLockShared(&mgr->guard) ;

    return item ;
}

bool addr_mgr_del(addr_mgr_t * mgr , addr_item_t * item)
{
    if(mgr == NULL || item == NULL)
        return false;

    ::AcquireSRWLockShared(&mgr->guard) ;
    ::rb_erase(&mgr->link , &item->link) ;
    ::ReleaseSRWLockShared(&mgr->guard) ;

    addr_item_free(item) ;
    ::free(item) ;

    return true ;    
}


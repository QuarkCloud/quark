
#ifndef __QKC_INTERNAL_RXTREE_H
#define __QKC_INTERNAL_RXTREE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define RADIX_NO_VALUE   (uintptr_t) -1


typedef struct __st_rx_node rx_node_t ;
struct __st_rx_node 
{
    rx_node_t *     right  ;        //环形双向链表
    rx_node_t *     left  ;   
    rx_node_t *     childs  ;

    union{
        rx_node_t * nodes  ;    //一个数组，挂接连续地址空间的的node
        uintptr_t   value ;     //对于叶子节点来说，这个挂着的是value
    } ;
} ; 

typedef struct __st_rx_tree rx_tree_t ;
struct __st_rx_tree{
    rx_node_t           root;
    uint8_t             bits[sizeof(uintptr_t)] ;
    uint8_t             total ;
    uint32_t            level ;
    void (*free)(uintptr_t value) ;
};

static const uint32_t kPageShift =   12 ;

inline size_t size_page_align(size_t size) 
{
    uint32_t mask = (1 << kPageShift) -1 ;
    uint32_t ext = (size & mask) ? 1 : 0 ;

    return (((size >> kPageShift) + ext) << kPageShift) ;
}

bool rx_tree_init(rx_tree_t * tree , uint8_t * bits , uint32_t level , void (*value_free)(uintptr_t value)) ;
void rx_tree_final(rx_tree_t * tree) ;
bool rx_tree_insert(rx_tree_t * tree , uint32_t key , uintptr_t value) ;
bool rx_tree_erase(rx_tree_t * tree , uint32_t key) ;
rx_node_t * rx_tree_find(rx_tree_t * tree , uint32_t key) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_RXTREE_H */

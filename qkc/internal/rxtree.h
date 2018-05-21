
#ifndef __QKC_INTERNAL_RXTREE_H
#define __QKC_INTERNAL_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define RADIX_NO_VALUE   (uintptr_t) -1

typedef struct __st_rx_node rx_node_t ;
{
    rx_node_t   *right;
    rx_node_t   *left;
    rx_node_t   *parent;
    uintptr_t   value;
} ; 

typedef struct __st_rx_tree rx_tree_t ;
struct __st_rx_tree{
    rx_node_t           *root;
    rx_node_t           *free;
    char                *start;
    size_t              size;
};

QKCAPI rx_tree_t *rx_tree_create();
QKCAPI int32_t rx_tree32_insert(rx_tree_t *tree, uint32_t key, uint32_t mask, uintptr_t value);
QKCAPI int32_t rx_tree32_delete(rx_tree_t *tree, uint32_t key, uint32_t mask);
QKCAPI uintptr_t rx_tree32_find(rx_tree_t *tree, uint32_t key);

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_RXTREE_H */

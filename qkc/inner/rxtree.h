
#ifndef __QKC_INNER_RXTREE_H
#define __QKC_INNER_RXTREE_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define RADIX_NO_VALUE   (uintptr_t) -1

typedef struct st_rx_node{
    struct st_rx_node   *right;
    struct st_rx_node   *left;
    struct st_rx_node   *parent;
    uintptr_t           value;
} rx_node_t ;


typedef struct st_rx_tree{
    rx_node_t           *root;
    rx_node_t           *free;
    char                *start;
    size_t              size;
} rx_tree_t;


QKCAPI rx_tree_t *rx_tree_create();
QKCAPI int32_t rx_tree32_insert(rx_tree_t *tree, uint32_t key, uint32_t mask, uintptr_t value);
QKCAPI int32_t rx_tree32_delete(rx_tree_t *tree, uint32_t key, uint32_t mask);
QKCAPI uintptr_t rx_tree32_find(rx_tree_t *tree, uint32_t key);

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INNER_RXTREE_H */

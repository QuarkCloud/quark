
#ifndef __QKC_INTERNAL_RBTREE_H
#define __QKC_INTERNAL_RBTREE_H 1

#include <quark_compile.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define	RB_RED		0
#define	RB_BLACK	1

typedef struct st_rb_node {
	struct st_rb_node *parent;
	struct st_rb_node *right;
	struct st_rb_node *left;
	int color;
}rb_node_t ;

typedef struct st_rb_root {
	rb_node_t * root;
} rb_root_t ;

// Copy from linux kernel 2.6 source (kernel.h, stddef.h)
#define container_of(ptr, type, member) ({      \
	const typeof( ((type *)0)->member ) *__mptr = (ptr);  \
	(type *)( (char *)__mptr - offsetof(type,member) );})
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


#define RB_ROOT	(rb_root_t) { NULL }
#define	rb_entry(ptr, type, member) container_of(ptr, type, member)

QKCAPI void rb_insert_color(rb_node_t *, rb_root_t *);
QKCAPI void rb_erase(rb_node_t *, rb_root_t *);

/* Find logical next and previous nodes in a tree */
QKCAPI rb_node_t *rb_next(rb_node_t *);
QKCAPI rb_node_t *rb_prev(rb_node_t *);
QKCAPI rb_node_t *rb_first(rb_root_t *);
QKCAPI rb_node_t *rb_last(rb_root_t *);

/* Fast replacement of a single node without remove/rebalance/add/rebalance */
QKCAPI void rb_replace_node(rb_node_t *victim, rb_node_t * new_node,  rb_root_t *root);

static inline void rb_link_node(rb_node_t * node, rb_node_t * parent,	rb_node_t ** rb_link)
{
	node->parent = parent;
	node->color = RB_RED;
	node->left = node->right = NULL;

	*rb_link = node;
}

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_RBTREE_H */

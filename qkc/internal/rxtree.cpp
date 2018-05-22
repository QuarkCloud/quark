
#include "rxtree.h"

bool rx_tree_init(rx_tree_t * tree , uint8_t * bits , uint32_t level , void (*value_free)(uintptr_t value))
{
    if(tree == NULL || bits == NULL || level > sizeof(uintptr_t))
        return false ;

    //确保所有bits的和为32或者64
    uint8_t total = 0 ;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        total += bits[lidx] ;
    }
    if(total != 32 && total != 64)
        return false ;

    ::memset(tree , 0 , sizeof(rx_tree_t)) ;
    ::memcpy(tree->bits , bits , level) ;
    tree->level = level ;
    tree->total = total ;
    tree->free = value_free ;

    return true ;
}

void rx_tree_final(rx_tree_t * tree)
{
    if(tree == NULL)
        return ;

    rx_node_t *nodes[8] ;
    ::memset(nodes , 0 , sizeof(nodes)) ;

    nodes[0] = &tree->root ;
    int level = (int)tree->level ;
    int lidx = 0;
    uint32_t last_value = 0 ;
    while(lidx >= 0 && lidx <= level)
    {
        rx_node_t * cur = nodes[lidx] ;
        if(cur == NULL)
        {
            --lidx ;
            continue ;
        }
        if(lidx >= level)
        {
            //最后一个，是value
            if(tree->free != NULL)
                tree->free(cur->value) ;
            if(last_value != cur->value)
            {
                ::printf("free value error , [%u] \n" , last_value) ;
            }
            ++last_value ;
            cur->value = 0 ;
        }

        if(cur->childs != NULL)
        {
            ++lidx ;
            nodes[lidx] = cur->childs ;
            continue ;
        }

        rx_node_t * next = cur->left ;
        if(next != cur && next != NULL)
        {
            nodes[lidx] = next ;
            //将cur从环形列表中，摘出来
            cur->right->left = cur->left ;
            cur->left->right = cur->right ;
            cur->left = cur->right = cur ;
        }
        else
        {
            //向上一级
            --lidx ;
            if(lidx >= 0)
            {
                rx_node_t * parent = nodes[lidx] ;
                parent->childs = NULL ;
            }
        }

        if(cur->nodes != NULL)
        {
            ::free(cur->nodes) ;
            cur->nodes = NULL ;
        }
    }
}

bool rx_tree_insert(rx_tree_t * tree , uint32_t key , uintptr_t value)
{
    if(tree == NULL)
        return false ;

    rx_node_t * parent = &tree->root ;
    uint32_t level = tree->level ;
    uint8_t high_bits = 0 ;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        uint8_t bits = tree->bits[lidx] ;
        rx_node_t * nodes = parent->nodes ;
        if(nodes == NULL)
        {
            size_t size = size_page_align(sizeof(rx_node_t) << bits) ;
            if(size == 0)
                ::printf("maybe error \n") ;
            nodes = (rx_node_t *)::malloc(size) ;
            if(nodes == NULL)
                return false ;

            ::memset(nodes , 0 , size) ;
            if(parent->nodes == NULL)
            {
                parent->nodes = nodes ;
            }
        }

        high_bits += bits ;
        uint32_t offset = (key >> (tree->total - high_bits)) & ((1 << bits) - 1) ; 

        rx_node_t * child = nodes + offset ;
        if(parent->childs == NULL)
        {
            child->right = child->left = child ;
            parent->childs = child ;
        }
        else
        {
            rx_node_t * childs = parent->childs ;
            if(childs != child)
            {
                if(child->right == NULL || child->right == NULL)
                    child->right = child->left = child ;

                if(child->right == child)
                {
                    child->right = childs->right ;
                    child->left = childs ;

                    child->right->left = child ;
                    childs->right = child ;
                }
            }
        }

        parent = child ;
    }

    parent->value = value ;
    return true ;
}

bool rx_tree_erase(rx_tree_t * tree , uint32_t key)
{
    if(tree == NULL || tree->root.nodes == NULL)
        return false ;

    uint32_t level = tree->level ;
    uint8_t high_bits = 0 ;
    rx_node_t * cur = &tree->root ;
    rx_node_t * path[8] = {cur};
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        rx_node_t * nodes = cur->nodes ;
        if(nodes == NULL)
            return false ;

        uint8_t bits = tree->bits[lidx] ;
        high_bits += bits ;
        uint32_t offset = (key >> (tree->total - high_bits)) & ((1 <<  bits) -1) ;

        cur = nodes + offset ;
        path[lidx + 1] = cur ;
    }

    if(tree->free != NULL)
        tree->free(cur->value) ;
    cur->value = 0 ;

    //清除访问路径上的空节点
    for(uint32_t lidx = level ; lidx > 0 ; --lidx)
    {
        rx_node_t * node = path[lidx] ;
        if(node->childs == NULL)
        {
            if(node->nodes != NULL)
            {
                ::free(node->nodes) ;
                node->nodes = NULL ;
            }
        }

        rx_node_t * parent = path[lidx - 1] ;
        if(node->left == cur)
        {
            //唯一个节点
            parent->childs = NULL ;
        }
        else
        {
            if(parent->childs == node)
                parent->childs = node->left ;

            node->left->right = node->right ;
            node->right->left = node->left ;
            node->left = node->right = node ;
            break ;
        }
    }

    return true ;
}

rx_node_t * rx_tree_find(rx_tree_t * tree , uint32_t key)
{
    if(tree == NULL || tree->root.nodes == NULL)
        return false ;

    uint32_t level = tree->level ;
    uint8_t high_bits = 0 ;
    rx_node_t * cur = &tree->root;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        rx_node_t * nodes = cur->nodes ;
        if(nodes == NULL)
            return false ;

        uint8_t bits = tree->bits[lidx] ;
        high_bits += bits ;
        uint32_t offset = (key >> (tree->total - high_bits)) & ((1 <<  bits) -1) ;

        cur = nodes + offset ;
    }

    return cur ;
}

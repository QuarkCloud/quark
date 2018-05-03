
#ifndef __QKC_INTERNAL_RLIST_H
#define __QKC_INTERNAL_RLIST_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct _st_rlist{
    struct _st_rlist * prev ;
    struct _st_rlist * next ;
} rlist_t;

static inline void rlist_init(rlist_t * rlist) 
{
    if(rlist != NULL) rlist->prev = rlist->next = rlist ;
}

static inline void rlist_add_tail(rlist_t * rlist , rlist_t * entry)
{
    rlist_t * last = rlist->prev ;
    last->next = entry ;
    entry->prev = last ;
    rlist->prev = entry ;
}

static inline void rlist_del(rlist_t * rlist , rlist_t * entry)
{
    rlist_t * prev = entry->prev ;
    rlist_t * next = entry->next ;

    prev->next = next ;
    next->prev = prev ;

    rlist_init(entry) ;
}

static inline bool rlist_empty(rlist_t *rlist){return (rlist == rlist->prev && rlist == rlist->next) ;}

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_RLIST_H */

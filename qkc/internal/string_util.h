
#ifndef __QKC_INTERNAL_STRING_UTIL_H
#define __QKC_INTERNAL_STRING_UTIL_H 1

#include <quark_compile.h>

__BEGIN_DECLS

typedef struct __st_str_seg{
    const char * start ;
    size_t size ;
} str_seg_t ;

size_t str_split(const char * str , const char  seperator , str_seg_t * segs , size_t count) ;

int str_ncmp(const char * src , size_t nsize , const char * dst) ;

__END_DECLS

#endif /** __QKC_INTERNAL_STRING_UTIL_H */

#ifndef __QKC_ENDIAN_H
#define __QKC_ENDIAN_H 1

#include <quark_compile.h>

__BEGIN_DECLS

static inline bool is_little_endian()
{
    static bool __little_endian__ = true ;
    static bool __endian_inited__ = false ;

    if(__endian_inited__ == false)
    {
        int val = 1 ;
        __little_endian__ = ((*((char *)&val)) == 1) ;
        __endian_inited__ = true ;
    }

    return __little_endian__ ;
}

__END_DECLS

#endif /** __QKC_ENDIAN_H */
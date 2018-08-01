#ifndef __QKC_SYS_UN_H
#define __QKC_SYS_UN_H 1

#include <quark_compile.h>

__BEGIN_DECLS

typedef unsigned short int sa_family_t;

struct sockaddr_un
{
    sa_family_t sun_family ;
    char sun_path[108];         /* Path name.  */
};

__END_DECLS

#endif /** __QKC_SYS_UN_H */

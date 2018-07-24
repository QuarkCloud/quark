#ifndef __QKC_SYS_UN_H
#define __QKC_SYS_UN_H 1

#include <quark_compile.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short int sa_family_t;

struct sockaddr_un
{
    sa_family_t sun_family ;
    char sun_path[108];         /* Path name.  */
};

#ifdef __cplusplus
}
#endif


#endif /** __QKC_SYS_UN_H */

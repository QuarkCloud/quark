
#ifndef __QKC_NET_IF_H
#define __QKC_NET_IF_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


QKCAPI unsigned int if_nametoindex (const char * ifname) ;
QKCAPI char *if_indextoname (unsigned int ifindex, char *ifname) ;

struct if_nameindex
{
    unsigned int if_index;
    char *if_name;
};

QKCAPI struct if_nameindex *if_nameindex (void) ;

QKCAPI void if_freenameindex (struct if_nameindex * ptr) ;

#ifdef __cplusplus
}
#endif

#endif	/** __QKC_NET_IF_H */

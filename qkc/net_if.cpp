
#include <net/if.h>
#include <wstub/iphlpapi.h>
#include <stdlib.h>
#include <string.h>

unsigned int if_nametoindex (const char *ifname)
{
    return _imp_if_nametoindex(ifname) ;
}

char * if_indextoname (unsigned int ifindex , char *ifname)
{
    return _imp_if_indextoname(ifindex , ifname) ;
}

struct if_nameindex * if_nameindex (void)
{
    size_t if_size = sizeof(struct if_nameindex) * 16 ;
    struct if_nameindex * if_bufs = (struct if_nameindex *)::malloc(if_size) ;
    ::memset(if_bufs , 0 , if_size) ;


    size_t if_index = 1 , if_counter = 0;
    while(if_counter < 15)
    {
        char buffer[256] ;
        ::memset(buffer , 0 , sizeof(buffer)) ;
        if(::if_indextoname(if_index , buffer) == NULL)
            break ;

        if_bufs[if_counter].if_index = if_index ;
        size_t nsize = ::strlen(buffer) + 1 ;
        char * name = (char *)::malloc(nsize) ;
        ::memcpy(name , buffer , nsize) ;
        if_bufs[if_counter].if_name = name ;

        ++if_index ;
        ++if_counter ;
    }

    if(if_counter == 0)
    {
        ::free(if_bufs) ;
        return NULL ;
    }

    return if_bufs ;
}

void if_freenameindex (struct if_nameindex *ifn)
{
    struct if_nameindex * head = ifn ;
    while(head->if_name != NULL)
    {
        ::free(head->if_name) ;
        head->if_name = NULL ;

        ++head ;
    }
    ::free(ifn) ;
}




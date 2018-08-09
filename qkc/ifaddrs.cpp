
#include <ifaddrs.h>
#include <wstub/iphlpapi.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static IP_ADAPTER_ADDRESSES* get_all_interfaces() ;

int getifaddrs (struct ifaddrs **ifap)
{
    IP_ADAPTER_ADDRESSES*  adapters = get_all_interfaces() ;
    if(adapters == NULL)
        return -1 ;

    int count = 0 ;
    for(IP_ADAPTER_ADDRESSES * adapter = adapters ; adapter != NULL ; adapter = adapter->Next)
    {
        if(adapter->OperStatus != IfOperStatusUp || adapter->FirstUnicastAddress == NULL)
            continue ;

        for(IP_ADAPTER_UNICAST_ADDRESS * unicast = (IP_ADAPTER_UNICAST_ADDRESS *)adapter->FirstUnicastAddress ;
            unicast != NULL ; unicast = unicast->Next)
        {
            ++count ;
        }
    }

    return 0 ;
}

void freeifaddrs (struct ifaddrs * ifa)
{
    //
}

static IP_ADAPTER_ADDRESSES* get_all_interfaces() 
{
    ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST |
      GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_PREFIX;
    IP_ADAPTER_ADDRESSES* buf = NULL ;
    ULONG size = 0;

    while(true)
    {
        ULONG r = _imp_get_adapters_addresses(AF_UNSPEC,flags,NULL,buf,&size);
        if(r == ERROR_SUCCESS)
            break ;
        if(buf != NULL)
        {
            ::free(buf) ;
            buf = NULL ;
        }

        if(r == ERROR_BUFFER_OVERFLOW)
        {
            buf = (IP_ADAPTER_ADDRESSES*)::malloc(size) ;
            continue ;
        }

        return NULL ;    
    }

    return buf ;
}


#include <ifaddrs.h>
#include <wstub/iphlpapi.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>

static IP_ADAPTER_ADDRESSES* get_all_interfaces() ;
static unsigned int get_if_flags(IP_ADAPTER_ADDRESSES * adapter) ;

int getifaddrs (struct ifaddrs **ifap)
{
    IP_ADAPTER_ADDRESSES*  adapters = get_all_interfaces() ;
    if(adapters == NULL)
        return -1 ;

    struct ifaddrs * ifs = NULL ;
    for(IP_ADAPTER_ADDRESSES * adapter = adapters ; adapter != NULL ; adapter = adapter->Next)
    {
        if(adapter->OperStatus != IfOperStatusUp || adapter->FirstUnicastAddress == NULL)
            continue ;

        size_t ifsize = sizeof(struct ifaddrs) ;
        struct ifaddrs * ifa = (struct ifaddrs *)::malloc(ifsize) ;
        ::memset(ifa , 0 , ifsize) ;

        ifa->ifa_name = ::strdup(adapter->AdapterName) ;
        ifa->ifa_flags = get_if_flags(adapter) ;

        for(IP_ADAPTER_UNICAST_ADDRESS * unicast = (IP_ADAPTER_UNICAST_ADDRESS *)adapter->FirstUnicastAddress ;
            unicast != NULL ; unicast = unicast->Next)
        {
            struct win_sockaddr * wsa = (struct win_sockaddr *)unicast->Address.lpSockaddr ;
            if(wsa->sa_family != AF_INET)
                continue ;

            if(adapter->IfType != IF_TYPE_SOFTWARE_LOOPBACK && adapter->IfType != IF_TYPE_PPP)
                ifa->ifa_flags |= IFF_BROADCAST ;

            if(ifa->ifa_addr == NULL)
                ifa->ifa_addr = (struct sockaddr *)::malloc(sizeof(struct sockaddr)) ;
            ::memcpy(ifa->ifa_addr , wsa , sa_size) ;

            //¼ÆËãÑÚÂë
            if(ifa->ifa_netmask == NULL)
            {
                ifa->ifa_netmask = (struct sockaddr *)::malloc(sizeof(struct sockaddr)) ;
                ::memset(ifa->ifa_netmask , 0 , sizeof(struct sockaddr)) ;
            }
            struct sockaddr_in * sin = (struct sockaddr_in *)ifa->ifa_netmask ;
            int prefix = (int)unicast->OnLinkPrefixLength ;
            sin->sin_family = AF_INET ;
            sin->sin_addr.s_addr = ::htonl(UINT32_MAX << (32 - prefix)) ;
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

static unsigned int get_if_flags(IP_ADAPTER_ADDRESSES * adapter) 
{
    unsigned int flags = IFF_UP;
    if (adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
        flags |= IFF_LOOPBACK;
    else if (adapter->IfType == IF_TYPE_PPP || adapter->IfType == IF_TYPE_SLIP)
        flags |= IFF_POINTOPOINT | IFF_NOARP;
    if (!(adapter->Flags & IP_ADAPTER_NO_MULTICAST))
        flags |= IFF_MULTICAST;
    if (adapter->OperStatus == IfOperStatusUp || adapter->OperStatus == IfOperStatusUnknown)
        flags |= IFF_RUNNING;
    if (adapter->OperStatus != IfOperStatusLowerLayerDown)
        flags |= IFF_LOWER_UP;
    if (adapter->OperStatus == IfOperStatusDormant)
        flags |= IFF_DORMANT;
    return flags;   
}


#include <ifaddrs.h>
#include <wstub/iphlpapi.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>

static IP_ADAPTER_ADDRESSES* get_all_interfaces() ;
static unsigned int get_if_flags(IP_ADAPTER_ADDRESSES * adapter) ;
static bool get_ipforward_table(PMIB_IPFORWARDTABLE& pift) ;
static DWORD get_routedst(PMIB_IPFORWARDTABLE pift , DWORD ifidx) ;

static struct ifaddrs * create_pfpacket(IP_ADAPTER_ADDRESSES * adapter) ;
static struct ifaddrs * create_inet(PMIB_IPFORWARDTABLE pift , IP_ADAPTER_ADDRESSES * adapter) ;
static void link_ifaddrs(struct ifaddrs *& root , struct ifaddrs *& last , struct ifaddrs *&node) ;

int getifaddrs (struct ifaddrs **ifap)
{
    IP_ADAPTER_ADDRESSES*  adapters = get_all_interfaces() ;
    if(adapters == NULL)
        return -1 ;

    PMIB_IPFORWARDTABLE pift = NULL ;
    if(get_ipforward_table(pift) == false)
    {
        ::free(adapters) ;
        return -1 ;
    }

    struct ifaddrs * ifs = NULL , *prev = NULL ;;
    for(IP_ADAPTER_ADDRESSES * adapter = adapters ; adapter != NULL ; adapter = adapter->Next)
    {
        if(adapter->OperStatus != IfOperStatusUp || adapter->FirstUnicastAddress == NULL)
            continue ;

        struct ifaddrs * ifa = create_pfpacket(adapter) ;
        link_ifaddrs(ifs , prev , ifa) ;

        struct ifaddrs * ifb = create_inet(pift , adapter) ;
        link_ifaddrs(ifs , prev , ifb) ;
    } 

    *ifap = ifs ;


    ::free(adapters) ;
    ::free(pift) ;

    return 0 ;
}

void freeifaddrs (struct ifaddrs * ifa)
{
    struct ifaddrs * cur = ifa ;
    while(cur != NULL)
    {
        struct ifaddrs *next = cur->ifa_next ;
        if(cur->ifa_name != NULL)
            ::free(cur->ifa_name) ;
        if(cur->ifa_addr != NULL)
            ::free(cur->ifa_addr) ;
        if(cur->ifa_netmask != NULL)
            ::free(cur->ifa_netmask) ;
        if(cur->ifa_broadaddr != NULL)
            ::free(cur->ifa_broadaddr) ;

        ::free(cur) ;
        cur = next ;
    }
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

static bool get_ipforward_table(PMIB_IPFORWARDTABLE& pift) 
{
    ULONG size = 0;
    if (_imp_get_ipforward_table(NULL, &size, FALSE) != ERROR_INSUFFICIENT_BUFFER)
        return false ;

    pift = (PMIB_IPFORWARDTABLE)::malloc(size) ;
    ::memset(pift , 0 , size) ;

    DWORD result = _imp_get_ipforward_table(pift, &size, FALSE) ;
    if(result != NO_ERROR)
    {
        ::free(pift) ;
        return false ;
    }

    return true ;
}

static DWORD get_routedst(PMIB_IPFORWARDTABLE pift , DWORD ifidx) 
{
    for(DWORD idx = 0 ; idx < pift->dwNumEntries ; ++idx)
    {
        if (pift->table[idx].dwForwardIfIndex == ifidx && pift->table[idx].dwForwardMask == INADDR_BROADCAST)
            return pift->table[idx].dwForwardDest;
    }
    return 0 ;
}

static struct ifaddrs * create_pfpacket(IP_ADAPTER_ADDRESSES * adapter)
{
    size_t ifsize = sizeof(struct ifaddrs) ;
    struct ifaddrs * ifa = (struct ifaddrs *)::malloc(ifsize) ;
    ::memset(ifa , 0 , ifsize) ;

    ifa->ifa_name = ::strdup(adapter->AdapterName) ;
    ifa->ifa_flags = get_if_flags(adapter) ;

    size_t lsize = sizeof(struct sockaddr_ll) ;

    if(ifa->ifa_addr == NULL)
    {
        ifa->ifa_addr = (struct sockaddr *)::malloc(lsize) ;
        ::memset(ifa->ifa_addr , 0 , lsize) ;
    }
    struct sockaddr_ll * sll = (struct sockaddr_ll *)ifa->ifa_addr ;
    sll->sll_family = AF_PACKET ;
    sll->sll_ifindex = (int)adapter->IfIndex ;
    ::memcpy(sll->sll_addr , adapter->PhysicalAddress , adapter->PhysicalAddressLength) ;
    sll->sll_halen = (unsigned char)adapter->PhysicalAddressLength ;
    sll->sll_hatype = 1 ;
    if(adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
        sll->sll_hatype = 772 ;

    if(ifa->ifa_broadaddr == NULL)
    {
        ifa->ifa_broadaddr = (struct sockaddr *)::malloc(lsize) ;
        ::memset(ifa->ifa_broadaddr , 0 , lsize) ;
    }
    sll = (struct sockaddr_ll *)ifa->ifa_broadaddr ;
    sll->sll_family = AF_PACKET ;
    sll->sll_ifindex = (int)adapter->IfIndex ;
    sll->sll_halen = (unsigned char)adapter->PhysicalAddressLength ;
    if(adapter->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
        sll->sll_hatype = 772 ;
    else
    {
        sll->sll_hatype = 1 ;
        ::memset(sll->sll_addr , -1 , sll->sll_halen) ;
    }

    return ifa ;
}

static struct ifaddrs * create_inet(PMIB_IPFORWARDTABLE pift , IP_ADAPTER_ADDRESSES * adapter)
{
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
        ::memcpy(ifa->ifa_addr , wsa , sizeof(struct sockaddr)) ;

        //计算掩码
        if(ifa->ifa_netmask == NULL)
        {
            ifa->ifa_netmask = (struct sockaddr *)::malloc(sizeof(struct sockaddr)) ;
            ::memset(ifa->ifa_netmask , 0 , sizeof(struct sockaddr)) ;
        }
        struct sockaddr_in * sin = (struct sockaddr_in *)ifa->ifa_netmask ;
        int prefix = (int)unicast->OnLinkPrefixLength ;
        sin->sin_family = AF_INET ;
        sin->sin_addr.s_addr = ::htonl(UINT32_MAX << (32 - prefix)) ;

        //计算广播地址
        if(ifa->ifa_broadaddr == NULL)
        {
            ifa->ifa_broadaddr = (struct sockaddr *)::malloc(sizeof(struct sockaddr)) ;
            ::memset(ifa->ifa_broadaddr , 0 , sizeof(struct sockaddr)) ;
        }

        sin = (struct sockaddr_in *)ifa->ifa_broadaddr ;
        sin->sin_family = AF_INET ;
        sin->sin_addr.s_addr = (in_addr_t)get_routedst(pift , adapter->IfIndex) ;
    }

    return ifa ;
}

static void link_ifaddrs(struct ifaddrs *& root , struct ifaddrs *& last , struct ifaddrs *&node) 
{
    if(root == NULL)
        root = node ;
    if(last != NULL)
        last->ifa_next = node ;
    last = node ;        
}


#include <netinet/in.h>
#include <byteswap.h>
#include <endian.h>

uint32_t ntohl (uint32_t netlong)
{
    if(is_little_endian() == true)
        return bswap_32(netlong) ;
    else
        return netlong ;
}

uint16_t ntohs (uint16_t netshort)
{
    if(is_little_endian() == true)
        return bswap_16(netshort) ;
    else
        return netshort ;
}


uint32_t htonl (uint32_t hostlong)
{
    if(is_little_endian() == true)
        return bswap_32(hostlong) ;
    else
        return hostlong ;
}

uint16_t htons (uint16_t hostshort)
{
    if(is_little_endian() == true)
        return bswap_16(hostshort) ;
    else
        return hostshort ;
}

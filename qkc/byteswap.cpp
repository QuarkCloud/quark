
#include <byteswap.h>

uint16_t bswap_16 (uint16_t x)
{
    uint16_t j;
    j =  (x << 8) ;
    j += (x >> 8) ;
    return j;
}

uint32_t bswap_32 (uint32_t x)
{
    uint32_t j;
    j =  (x << 24);
    j += (x <<  8) & 0x00FF0000;
    j += (x >>  8) & 0x0000FF00;
    j += (x >> 24);
    return j;
}

uint64_t bswap_64 (uint64_t x)
{
    uint64_t j;
    j =  (x << 56);
    j += (x << 40)&0x00FF000000000000;
    j += (x << 24)&0x0000FF0000000000;
    j += (x <<  8)&0x000000FF00000000;
    j += (x >>  8)&0x00000000FF000000;
    j += (x >> 24)&0x0000000000FF0000;
    j += (x >> 40)&0x000000000000FF00;
    j += (x >> 56);
    return j;
}

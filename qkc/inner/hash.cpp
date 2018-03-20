
#include "hash.h"

uint32_t hash_time33(const char * str , int size , uint32_t hash) 
{
    const uint8_t * p = (const uint8_t *)str ;
    if(size < 0)
    {
        int i = 0 ;
        for(; (*p) != '\0' ; ++p)
            hash = (hash << 5) + hash + (*p) ;
    }
    else
    {
        for(int i = size ; i >= 0 ; --i , ++p)
        {
            hash = (hash << 5) + hash + (*p) ;
        }
    }

    return hash ;
}

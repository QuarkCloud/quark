
#include "string_util.h"

size_t str_split(const char * str , const char  seperator , str_seg_t * segs , size_t count)
{
    if(str == NULL)
        return 0 ;

    size_t size = 0 , offset = 0;
    const char * start = str ;
    for(int idx = 0 ; ; ++idx)
    {
        char ch = str[idx] ;
        if(ch == seperator || ch == '\0')
        {
            if(offset >= count)
                break ;

            if(size != 0)
            {
                segs[offset].start = start ;
                segs[offset].size = size ;
                ++offset ;
            }
            if(ch == '\0')
                break ;

            start = str + idx + 1;
            size = 0 ;
        }
        else
            ++size ;
    }

    return offset ;
}

int str_ncmp(const char * src , size_t nsize , const char * dst) 
{
    if(src == NULL)
    {
        if(dst == NULL)
            return 0 ;
        else
            return -1 ;
    }
    if(dst == NULL)
        return 1 ;

    for(size_t idx = 0 ; idx < nsize ; ++idx)
    {
        char ch = src[idx] - dst[idx] ;
        if(ch != 0)
            return ch ;

        if(dst[idx] == '\0')
            return 0 ;
    }

    if(dst[nsize] == '\0')
        return 0 ;
    else
        return -1 ;
}

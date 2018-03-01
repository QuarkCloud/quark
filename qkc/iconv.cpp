

#include <iconv.h>

iconv_t iconv_open (const char * to , const char * from)
{
    return NULL ;
}

size_t iconv (iconv_t cd , char ** inbuf , size_t * inleft,     char ** outbuf, size_t * outleft)
{
    return 0 ;
}

int iconv_close (iconv_t cd)
{
    return 0 ;
}

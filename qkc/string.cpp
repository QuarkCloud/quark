
#include <string.h>
#include "wobjs/CrtLinker.h"



char *strtok_r (char * s, const char * delim,  char ** save_ptr)
{
    return strtok_s(s , delim , save_ptr) ;
}


char *strerror_r (int errnum , char * buf, size_t buflen)
{
    if(strerror_s(buf , buflen , errnum) == 0)
        return buf ;
    else
        return NULL ;
}

#include <string.h>
#include <wintf/wcrt.h>


__BEGIN_DECLS

QUARK_LINKAGE errno_t strerror_s(char * buf, size_t size , int errnum);

__END_DECLS


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
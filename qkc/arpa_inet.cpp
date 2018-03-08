
#include <arpa/inet.h>
#include <winsock2.h>

in_addr_t inet_addr (const char * cp)
{
    return _imp_inet_addr(cp) ;
}

char * inet_ntoa (struct in_addr in)
{
    IN_ADDR wi ;
    wi.s_addr = in.s_addr ;
    return _imp_inet_ntoa(wi) ;
}

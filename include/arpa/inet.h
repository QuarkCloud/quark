#ifndef __QKC_ARPA_INET_H
#define __QKC_ARPA_INET_H 1

#include <quark_compile.h>
#include <netinet/in.h>	

__BEGIN_DECLS

QKCAPI in_addr_t inet_addr (const char * cp) ;
QKCAPI char *inet_ntoa (struct in_addr in) ;

__END_DECLS

#endif /** __QKC_ARPA_INET_H */

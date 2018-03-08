#ifndef __QKC_ARPA_INET_H
#define __QKC_ARPA_INET_H 1

#include <netinet/in.h>	

#ifdef __cplusplus
extern "C" {
#endif

QKCAPI in_addr_t inet_addr (const char * cp) ;
QKCAPI char *inet_ntoa (struct in_addr in) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_ARPA_INET_H */

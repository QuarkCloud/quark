

#ifndef __QKC_NETDB_H
#define __QKC_NETDB_H 1

#include <quark_compile.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct addrinfo
{
  int ai_flags;			/* Input flags.  */
  int ai_family;		/* Protocol family for socket.  */
  int ai_socktype;		/* Socket type.  */
  int ai_protocol;		/* Protocol for socket.  */
  socklen_t ai_addrlen;		/* Length of socket address.  */
  struct sockaddr *ai_addr;	/* Socket address for socket.  */
  char *ai_canonname;		/* Canonical name for service location.  */
  struct addrinfo *ai_next;	/* Pointer to next in list.  */
};

QKCAPI int getaddrinfo (const char *name,const char *service,const struct addrinfo *req,struct addrinfo **pai);

QKCAPI void freeaddrinfo (struct addrinfo * ai) ;

QKCAPI int getnameinfo (const struct sockaddr *sa,socklen_t salen, char *host, socklen_t hostlen, 
            char *serv, socklen_t servlen, int flags);

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_NETDB_H */

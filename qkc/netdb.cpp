
#include <netdb.h>

int getaddrinfo (const char *name,const char *service,const struct addrinfo *req,struct addrinfo **pai)
{
    return 0 ;
}

void freeaddrinfo (struct addrinfo * ai) 
{
    //
}

int getnameinfo (const struct sockaddr *sa,socklen_t salen, char *host, socklen_t hostlen, 
            char *serv, socklen_t servlen, int flags)
{
    return 0 ;
}

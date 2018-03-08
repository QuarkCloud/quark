
#include <winsock2.h>
#include <windows.h>

typedef struct __st_lpfn_socket{
    LPFN_ACCEPT             lpfn_accept ;
    LPFN_BIND               lpfn_bind ;
    LPFN_CLOSESOCKET        lpfn_closesocket ;
    LPFN_CONNECT            lpfn_connect ;
    LPFN_IOCTLSOCKET        lpfn_ioctlsocket ;
    LPFN_GETPEERNAME        lpfn_getpeername ;
    LPFN_GETSOCKNAME        lpfn_getsockname ;
    LPFN_GETSOCKOPT         lpfn_getsockopt ;
    LPFN_HTONL              lpfn_htonl ;
    LPFN_HTONS              lpfn_htons ;
    LPFN_INET_ADDR          lpfn_inet_addr ;
    LPFN_INET_NTOA          lpfn_inet_ntoa ;
    LPFN_LISTEN             lpfn_listen ;
    LPFN_NTOHL              lpfn_ntohl ;
    LPFN_NTOHS              lpfn_ntohs ;
    LPFN_RECV               lpfn_recv ;
    LPFN_RECVFROM           lpfn_recvfrom ;
    LPFN_SEND               lpfn_send ;
    LPFN_SENDTO             lpfn_sendto ;
    LPFN_SETSOCKOPT         lpfn_setsockopt ;
    LPFN_SHUTDOWN           lpfn_shutdown ;
    LPFN_SOCKET             lpfn_socket ;
    LPFN_GETHOSTBYADDR      lpfn_gethostbyaddr ;
    LPFN_GETHOSTBYNAME      lpfn_gethostbyname ;
    LPFN_GETHOSTNAME        lpfn_gethostname ;
    LPFN_GETSERVBYPORT      lpfn_getservbyport ;
    LPFN_GETSERVBYNAME      lpfn_getservbyname ;
    LPFN_GETPROTOBYNUMBER   lpfn_getprotobynumber ;
    LPFN_GETPROTOBYNAME     lpfn_getprotobyname ;
    LPFN_WSASTARTUP         lpfn_WSAStartup ;
    LPFN_WSACLEANUP         lpfn_WSACleanup ;
} lpfn_socket_t ;

SRWLOCK __socket_inner_rwlock__ =  SRWLOCK_INIT ;
static bool __socket_inner_rwlock_inited__ = false ;
static lpfn_socket_t __socket_pfns__ ;

#define DECLARE_SOCKET_PFN(name , type) __socket_pfns__.lpfn_##name = (type)::GetProcAddress(module , #name)

bool socket_library_load()
{
    HMODULE module = ::LoadLibrary("ws2_32.dll") ;
    if(module == NULL)
        return false ;
    
    DECLARE_SOCKET_PFN(accept ,             LPFN_ACCEPT);
    DECLARE_SOCKET_PFN(bind ,               LPFN_BIND);
    DECLARE_SOCKET_PFN(closesocket ,        LPFN_CLOSESOCKET);
    DECLARE_SOCKET_PFN(connect ,            LPFN_CONNECT);
    DECLARE_SOCKET_PFN(ioctlsocket ,        LPFN_IOCTLSOCKET);

    DECLARE_SOCKET_PFN(getpeername ,        LPFN_GETPEERNAME);
    DECLARE_SOCKET_PFN(getsockname ,        LPFN_GETSOCKNAME);
    DECLARE_SOCKET_PFN(getsockopt ,         LPFN_GETSOCKOPT);
    DECLARE_SOCKET_PFN(htonl ,              LPFN_HTONL);
    DECLARE_SOCKET_PFN(htons ,              LPFN_HTONS);
    
    DECLARE_SOCKET_PFN(inet_addr ,          LPFN_INET_ADDR);
    DECLARE_SOCKET_PFN(inet_ntoa ,          LPFN_INET_NTOA);
    DECLARE_SOCKET_PFN(listen ,             LPFN_LISTEN);
    DECLARE_SOCKET_PFN(ntohl ,              LPFN_NTOHL);
    DECLARE_SOCKET_PFN(ntohs ,              LPFN_NTOHS);
    
    DECLARE_SOCKET_PFN(recv ,               LPFN_RECV);
    DECLARE_SOCKET_PFN(recvfrom ,           LPFN_RECVFROM);
    DECLARE_SOCKET_PFN(send ,               LPFN_SEND);
    DECLARE_SOCKET_PFN(sendto ,             LPFN_SENDTO);
    DECLARE_SOCKET_PFN(setsockopt ,         LPFN_SETSOCKOPT);
    
    DECLARE_SOCKET_PFN(shutdown ,           LPFN_SHUTDOWN);
    DECLARE_SOCKET_PFN(socket ,             LPFN_SOCKET);
    DECLARE_SOCKET_PFN(gethostbyaddr ,      LPFN_GETHOSTBYADDR);
    DECLARE_SOCKET_PFN(gethostbyname ,      LPFN_GETHOSTBYNAME);
    DECLARE_SOCKET_PFN(gethostname ,        LPFN_GETHOSTNAME);
    
    DECLARE_SOCKET_PFN(getservbyport ,      LPFN_GETSERVBYPORT);
    DECLARE_SOCKET_PFN(getservbyname ,      LPFN_GETSERVBYNAME);
    DECLARE_SOCKET_PFN(getprotobynumber ,   LPFN_GETPROTOBYNUMBER);
    DECLARE_SOCKET_PFN(getprotobyname ,     LPFN_GETPROTOBYNAME);
    DECLARE_SOCKET_PFN(WSAStartup ,         LPFN_WSASTARTUP);
    
    DECLARE_SOCKET_PFN(WSACleanup ,         LPFN_WSACLEANUP);

    return  true ;
}

bool socket_library_init()
{
    if(__socket_inner_rwlock_inited__ == false)
    {
        ::AcquireSRWLockExclusive(&__socket_inner_rwlock__) ;

        if(__socket_inner_rwlock_inited__ == false)
            __socket_inner_rwlock_inited__ = socket_library_load() ;
        ::ReleaseSRWLockExclusive(&__socket_inner_rwlock__) ;
    }

    return __socket_inner_rwlock_inited__ ;
}



SOCKET _imp_accept(SOCKET s , struct sockaddr FAR * addr,  int FAR * addrlen)
{
    if(socket_library_init() == false)
        return INVALID_SOCKET ;

    return __socket_pfns__.lpfn_accept(s , addr , addrlen) ;
}

int _imp_bind(SOCKET s, const struct sockaddr FAR * name,  int namelen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_bind(s , name , namelen) ;
}

int _imp_closesocket(SOCKET s)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_closesocket(s) ;
}

int _imp_connect(SOCKET s,const struct sockaddr FAR * name,int namelen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_connect(s , name , namelen) ;
}

int _imp_ioctlsocket(SOCKET s,long cmd, u_long FAR * argp)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_ioctlsocket(s , cmd , argp) ;
}

int _imp_getpeername(SOCKET s,struct sockaddr FAR * name, int FAR * namelen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_getpeername(s , name , namelen) ;
}

int _imp_getsockname(SOCKET s,struct sockaddr FAR * name, int FAR * namelen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_getsockname(s , name , namelen) ;
}

int _imp_getsockopt(SOCKET s,int level,int optname,char FAR * optval, int FAR * optlen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_getsockopt(s , level , optname , optval , optlen) ;
}

u_long _imp_htonl(u_long hostlong)
{
    if(socket_library_init() == false)
        return 0 ;

    return __socket_pfns__.lpfn_htonl(hostlong) ;
}

u_short _imp_htons(u_short hostshort)
{
    if(socket_library_init() == false)
        return 0 ;

    return __socket_pfns__.lpfn_htons(hostshort) ;
}

unsigned long _imp_inet_addr( const char FAR * cp)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_inet_addr(cp) ;
}

char FAR * _imp_inet_ntoa(IN_ADDR in)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_inet_ntoa(in) ;
}

int _imp_listen(SOCKET s,int backlog)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_listen(s , backlog) ;
}

u_long _imp_ntohl(u_long netlong)
{
    if(socket_library_init() == false)
        return 0 ;

    return __socket_pfns__.lpfn_ntohl(netlong) ;
}

u_short _imp_ntohs(u_short netshort)
{
    if(socket_library_init() == false)
        return 0 ;

    return __socket_pfns__.lpfn_ntohs(netshort) ;
}

int _imp_recv(SOCKET s, char FAR * buf, int len,int flags)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_recv(s , buf , len , flags) ;
}

int _imp_recvfrom(SOCKET s,char FAR * buf,int len,int flags,struct sockaddr FAR * from, int FAR * fromlen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_recvfrom(s , buf , len , flags , from , fromlen) ;
}

int _imp_send(SOCKET s,const char FAR * buf,int len,int flags)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_send(s , buf , len , flags) ;
}

int _imp_sendto(SOCKET s,const char FAR * buf,int len, int flags, const struct sockaddr FAR * to,int tolen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_sendto(s , buf , len , flags , to , tolen) ;
}

int _imp_setsockopt(SOCKET s,int level,int optname,const char FAR * optval,int optlen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_setsockopt(s , level , optname , optval , optlen) ;
}

int _imp_shutdown(SOCKET s,int how)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_shutdown(s , how) ;
}

SOCKET _imp_socket(int af,int type,int protocol)
{
    if(socket_library_init() == false)
        return INVALID_SOCKET ;

    return __socket_pfns__.lpfn_socket(af , type , protocol) ;
}

struct hostent FAR * _imp_gethostbyaddr( const char FAR * addr,int len, int type)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_gethostbyaddr(addr , len , type) ;
}

struct hostent FAR * _imp_gethostbyname( const char FAR * name)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_gethostbyname(name) ;
}

int _imp_gethostname(char FAR * name,int namelen)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_gethostname(name , namelen) ;
}

struct servent FAR * _imp_getservbyport( int port, const char FAR * proto)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_getservbyport(port , proto) ;
}

struct servent FAR * _imp_getservbyname( const char FAR * name,  const char FAR * proto)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_getservbyname(name , proto) ;
}

struct protoent FAR * _imp_getprotobynumber(int number)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_getprotobynumber(number) ;
}

struct protoent FAR * _imp_getprotobyname(const char FAR * name)
{
    if(socket_library_init() == false)
        return NULL ;

    return __socket_pfns__.lpfn_getprotobyname(name) ;
}

int _imp_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_WSAStartup(wVersionRequested , lpWSAData) ;
}

int _imp_WSACleanup(void)
{
    if(socket_library_init() == false)
        return SOCKET_ERROR ;

    return __socket_pfns__.lpfn_WSACleanup() ;
}

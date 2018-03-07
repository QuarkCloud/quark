
#ifndef __QUARK_WSTUB_WINSOCK2_H
#define __QUARK_WSTUB_WINSOCK2_H 1

#include <windef.h>
#include <winerror.h>
#include <winnt.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

typedef UINT_PTR        SOCKET;
#define WSAAPI          FAR PASCAL


#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

typedef struct WSAData {
        WORD                    wVersion;
        WORD                    wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
#endif
} WSADATA, FAR * LPWSADATA;

/* Socket function prototypes */
SOCKET WSAAPI accept(IN SOCKET s , struct sockaddr FAR * addr, __inout_opt int FAR * addrlen);
typedef SOCKET (WSAAPI * LPFN_ACCEPT)(IN SOCKET s, struct sockaddr FAR * addr, __inout_opt int FAR * addrlen);

int WSAAPI bind(IN SOCKET s, const struct sockaddr FAR * name,  IN int namelen);
typedef int (WSAAPI * LPFN_BIND)(IN SOCKET s, const struct sockaddr FAR * name,IN int namelen);

int WSAAPI closesocket(IN SOCKET s);
typedef int (WSAAPI * LPFN_CLOSESOCKET)(IN SOCKET s);

int WSAAPI connect(IN SOCKET s,const struct sockaddr FAR * name,IN int namelen);
typedef int (WSAAPI * LPFN_CONNECT)(IN SOCKET s,const struct sockaddr FAR * name,IN int namelen);

int WSAAPI ioctlsocket(IN SOCKET s,IN long cmd,__inout u_long FAR * argp);
typedef int (WSAAPI * LPFN_IOCTLSOCKET)(IN SOCKET s,IN long cmd,__inout u_long FAR * argp);

int WSAAPI getpeername(IN SOCKET s,struct sockaddr FAR * name,__inout int FAR * namelen);
typedef int (WSAAPI * LPFN_GETPEERNAME)(IN SOCKET s,struct sockaddr FAR * name,__inout int FAR * namelen);

int WSAAPI getsockname(IN SOCKET s,struct sockaddr FAR * name,__inout int FAR * namelen);
typedef int (WSAAPI * LPFN_GETSOCKNAME)(IN SOCKET s,struct sockaddr FAR * name,__inout int FAR * namelen);

int WSAAPI getsockopt(IN SOCKET s,IN int level,IN int optname,char FAR * optval,__inout int FAR * optlen);
typedef int (WSAAPI * LPFN_GETSOCKOPT)(IN SOCKET s,IN int level,IN int optname,char FAR * optval,__inout int FAR * optlen);

u_long WSAAPI htonl(IN u_long hostlong);
typedef u_long (WSAAPI * LPFN_HTONL)(IN u_long hostlong);

u_short WSAAPI htons(IN u_short hostshort);
typedef u_short (WSAAPI * LPFN_HTONS)(IN u_short hostshort);

unsigned long WSAAPI inet_addr(__in IN const char FAR * cp);
typedef unsigned long (WSAAPI * LPFN_INET_ADDR)(IN const char FAR * cp);

char FAR * WSAAPI inet_ntoa(IN struct in_addr in);
typedef char FAR * (WSAAPI * LPFN_INET_NTOA)(IN struct in_addr in);

int WSAAPI listen(IN SOCKET s,IN int backlog);
typedef int (WSAAPI * LPFN_LISTEN)(IN SOCKET s,IN int backlog);

u_long WSAAPI ntohl(IN u_long netlong);
typedef u_long (WSAAPI * LPFN_NTOHL)(IN u_long netlong);

u_short WSAAPI ntohs(IN u_short netshort);
typedef u_short (WSAAPI * LPFN_NTOHS)(IN u_short netshort);

int WSAAPI recv(IN SOCKET s, char FAR * buf, IN int len,IN int flags);
typedef int (WSAAPI * LPFN_RECV)(IN SOCKET s,char FAR * buf,IN int len,IN int flags);

int WSAAPI recvfrom(IN SOCKET s,char FAR * buf,IN int len,IN int flags,struct sockaddr FAR * from,__inout_opt int FAR * fromlen);
typedef int (WSAAPI * LPFN_RECVFROM)(IN SOCKET s,char FAR * buf,IN int len,IN int flags,struct sockaddr FAR * from,__inout_opt int FAR * fromlen);

int WSAAPI send(IN SOCKET s,const char FAR * buf,IN int len,IN int flags);
typedef int (WSAAPI * LPFN_SEND)( IN SOCKET s,const char FAR * buf,IN int len,IN int flags);

int WSAAPI sendto(IN SOCKET s,const char FAR * buf,IN int len, IN int flags, const struct sockaddr FAR * to,IN int tolen);
typedef int (WSAAPI * LPFN_SENDTO)(IN SOCKET s,const char FAR * buf,IN int len,IN int flags,const struct sockaddr FAR * to,IN int tolen);

int WSAAPI setsockopt(IN SOCKET s,IN int level,IN int optname,const char FAR * optval,IN int optlen);
typedef int (WSAAPI * LPFN_SETSOCKOPT)(IN SOCKET s,IN int level,IN int optname,const char FAR * optval,IN int optlen);

int WSAAPI shutdown(IN SOCKET s,IN int how);
typedef int (WSAAPI * LPFN_SHUTDOWN)(IN SOCKET s,IN int how);

SOCKET WSAAPI socket(IN int af,IN int type,IN int protocol);
typedef SOCKET (WSAAPI * LPFN_SOCKET)(IN int af,IN int type,IN int protocol);

struct hostent FAR * WSAAPI gethostbyaddr( const char FAR * addr,IN int len, IN int type);
typedef struct hostent FAR * (WSAAPI * LPFN_GETHOSTBYADDR)(const char FAR * addr,IN int len,IN int type);

struct hostent FAR * WSAAPI gethostbyname(__in const char FAR * name);
typedef struct hostent FAR * (WSAAPI * LPFN_GETHOSTBYNAME)( __in const char FAR * name);

int WSAAPI gethostname(char FAR * name,IN int namelen);
typedef int (WSAAPI * LPFN_GETHOSTNAME)(char FAR * name,IN int namelen);

struct servent FAR * WSAAPI getservbyport(__in int port,__in const char FAR * proto);
typedef struct servent FAR * (WSAAPI * LPFN_GETSERVBYPORT)(IN int port, __in const char FAR * proto);

struct servent FAR * WSAAPI getservbyname(__in const char FAR * name, __in const char FAR * proto);
typedef struct servent FAR * (WSAAPI * LPFN_GETSERVBYNAME)( __in const char FAR * name,__in const char FAR * proto);

struct protoent FAR * WSAAPI getprotobynumber(IN int number);
typedef struct protoent FAR * (WSAAPI * LPFN_GETPROTOBYNUMBER)(IN int number);

struct protoent FAR * WSAAPI getprotobyname(IN const char FAR * name);
typedef struct protoent FAR * (WSAAPI * LPFN_GETPROTOBYNAME)(__in const char FAR * name);

int WSAAPI WSAStartup(IN WORD wVersionRequested,OUT LPWSADATA lpWSAData);
typedef int (WSAAPI * LPFN_WSASTARTUP)(IN WORD wVersionRequested,OUT LPWSADATA lpWSAData);

int WSAAPI WSACleanup(void);
typedef int (WSAAPI * LPFN_WSACLEANUP)(void);

#ifdef __cplusplus
}
#endif

#endif  /** __QUARK_WSTUB_WINSOCK2_H */

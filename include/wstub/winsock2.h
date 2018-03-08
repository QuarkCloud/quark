
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

typedef struct win_addr {
    u_int s_addr ;
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;

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

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)


#if !defined(MAKEWORD)
#define MAKEWORD(low,high) \
        ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#endif

#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION MAKEWORD(2,2)
#endif



/* Socket function prototypes */
SOCKET _imp_accept(SOCKET s , struct sockaddr FAR * addr,  int FAR * addrlen);
typedef SOCKET (WSAAPI * LPFN_ACCEPT)(SOCKET s, struct sockaddr FAR * addr,  int FAR * addrlen);

int _imp_bind(SOCKET s, const struct sockaddr FAR * name,  int namelen);
typedef int (WSAAPI * LPFN_BIND)(SOCKET s, const struct sockaddr FAR * name,int namelen);

int _imp_closesocket(SOCKET s);
typedef int (WSAAPI * LPFN_CLOSESOCKET)(SOCKET s);

int _imp_connect(SOCKET s,const struct sockaddr FAR * name,int namelen);
typedef int (WSAAPI * LPFN_CONNECT)(SOCKET s,const struct sockaddr FAR * name,int namelen);

int _imp_ioctlsocket(SOCKET s,long cmd, u_long FAR * argp);
typedef int (WSAAPI * LPFN_IOCTLSOCKET)(SOCKET s,long cmd, u_long FAR * argp);

int _imp_getpeername(SOCKET s,struct sockaddr FAR * name, int FAR * namelen);
typedef int (WSAAPI * LPFN_GETPEERNAME)(SOCKET s,struct sockaddr FAR * name, int FAR * namelen);

int _imp_getsockname(SOCKET s,struct sockaddr FAR * name, int FAR * namelen);
typedef int (WSAAPI * LPFN_GETSOCKNAME)(SOCKET s,struct sockaddr FAR * name, int FAR * namelen);

int _imp_getsockopt(SOCKET s,int level,int optname,char FAR * optval, int FAR * optlen);
typedef int (WSAAPI * LPFN_GETSOCKOPT)(SOCKET s,int level,int optname,char FAR * optval, int FAR * optlen);

u_long _imp_htonl(u_long hostlong);
typedef u_long (WSAAPI * LPFN_HTONL)(u_long hostlong);

u_short _imp_htons(u_short hostshort);
typedef u_short (WSAAPI * LPFN_HTONS)(u_short hostshort);

unsigned long _imp_inet_addr( const char FAR * cp);
typedef unsigned long (WSAAPI * LPFN_INET_ADDR)(const char FAR * cp);

char FAR * _imp_inet_ntoa(IN_ADDR in);
typedef char FAR * (WSAAPI * LPFN_INET_NTOA)(IN_ADDR in);

int _imp_listen(SOCKET s,int backlog);
typedef int (WSAAPI * LPFN_LISTEN)(SOCKET s,int backlog);

u_long _imp_ntohl(u_long netlong);
typedef u_long (WSAAPI * LPFN_NTOHL)(u_long netlong);

u_short _imp_ntohs(u_short netshort);
typedef u_short (WSAAPI * LPFN_NTOHS)(u_short netshort);

int _imp_recv(SOCKET s, char FAR * buf, int len,int flags);
typedef int (WSAAPI * LPFN_RECV)(SOCKET s,char FAR * buf,int len,int flags);

int _imp_recvfrom(SOCKET s,char FAR * buf,int len,int flags,struct sockaddr FAR * from, int FAR * fromlen);
typedef int (WSAAPI * LPFN_RECVFROM)(SOCKET s,char FAR * buf,int len,int flags,struct sockaddr FAR * from, int FAR * fromlen);

int _imp_send(SOCKET s,const char FAR * buf,int len,int flags);
typedef int (WSAAPI * LPFN_SEND)( SOCKET s,const char FAR * buf,int len,int flags);

int _imp_sendto(SOCKET s,const char FAR * buf,int len, int flags, const struct sockaddr FAR * to,int tolen);
typedef int (WSAAPI * LPFN_SENDTO)(SOCKET s,const char FAR * buf,int len,int flags,const struct sockaddr FAR * to,int tolen);

int _imp_setsockopt(SOCKET s,int level,int optname,const char FAR * optval,int optlen);
typedef int (WSAAPI * LPFN_SETSOCKOPT)(SOCKET s,int level,int optname,const char FAR * optval,int optlen);

int _imp_shutdown(SOCKET s,int how);
typedef int (WSAAPI * LPFN_SHUTDOWN)(SOCKET s,int how);

SOCKET _imp_socket(int af,int type,int protocol);
typedef SOCKET (WSAAPI * LPFN_SOCKET)(int af,int type,int protocol);

struct hostent FAR * _imp_gethostbyaddr( const char FAR * addr,int len, int type);
typedef struct hostent FAR * (WSAAPI * LPFN_GETHOSTBYADDR)(const char FAR * addr,int len,int type);

struct hostent FAR * _imp_gethostbyname( const char FAR * name);
typedef struct hostent FAR * (WSAAPI * LPFN_GETHOSTBYNAME)(  const char FAR * name);

int _imp_gethostname(char FAR * name,int namelen);
typedef int (WSAAPI * LPFN_GETHOSTNAME)(char FAR * name,int namelen);

struct servent FAR * _imp_getservbyport( int port, const char FAR * proto);
typedef struct servent FAR * (WSAAPI * LPFN_GETSERVBYPORT)(int port,  const char FAR * proto);

struct servent FAR * _imp_getservbyname( const char FAR * name,  const char FAR * proto);
typedef struct servent FAR * (WSAAPI * LPFN_GETSERVBYNAME)(  const char FAR * name, const char FAR * proto);

struct protoent FAR * _imp_getprotobynumber(int number);
typedef struct protoent FAR * (WSAAPI * LPFN_GETPROTOBYNUMBER)(int number);

struct protoent FAR * _imp_getprotobyname(const char FAR * name);
typedef struct protoent FAR * (WSAAPI * LPFN_GETPROTOBYNAME)( const char FAR * name);

int _imp_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef int (WSAAPI * LPFN_WSASTARTUP)(WORD wVersionRequested, LPWSADATA lpWSAData);

int _imp_WSACleanup(void);
typedef int (WSAAPI * LPFN_WSACLEANUP)(void);

#ifdef __cplusplus
}
#endif

#endif  /** __QUARK_WSTUB_WINSOCK2_H */

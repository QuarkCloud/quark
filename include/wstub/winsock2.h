
#ifndef __QUARK_WSTUB_WINSOCK2_H
#define __QUARK_WSTUB_WINSOCK2_H 1

#include <windows.h>

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

typedef struct _OVERLAPPED *    LPWSAOVERLAPPED;
#define WSAEVENT                HANDLE



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

int WSAStartup(WORD version, LPWSADATA data);
typedef int (WSAAPI * LPFN_WSASTARTUP)(WORD version, LPWSADATA data);

int WSACleanup(void);
typedef int (WSAAPI * LPFN_WSACLEANUP)(void);


typedef struct _WSABUF {
    ULONG len;     /* the length of the buffer */
    CHAR FAR *buf; /* the pointer to the buffer */
} WSABUF, FAR * LPWSABUF;
typedef void(CALLBACK * LPWSAOVERLAPPED_COMPLETION_ROUTINE)(DWORD dwError , DWORD cbTransferred , LPWSAOVERLAPPED lpOverlapped , DWORD dwFlags);
#define SIO_NSP_NOTIFY_CHANGE         _WSAIOW(IOC_WS2 , 25)
typedef enum _WSACOMPLETIONTYPE {
    NSP_NOTIFY_IMMEDIATELY = 0 , 
    NSP_NOTIFY_HWND , 
    NSP_NOTIFY_EVENT , 
    NSP_NOTIFY_PORT , 
    NSP_NOTIFY_APC , 
} WSACOMPLETIONTYPE , *PWSACOMPLETIONTYPE , FAR * LPWSACOMPLETIONTYPE;
typedef struct _WSACOMPLETION {
    WSACOMPLETIONTYPE Type;
    union {
        struct {
            LPWSAOVERLAPPED lpOverlapped;
        } Event;
        struct {
            LPWSAOVERLAPPED lpOverlapped;
            LPWSAOVERLAPPED_COMPLETION_ROUTINE lpfnCompletionProc;
        } Apc;
        struct {
            LPWSAOVERLAPPED lpOverlapped;
            HANDLE hPort;
            ULONG_PTR Key;
        } Port;
    } Parameters;
} WSACOMPLETION , *PWSACOMPLETION , FAR *LPWSACOMPLETION;

void _imp_WSASetLastError(int error);
typedef void(WSAAPI * LPFN_WSASETLASTERROR)(int error);

int _imp_WSAGetLastError(void);
typedef int(WSAAPI * LPFN_WSAGETLASTERROR)(void);

BOOL _imp_WSACloseEvent(WSAEVENT evt);
typedef BOOL(WSAAPI * LPFN_WSACLOSEEVENT)(WSAEVENT evt);

WSAEVENT _imp_WSACreateEvent(void);
typedef WSAEVENT(WSAAPI * LPFN_WSACREATEEVENT)(void);

int _imp_WSAEventSelect(SOCKET s , WSAEVENT evto , long nevt);
typedef int(WSAAPI * LPFN_WSAEVENTSELECT)(SOCKET s , WSAEVENT evto , long nevt);

BOOL _imp_WSAGetOverlappedResult(SOCKET s , LPWSAOVERLAPPED ov , LPDWORD transfer , BOOL wait , LPDWORD flag);
typedef BOOL(WSAAPI * LPFN_WSAGETOVERLAPPEDRESULT)(SOCKET s , LPWSAOVERLAPPED ov , LPDWORD transfer , BOOL wait , LPDWORD flag);

int _imp_WSAIoctl(SOCKET s , DWORD code , LPVOID inbuf , DWORD inbytes , LPVOID outbuf , DWORD outbytes , LPDWORD retbytes , 
                  LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);
typedef int(WSAAPI * LPFN_WSAIOCTL)(SOCKET s , DWORD code , LPVOID inbuf , DWORD inbytes , LPVOID outbuf , DWORD outbytes , 
                  LPDWORD retbytes , LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);

int _imp_WSARecv(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD recvbytes , LPDWORD flag , LPWSAOVERLAPPED ov ,
                LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);
typedef int(WSAAPI * LPFN_WSARECV)(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD recvbytes , LPDWORD flag , 
                LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);

int _imp_WSARecvFrom(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD recvbytes , LPDWORD flag , struct sockaddr FAR * from , 
                LPINT from_len , LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);
typedef int(WSAAPI * LPFN_WSARECVFROM)(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD recvbytes , LPDWORD flag , struct sockaddr FAR * from , 
                LPINT from_len , LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);

BOOL _imp_WSAResetEvent(WSAEVENT evt);
typedef BOOL(WSAAPI * LPFN_WSARESETEVENT)(WSAEVENT evt);

int _imp_WSASend(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD sentbytes , DWORD flag , LPWSAOVERLAPPED ov , 
                 LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);
typedef int(WSAAPI * LPFN_WSASEND)(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD sentbytes , DWORD flag , 
                LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);

int _imp_WSASendTo(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD sentbytes , DWORD flag , const struct sockaddr FAR * to , 
                int to_len , LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);
typedef int(WSAAPI * LPFN_WSASENDTO)(SOCKET s , LPWSABUF buf , DWORD bufsize , LPDWORD sentbytes , DWORD flag , 
                const struct sockaddr FAR * to , int to_len , LPWSAOVERLAPPED ov , LPWSAOVERLAPPED_COMPLETION_ROUTINE routine);

BOOL _imp_WSASetEvent(WSAEVENT evt);
typedef BOOL(WSAAPI * LPFN_WSASETEVENT)(WSAEVENT evt);


#define MAX_PROTOCOL_CHAIN 7

#define BASE_PROTOCOL      1
#define LAYERED_PROTOCOL   0

typedef struct _WSAPROTOCOLCHAIN {
    int ChainLen;                                 /* the length of the chain,     */
                                                  /* length = 0 means layered protocol, */
                                                  /* length = 1 means base protocol, */
                                                  /* length > 1 means protocol chain */
    DWORD ChainEntries[MAX_PROTOCOL_CHAIN];       /* a list of dwCatalogEntryIds */
} WSAPROTOCOLCHAIN, FAR * LPWSAPROTOCOLCHAIN;

#define WSAPROTOCOL_LEN  255

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;

typedef struct _WSAPROTOCOL_INFOA {
    DWORD dwServiceFlags1;
    DWORD dwServiceFlags2;
    DWORD dwServiceFlags3;
    DWORD dwServiceFlags4;
    DWORD dwProviderFlags;
    GUID ProviderId;
    DWORD dwCatalogEntryId;
    WSAPROTOCOLCHAIN ProtocolChain;
    int iVersion;
    int iAddressFamily;
    int iMaxSockAddr;
    int iMinSockAddr;
    int iSocketType;
    int iProtocol;
    int iProtocolMaxOffset;
    int iNetworkByteOrder;
    int iSecurityScheme;
    DWORD dwMessageSize;
    DWORD dwProviderReserved;
    CHAR   szProtocol[WSAPROTOCOL_LEN+1];
} WSAPROTOCOL_INFOA, FAR * LPWSAPROTOCOL_INFOA;
typedef struct _WSAPROTOCOL_INFOW {
    DWORD dwServiceFlags1;
    DWORD dwServiceFlags2;
    DWORD dwServiceFlags3;
    DWORD dwServiceFlags4;
    DWORD dwProviderFlags;
    GUID ProviderId;
    DWORD dwCatalogEntryId;
    WSAPROTOCOLCHAIN ProtocolChain;
    int iVersion;
    int iAddressFamily;
    int iMaxSockAddr;
    int iMinSockAddr;
    int iSocketType;
    int iProtocol;
    int iProtocolMaxOffset;
    int iNetworkByteOrder;
    int iSecurityScheme;
    DWORD dwMessageSize;
    DWORD dwProviderReserved;
    WCHAR  szProtocol[WSAPROTOCOL_LEN+1];
} WSAPROTOCOL_INFOW, FAR * LPWSAPROTOCOL_INFOW;

typedef unsigned int             GROUP;

SOCKET  _imp_WSASocket(int af , int type , int protocol , LPWSAPROTOCOL_INFOA info , GROUP g , DWORD flag);
typedef  SOCKET (WSAAPI * LPFN_WSASOCKET)(int af , int type , int protocol , LPWSAPROTOCOL_INFOA info , GROUP g , DWORD flag);

DWORD _imp_WSAWaitForMultipleEvents(DWORD evt_count , const WSAEVENT FAR * evts , BOOL wait_all , DWORD timeout , BOOL alertable);
typedef DWORD(WSAAPI * LPFN_WSAWAITFORMULTIPLEEVENTS)(DWORD evt_count , const WSAEVENT FAR * evts , BOOL wait_all , 
            DWORD timeout , BOOL alertable);

#ifdef __cplusplus
}
#endif

#endif  /** __QUARK_WSTUB_WINSOCK2_H */

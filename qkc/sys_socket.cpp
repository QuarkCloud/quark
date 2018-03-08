
#include <winsock2.h>
#include <sys/socket.h>
#include <wintf/wobj.h>

static inline SOCKET wobj_to_socket(int fd) {return (SOCKET)wobj_get(fd) ;}

int socket(int domain , int type , int protocol)
{
    SOCKET handle = _imp_socket(domain , type , protocol) ;
    if(handle == INVALID_SOCKET)
    {
        DWORD error_code = ::GetLastError() ;
        return -1 ;
    }

    return wobj_set(WOBJ_SOCK , (HANDLE)handle , NULL) ;
}

int bind(int fd , const struct sockaddr * addr , socklen_t len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_bind(handle , addr , len) ;
}

int getsockname(int fd , struct sockaddr * addr , socklen_t * len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_getsockname(handle , addr , (int *)len) ;
}

int connect(int fd , const struct sockaddr * addr , socklen_t len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_connect(handle , addr , (int)len) ;
}

int getpeername(int fd , struct sockaddr * addr , socklen_t * len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_getpeername(handle , addr , (int *)len) ;
}

ssize_t send(int fd , const void * buf , size_t n , int flags)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_send(handle , (const char *)buf , n , flags) ;
}

ssize_t recv(int fd , void *buf , size_t n , int flags)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_recv(handle , (char *)buf , n , flags) ;
}

ssize_t sendto(int fd , const void *buf , size_t n , int flags , const struct sockaddr * addr , socklen_t addr_len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_sendto(handle , (const char *)buf , n , flags , addr , addr_len) ;
}

ssize_t recvfrom(int fd , void * buf , size_t n , int flags , struct sockaddr * addr , socklen_t * addr_len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_recvfrom(handle , (char *)buf , n , flags , addr , (int *)addr_len) ;
}

int getsockopt(int fd , int level , int optname , void * optval , socklen_t * optlen)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_getsockopt(handle , level , optname , (char *)optval , (int *)optlen) ;
}

int setsockopt(int fd , int level , int optname , const void *optval , socklen_t optlen)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_setsockopt(handle , level , optname , (const char *)optval , (int)optlen) ;
}

int listen(int fd , int n)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_listen(handle , n) ;
}

int accept(int fd , struct sockaddr * addr , socklen_t * addr_len)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_accept(handle , addr , (int *)addr_len) ;
}

int shutdown(int fd , int how)
{
    SOCKET handle = wobj_to_socket(fd) ;
    if(handle == NULL)
        return -1 ;

    return ::_imp_shutdown(handle , how) ;
}

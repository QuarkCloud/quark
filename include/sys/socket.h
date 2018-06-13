#ifndef __QKC_SYS_SOCKET_H
#define __QKC_SYS_SOCKET_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>
#include <bits/socket.h>


/* The following constants should be used for the second parameter of
 `shutdown'.*/
enum
{
    SHUT_RD = 0 , 		/* No more receptions.*/
#define SHUT_RD		SHUT_RD
    SHUT_WR , 		/* No more transmissions.*/
#define SHUT_WR		SHUT_WR
    SHUT_RDWR		/* No more receptions or transmissions.*/
#define SHUT_RDWR	SHUT_RDWR
};



#ifdef __cplusplus
extern "C" {
#endif

QKCAPI int socket(int domain , int type , int protocol) ;

QKCAPI int bind(int fd , const struct sockaddr * addr , socklen_t len);

QKCAPI int getsockname(int fd , struct sockaddr * addr , socklen_t * len) ;

QKCAPI int connect(int fd , const struct sockaddr * addr , socklen_t len) ;

QKCAPI int getpeername(int fd , struct sockaddr * addr , socklen_t * len) ;

QKCAPI ssize_t send(int fd , const void * buf , size_t n , int flags);

QKCAPI ssize_t recv(int fd , void *buf , size_t n , int flags);

QKCAPI ssize_t sendto(int fd , const void *buf , size_t n , int flags , const struct sockaddr * addr , socklen_t addr_len);

QKCAPI ssize_t recvfrom(int fd , void * buf , size_t n , int flags , struct sockaddr * addr , socklen_t * addr_len);

QKCAPI int getsockopt(int fd , int level , int optname , void * optval , socklen_t * optlen) ;

QKCAPI int setsockopt(int fd , int level , int optname , const void *optval , socklen_t optlen) ;

QKCAPI int listen(int fd , int n) ;

QKCAPI int accept(int fd , struct sockaddr * addr , socklen_t * addr_len);

QKCAPI int shutdown(int fd , int how) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_SOCKET_H */

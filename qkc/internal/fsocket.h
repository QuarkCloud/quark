
#ifndef __QKC_INTERNAL_FSOCKET_H
#define __QKC_INTERNAL_FSOCKET_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>
#include "ring_buffer.h"
#include "iocp_item.h"

__BEGIN_DECLS

/**
    为了实现epoll特性，需要对原来的socket做些改造，添加附加信息，不能直接使用。
*/
#define SOCKET_STAGE_VOID       0 
#define SOCKET_STAGE_BIND       1
#define SOCKET_STAGE_LISTEN     2
#define SOCKET_STAGE_ACCEPT     3
#define SOCKET_STAGE_CONNECT    11
#define SOCKET_STAGE_DESROTY    -1

//typedef struct __st_socket_ovlp				socket_ovlp_t ;
typedef struct __st_socket_send_result		socket_send_result_t ;
typedef struct __st_socket_recv_result		socket_recv_result_t ;
typedef struct __st_socket_accept_result	socket_accept_result_t;
typedef struct __st_socket          socket_t;

int iocp_socket_callback(iocp_item_t * item , int evt , int result) ;
void iocp_socket_free(iocp_item_t * item) ;

typedef struct __st_socket{
    SOCKET socket ;
    int8_t type ;                  //SOCK_STREAM、SOCK_DGRAM
    int8_t stage ;  
    int8_t noblock :1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

    HANDLE            locker ;    //创建销毁保护
	socket_accept_result_t * acceptor ;
	socket_send_result_t   * sender ;
	socket_recv_result_t   * receiver ;

    void * addition ;               //附加的信息
} socket_t;

/**
    2018-04-19
    将iocp的preactor模式转换成epoll的actor难度比较大。主要体现在send时，不知道可以发送最大的字节数。
    send的可发字节数同时【记住是同时！！！】受收发双方的缓冲区大小限制，而不是单纯地由本地发送缓冲区决定。
    immediately返回的字节数只能做参考用，不是最终完成的字节数。当请求发送的字节数，不能马上返回时，
    缓冲区就会被锁定，造成阻塞，降低效率。因此，需要外挂一个缓冲区，来确保字节数是可控的。但因为拷贝会
    导致性能下降。相较于阻塞来说，应该反而会好一点。
    如果通过TCP将同一块数据同时向多个连接发送，那么性能损失会因为拷贝而下降更加严重。
*/
#define SNDBUFSIZE      8192
struct __st_socket_send_result{
    iocp_ovlp_t   link ;

    WSABUF          data ;
    ring_buffer_t   ring_buffer ;
};

struct __st_socket_recv_result{
    iocp_ovlp_t   link ;
    WSABUF data ;
};

#define ACCEPT_ADDRESS_SIZE 256
struct __st_socket_accept_result{
    iocp_ovlp_t   link ;

    SOCKET insocket ;           //接收到客户端连接请求的套接字   
    char address[ACCEPT_ADDRESS_SIZE] ;
    size_t bytes ;
};

void update_contex_acceptex(SOCKET& new_socket , SOCKET& listen_socket) ;

socket_t * socket_new() ;
bool socket_init(socket_t * s) ;
void socket_free(socket_t * s) ;
bool socket_final(socket_t * s) ;

socket_send_result_t * socket_send_result_new() ;
void socket_send_result_free(socket_send_result_t * result) ;
bool socket_send_result_init(socket_send_result_t * result) ;
bool socket_send_result_final(socket_send_result_t * result) ;

bool socket_send(socket_send_result_t * result , int flags) ;
bool socket_sendto(socket_send_result_t * result , int flags , const struct sockaddr * addr , int addr_len) ;

socket_recv_result_t * socket_recv_result_new() ;
void socket_recv_result_free(socket_recv_result_t * result) ;
bool socket_recv_result_init(socket_recv_result_t * result) ;
bool socket_recv_result_final(socket_recv_result_t * result) ;

bool socket_start_recv(socket_recv_result_t * result) ;
bool socket_start_recvfrom(socket_recv_result_t * result , int flags , struct sockaddr * addr , int * addr_len) ;

socket_accept_result_t * socket_accept_result_new() ;
void socket_accept_result_free(socket_accept_result_t * result) ;
bool socket_accept_result_init(socket_accept_result_t * result) ;
bool socket_accept_result_final(socket_accept_result_t * result) ;

//从accept缓冲区中，提取结果
bool socket_accept(socket_accept_result_t * result , SOCKET& new_socket , struct sockaddr * local , struct sockaddr * remote) ;
bool socket_start_accept(socket_accept_result_t * result) ;

SOCKET socket_from_ovlp(iocp_ovlp_t * ovlp) ;


__END_DECLS

#endif /** __QKC_INTERNAL_FSOCKET_H */

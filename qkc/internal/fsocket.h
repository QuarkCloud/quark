
#ifndef __QKC_INTERNAL_FSOCKET_H
#define __QKC_INTERNAL_FSOCKET_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>
#include "ring_buffer.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
    为了实现epoll特性，需要对原来的socket做些改造，添加附加信息，不能直接使用。
*/
#define SOCKET_STAGE_VOID       0 
#define SOCKET_STAGE_BIND       1
#define SOCKET_STAGE_LISTEN     2
#define SOCKET_STAGE_ACCEPT     3
#define SOCKET_STAGE_CONNECT    11
#define SOCKET_STAGE_DESROTY    -1

typedef struct __st_socket_ovlp     socket_ovlp_t ;
typedef struct __st_send_result     send_result_t ;
typedef struct __st_recv_result     recv_result_t ;
typedef struct __st_accept_result   accept_result_t;
typedef struct __st_socket          socket_t;

typedef int (*socket_callback_t)(socket_t * s , int evt , int result) ;

#define kBeforeSocketClose      1 
#define kSocketConnect          2
#define kSocketSend             3
#define kSocketSendTo           4
#define kSocketRecv             5
#define kSocketRecvFrom         6

void socket_callback(socket_t *s , int evt , int result) ;

typedef struct __st_socket{
    SOCKET socket ;
    int8_t type ;                  //SOCK_STREAM、SOCK_DGRAM
    int8_t stage ;  
    int8_t noblock :1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

    HANDLE            locker ;    //创建销毁保护
    accept_result_t * acceptor ;
    send_result_t   * sender ;
    recv_result_t   * receiver ;

    void * addition ;               //附加的信息
    socket_callback_t callback ;
} socket_t;

typedef enum{
    OVLP_VOID   =   0 ,
    OVLP_ACCEPT =   1 ,
    OVLP_INPUT  =   2 ,
    OVLP_OUTPUT =   3 
} ovlp_type_t ;

struct __st_socket_ovlp
{
    OVERLAPPED          ovlp ;
    int                 status ;
    ovlp_type_t         type ;
    socket_t    *       owner ;
    volatile LONG       counter ;
} ;

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
struct __st_send_result{
    socket_ovlp_t   link ;

    WSABUF          data ;
    ring_buffer_t   ring_buffer ;
};

struct __st_recv_result{
    socket_ovlp_t   link ;
    WSABUF data ;
};

#define ACCEPT_ADDRESS_SIZE 256
struct __st_accept_result{
    socket_ovlp_t   link ;

    SOCKET insocket ;           //接收到客户端连接请求的套接字   
    char address[ACCEPT_ADDRESS_SIZE] ;
    size_t bytes ;
};

void update_contex_acceptex(SOCKET& new_socket , SOCKET& listen_socket) ;

bool sockopt_get_keepalive(SOCKET& s , bool &optval) ;
bool sockopt_set_keepalive(SOCKET& s , bool optval) ;

bool sockopt_get_recv_buffer_size(SOCKET& s , int& optval) ;
bool sockopt_set_recv_buffer_size(SOCKET& s , int optval) ;

bool sockopt_get_send_buffer_size(SOCKET& s , int& optval) ;
bool sockopt_set_send_buffer_size(SOCKET& s , int optval) ;

bool sockopt_get_recv_timeout(SOCKET& s , DWORD& optval) ;
bool sockopt_set_recv_timeout(SOCKET& s , DWORD optval) ;

bool sockopt_get_send_timeout(SOCKET& s , DWORD& optval) ;
bool sockopt_set_send_timeout(SOCKET& s , DWORD optval) ;

bool sockopt_get_nodelay(SOCKET&s , bool& optval) ;
bool sockopt_set_nodelay(SOCKET&s , bool optval) ;

bool socket_get_readable_size(SOCKET& s , DWORD& size) ;
bool socket_set_nonblock(SOCKET& s , bool enable) ;

socket_t * socket_new() ;
bool socket_init(socket_t * s) ;
void socket_free(socket_t * s) ;
bool socket_final(socket_t * s) ;

send_result_t * send_result_new() ;
void send_result_free(send_result_t * result) ;
bool send_result_init(send_result_t * result) ;
bool send_result_final(send_result_t * result) ;

bool socket_send(send_result_t * result , int flags) ;
bool socket_sendto(send_result_t * result , int flags , const struct sockaddr * addr , int addr_len) ;

recv_result_t * recv_result_new() ;
void recv_result_free(recv_result_t * result) ;
bool recv_result_init(recv_result_t * result) ;
bool recv_result_final(recv_result_t * result) ;

bool socket_start_recv(recv_result_t * result) ;
bool socket_start_recvfrom(recv_result_t * result , int flags , struct sockaddr * addr , int * addr_len) ;

bool socket_ovlp_lock(socket_ovlp_t * ovlp) ;
bool socket_ovlp_unlock(socket_ovlp_t * ovlp) ;
int socket_ovlp_counter(socket_ovlp_t * ovlp) ;

accept_result_t * accept_result_new() ;
void accept_result_free(accept_result_t * result) ;
bool accept_result_init(accept_result_t * result) ;
bool accept_result_final(accept_result_t * result) ;

//从accept缓冲区中，提取结果
bool socket_accept(accept_result_t * result , SOCKET& new_socket , struct sockaddr * local , struct sockaddr * remote) ;
bool socket_start_accept(accept_result_t * result) ;


#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_FSOCKET_H */

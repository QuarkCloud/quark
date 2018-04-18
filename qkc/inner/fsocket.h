
#ifndef __QKC_INNER_FSOCKET_H
#define __QKC_INNER_FSOCKET_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>

#ifdef	__cplusplus
extern "C" {
#endif

/**
    为了实现epoll特性，需要对原来的socket做些改造，添加附加信息，不能直接使用。
*/
#define SOCKET_STAGE_VOID       0 
#define SOCKET_STAGE_BIND       1
#define SOCKET_STAGE_LISTEN     2
#define SOCKET_STAGE_CONNECT    3
#define SOCKET_STAGE_DESROTY    -1

typedef struct __st_socket_ovlp     socket_ovlp_t ;
typedef struct __st_send_result     send_result_t ;
typedef struct __st_recv_result     recv_result_t ;
typedef struct __st_accept_result   accept_result_t;

typedef struct __st_socket{
    SOCKET socket ;
    int8_t type ;                  //SOCK_STREAM、SOCK_DGRAM
    int8_t stage ;  
    int8_t noblock :1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

    accept_result_t * acceptor ;
    send_result_t   * sender ;
    recv_result_t   * receiver ;
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
} ;

struct __st_send_result{
    socket_ovlp_t   link ;

    WSABUF data ;
    int bufsize ;               //套接字本身缓冲区的大小
    int sending_bytes ;         //本次正在发送中的字节数
    size_t to_bytes ;           //请求发送的总字节数
    size_t completed_bytes ;    //完成发送的总字节数
};

struct __st_recv_result{
    socket_ovlp_t   link ;

    WSABUF data ;
    size_t from_bytes ;
    size_t complete_bytes ;
};

#define ACCEPT_ADDRESS_SIZE 256
struct __st_accept_result{
    socket_ovlp_t   link ;

    SOCKET insocket ;           //接收到客户端连接请求的套接字   
    char address[ACCEPT_ADDRESS_SIZE] ;
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

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INNER_FSOCKET_H */

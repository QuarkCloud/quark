
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
#define SOCKET_STAGE_NOTUSED    -1

typedef struct __st_socket{
    SOCKET socket ;
    int type ;                  //SOCK_STREAM、SOCK_DGRAM
    int stage ;  
    int8_t noblocking ;         //默认阻塞，是一个很重要的标识!!!
    int8_t nodelay ;            //默认延迟
    int8_t keepalive ;          //保持活跃，默认false
} socket_t;

typedef enum{
    OVLP_VOID   =   0 ,
    OVLP_ACCEPT =   1 ,
    OVLP_INPUT  =   2 ,
    OVLP_OUTPUT =   3 
} ovlp_type_t ;

typedef struct __st_socket_ovlp
{
    OVERLAPPED ovlp ;
    int                 status ;
    ovlp_type_t         type ;
} socket_ovlp_t;

typedef struct __st_send_result{
    socket_ovlp_t   header ;

    WSABUF data ;
    int bufsize ;               //套接字本身缓冲区的大小
    int sending_bytes ;         //本次正在发送中的字节数
    size_t to_bytes ;           //正在发送的总字节数
    size_t completed_bytes ;    //完成发送的总字节数
} send_result_t ;

typedef struct __st_recv_result{
    socket_ovlp_t   header ;

    WSABUF data ;
    size_t from_bytes ;
    size_t complete_bytes ;
} recv_result_t ;

#define ACCEPT_ADDRESS_SIZE 256
typedef struct __st_accept_result{
    socket_ovlp_t   header ;
    SOCKET insocket ;           //接收到客户端连接请求的套接字   
    char address[ACCEPT_ADDRESS_SIZE] ;
} accept_result_t;

LPFN_ACCEPTEX look_up_acceptex(SOCKET& s) ;

void update_contex_acceptex(SOCKET& new_socket , SOCKET& listen_socket) ;


#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INNER_FSOCKET_H */

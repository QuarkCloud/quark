
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
    Ϊ��ʵ��epoll���ԣ���Ҫ��ԭ����socket��Щ���죬��Ӹ�����Ϣ������ֱ��ʹ�á�
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
    int8_t type ;                  //SOCK_STREAM��SOCK_DGRAM
    int8_t stage ;  
    int8_t noblock :1;            //Ĭ����������һ������Ҫ�ı�ʶ!!!������ò��û�б�Ҫ��

    HANDLE            locker ;    //�������ٱ���
	socket_accept_result_t * acceptor ;
	socket_send_result_t   * sender ;
	socket_recv_result_t   * receiver ;

    void * addition ;               //���ӵ���Ϣ
} socket_t;

/**
    2018-04-19
    ��iocp��preactorģʽת����epoll��actor�ѶȱȽϴ���Ҫ������sendʱ����֪�����Է��������ֽ�����
    send�Ŀɷ��ֽ���ͬʱ����ס��ͬʱ�����������շ�˫���Ļ�������С���ƣ������ǵ������ɱ��ط��ͻ�����������
    immediately���ص��ֽ���ֻ�����ο��ã�����������ɵ��ֽ������������͵��ֽ������������Ϸ���ʱ��
    �������ͻᱻ�������������������Ч�ʡ���ˣ���Ҫ���һ������������ȷ���ֽ����ǿɿصġ�����Ϊ������
    ���������½��������������˵��Ӧ�÷������һ�㡣
    ���ͨ��TCP��ͬһ������ͬʱ�������ӷ��ͣ���ô������ʧ����Ϊ�������½��������ء�
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

    SOCKET insocket ;           //���յ��ͻ�������������׽���   
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

//��accept�������У���ȡ���
bool socket_accept(socket_accept_result_t * result , SOCKET& new_socket , struct sockaddr * local , struct sockaddr * remote) ;
bool socket_start_accept(socket_accept_result_t * result) ;

SOCKET socket_from_ovlp(iocp_ovlp_t * ovlp) ;


__END_DECLS

#endif /** __QKC_INTERNAL_FSOCKET_H */

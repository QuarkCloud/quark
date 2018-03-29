
#ifndef __QKC_INNER_FSOCKET_H
#define __QKC_INNER_FSOCKET_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct __st_socket_channel
{
    OVERLAPPED ovld ;
    WSABUF  data ;
    size_t  bytes ;
    int     status ;
    char *  buffer ;
    size_t  bufsize ;
} socket_channel_t;

typedef enum{
    CHANNEL_INPUT  =0 ,
    CHANNEL_OUTPUT = 1 
} channel_type_t ;

bool socket_channel_init(socket_channel_t * chn , size_t size) ;
bool socket_channel_final(socket_channel_t * chn) ;
bool socket_channel_complete(socket_channel_t * chn , size_t bytes) ;


typedef struct __socket_data{
    socket_channel_t in ;
    socket_channel_t out ;
    bool noblock ;               //是否是非阻塞模式，默认阻塞模式
} socket_data_t ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INNER_FSOCKET_H */

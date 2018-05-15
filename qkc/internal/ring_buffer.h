
#ifndef __QKC_INTERNAL_RING_BUFFER_H
#define __QKC_INTERNAL_RING_BUFFER_H 1

#include <stdlib.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct __st_ring_buffer{
    char * buffer ;
    size_t bufsize ;
    size_t head ;
    size_t tail ;
} ring_buffer_t;

bool ring_buffer_init(ring_buffer_t * ring , size_t size) ;

bool ring_buffer_final(ring_buffer_t * ring) ;

size_t ring_buffer_write_stream(ring_buffer_t * ring , const void * buf , size_t size) ;

bool ring_buffer_refer_stream(ring_buffer_t * ring , char *&buf , size_t& size) ;

bool ring_buffer_move_size(ring_buffer_t * ring , size_t size) ;

/**
    为sendto服务的，每个消息需要分片发送，而且分片是独立，不可分割。
    因此每个报文头部加上两个字节的魔数0x5555，再加上两个字节的长度
*/
size_t ring_buffer_write_message(ring_buffer_t * ring , const void * buf , size_t size) ;

bool ring_buffer_refer_message(ring_buffer_t * ring , char *&buf , size_t& size) ;


#ifdef	__cplusplus
}
#endif

#endif /** __QKC_INTERNAL_RING_BUFFER_H */

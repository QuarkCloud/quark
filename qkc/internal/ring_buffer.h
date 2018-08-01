
#ifndef __QKC_INTERNAL_RING_BUFFER_H
#define __QKC_INTERNAL_RING_BUFFER_H 1

#include <stdlib.h>
#include <string.h>

__BEGIN_DECLS

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
    Ϊsendto����ģ�ÿ����Ϣ��Ҫ��Ƭ���ͣ����ҷ�Ƭ�Ƕ��������ɷָ
    ���ÿ������ͷ�����������ֽڵ�ħ��0x5555���ټ��������ֽڵĳ���
*/
size_t ring_buffer_write_message(ring_buffer_t * ring , const void * buf , size_t size) ;

bool ring_buffer_refer_message(ring_buffer_t * ring , char *&buf , size_t& size) ;


__END_DECLS

#endif /** __QKC_INTERNAL_RING_BUFFER_H */

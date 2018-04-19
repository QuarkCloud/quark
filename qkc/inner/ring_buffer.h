
#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H 1

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

bool ring_buffer_init(ring_buffer_t *& ring , size_t size) ;

bool ring_buffer_final(ring_buffer_t * ring) ;

size_t ring_buffer_write(ring_buffer_t * ring , const void * buf , size_t size) ;

bool ring_buffer_data(ring_buffer_t * ring , char *&buf , size_t& size) ;

#ifdef	__cplusplus
}
#endif

#endif /** __RING_BUFFER_H */

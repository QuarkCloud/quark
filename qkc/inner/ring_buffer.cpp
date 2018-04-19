
#include "ring_buffer.cpp"
#include <errno.h>

bool ring_buffer_init(ring_buffer_t *& ring , size_t size)
{
    ring_buffer_t * rbuf = (ring_buffer_t *)::malloc(sizeof(ring_buffer_t)) ;
    if(rbuf == NULL)
    {
        errno = ENOMEN ;
        return false ;    
    }
    ::memset(rbuf , 0 , sizeof(ring_buffer_t)) ;

    char * buffer = (char *)::malloc(size) ;
    if(buffer == NULL)
    {
        ::free(rbuf) ;
        errno = ENOMEM ;
        return false ;
    }

    rbuf->buffer = buffer ;
    rbuf->bufsize = size ;
    ring = rbuf ;
    return true ;
}

bool ring_buffer_final(ring_buffer_t * ring)
{
    if(ring == NULL)
        return false ;

    char * buffer = ring->buffer ;
    ring->buffer = NULL ;
    if(buffer != NULL)
        ::free(buffer) ;
    ::free(ring) ;
    return true ;
}

size_t ring_buffer_write(ring_buffer_t * ring , const void * buf , size_t size)
{
    size_t head = ring->head , tail = ring->tail , bufsize = ring->bufsize;
    size_t left_size = 0 ;
    if(head >= tail)
        left_size = bufsize + tail - head ;
    else
        left_size = tail - head ;
    --left_size ;

    size_t copy_size = (left_size >= size)? size : left_size ;
    if(head > tail)
    {
        //尾部回绕，直接从tail拷贝head
        ::memcpy(ring->buffer + tail , buf , copy_size) ;
        ring->tail += copy_size ;
    }
    else
    {
        //分段拷贝
        size_t cont_size = SNDBUFSIZE - tail ;
        if(cont_size > copy_size)
            cont_size = copy_size ;
        ::memcpy(ring->buffer + tail , buf , cont_size) ;
        size_t broken_size = copy_size - cont_size ;
        if(broken_size > 0)
        {
            ::memcpy(ring->buffer , buf + cont_size , broken_size) ;
            ring->tail = broken_size ;
        }
        else
        {
            ring->tail += cont_size ;
        }
    }

    return copy_size ;
}

bool ring_buffer_data(ring_buffer_t * ring , char *&buf , size_t& size)
{
    if(ring->head <= ring->tail)
    {
        size = ring->tail - ring->head ;
        buf = ring->buffer + ring->head ;
        return true ;
    }

    size = ring->bufsize - ring->head ;
    buf = ring->buffer + ring->head ;
    return true ;
}


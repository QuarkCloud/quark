
#include "fsocket.h"
#include <stdlib.h>
#include <string.h>

bool socket_channel_init(socket_channel_t * chn , size_t size)
{
    ::memset(chn , 0 , sizeof(socket_channel_t)) ;
    char * buffer = (char *)::malloc(size) ;
    if(buffer == NULL)
        return false ;

    chn->buffer = buffer ;
    chn->bufsize = size ;

    //之所以没有初始化data，是因为recv/send是不同的
    return true ;
}

bool socket_channel_final(socket_channel_t * chn)
{
    if(chn == NULL)
        return false ;

    char * buffer = chn->buffer;
    ::memset(chn , 0 , sizeof(socket_channel_t)) ;
    if(buffer != NULL)
        ::free(buffer) ;

    return false ;
}

bool socket_channel_complete(socket_channel_t * chn , size_t bytes)
{
    if(chn == NULL)
        return false ;
    chn->bytes = bytes ;
    return true ;
}

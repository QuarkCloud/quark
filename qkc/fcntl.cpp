
#include <fcntl.h>
#include <wintf/wcrt.h>

int fcntl (int fd, int cmd , ...)
{
    return 0 ;
}


int creat(const char * file , mode_t mode) 
{
    return ::_creat(file , mode) ;
}

int open(const char * file , int flag,mode_t mode)
{
    return ::_open(file , flag , mode) ;
}



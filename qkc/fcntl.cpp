
#include <fcntl.h>


#ifdef __cplusplus
extern "C"{
#endif

QUARK_LINKAGE int _creat(const char * file , mode_t mode) ;
QUARK_LINKAGE int _open(const char * file , int flag , mode_t mode) ;

#ifdef __cplusplus
}
#endif

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



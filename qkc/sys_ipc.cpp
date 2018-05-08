#include <stdint.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>

key_t ftok (const char *pathname , int proj_id)
{
    //int stat (const char * file, struct stat * buf) ;
    struct stat sbuf ;
    if(::stat(pathname , &sbuf) != 0)
        return -1 ;

    uint32_t dev = sbuf.st_dev ;
    uint32_t ino = sbuf.st_ino ;
    uint32_t id = (uint32_t)proj_id ;

    key_t key = (id << 24) | ((dev & 0XFF) << 16) | (ino & 0XFFFF) ;
    return key ;
}


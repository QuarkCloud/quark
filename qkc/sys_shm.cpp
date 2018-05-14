
#include <sys/shm.h>
#include <wintf/wipc.h>
#include <wintf/wobj.h>

int shmctl (int shmid, int cmd, struct shmid_ds * buf)
{
    return 0 ;
}

int shmget (key_t key, size_t size, int shmflg)
{
    return 0 ;
}

void *shmat (int shmid, const void * shmaddr, int shmflg)
{
    return NULL ;
}

int shmdt (const void * shmaddr)
{
    return 0 ;
}


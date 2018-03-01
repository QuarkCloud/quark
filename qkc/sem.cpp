
#include <sys/sem.h>

int semctl (int semid, int semnum, int cmd, ...)
{
    return 0 ;
}

int semget (key_t key, int nsems, int semflg)
{
    return 0 ;
}

int semop (int semid, struct sembuf * sops, size_t nsops)
{
    return 0 ;
}

int semtimedop (int semid, struct sembuf *sops , size_t nsops , const struct timespec * timeout)
{
    return 0 ;
}

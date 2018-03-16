
#include <sys/sem.h>
#include <stdarg.h>

int semctl(int semid, int semnum, int cmd, ...)
{
    union semun arg = {0};
    if(cmd == IPC_STAT || cmd == IPC_SET || cmd == IPC_INFO || cmd == SEM_INFO || 
       cmd == GETALL || cmd == SETALL || cmd == SETVAL)
    {
        va_list ap;
        va_start (ap, cmd);
        arg = va_arg (ap, union semun);
        va_end (ap);
    }

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


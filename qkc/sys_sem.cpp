
#include <sys/sem.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <wintf/wipc.h>
#include <wintf/wobj.h>

int semctl(int semid, int semnum, int cmd, ...)
{
    union semun arg = {0};
    if(cmd == IPC_STAT || cmd == IPC_SET || cmd == IPC_INFO || cmd == SEM_INFO || 
       cmd == GETALL || cmd == SETALL || cmd == SETVAL || cmd == GETVAL)
    {
        va_list ap;
        va_start (ap, cmd);
        arg = va_arg (ap, union semun);
        va_end (ap);
    }

    wobj_t * obj = wobj_find_by_handle(WOBJ_SEMA , (HANDLE)semid) ;
    if(obj == NULL || obj->addition == NULL)
    {
        errno = ENOENT ;
        return -1 ;
    }

    win_sem_t * wsem = (win_sem_t *)obj->addition ;
    if(wsem->handle == NULL)
    {
        return -1 ;
    }

    ipc_sem_t * sem =  wsem->ipc ;

    sem->ctime = (int)::time(NULL) ;

    if(cmd == IPC_STAT || cmd == IPC_SET || cmd == IPC_INFO)
    {
        errno = ENOSYS ;
        return -1 ;
    }

    if(cmd == GETVAL)
    {
        arg.val = (int)sem->value ;
    }



    return 0 ;
}

int semget (key_t key, int nsems, int semflg)
{
    int semid = ipc_alloc_id(key , IPC_TYPE_SEM , semflg) ;
    if(semid < 0)
        return -1 ;

    win_sem_t * sem = ipc_sem_create(semid) ;
    if(sem == NULL)
        return -1 ;

    int oid = ::wobj_set(WOBJ_SEMA , (HANDLE)semid , sem) ;
    if(oid <= 0)
    {
        ipc_sem_destroy(sem) ;
        return -1 ;
    }

    sem->oid = oid ;
    return semid ;
}

int semop (int semid, struct sembuf * sops, size_t nsops)
{
    return 0 ;
}


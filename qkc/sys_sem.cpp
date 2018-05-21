
#include <sys/sem.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <wintf/wipc.h>
#include <wintf/wobj.h>

int semctl_getval(ipc_sem_t * sem)
{
    return sem->value ;
}

int semctl_setval(win_sem_t * wsem , int val)
{
    if(wsem == NULL || wsem->ipc == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }
    ipc_item_t * sem = wsem->ipc ;
    if(val > sem->value)
    {
        int delta = val - sem->value ;
        if(::ReleaseSemaphore(wsem->handle , delta , NULL) == TRUE)
        {
            ::InterlockedExchangeAdd(&sem->value , delta) ;
            sem->value = val ;
        }
        else
            return -1 ;
    }
    else if(val < sem->value)
    {
        while(true)
        {
            if(val >= sem->value)
                break ;

            DWORD result = 0 ;
            if((result = ::WaitForSingleObject(wsem->handle , 0)) == WAIT_OBJECT_0)
                ::InterlockedDecrement(&sem->value) ;
            else if(result == WAIT_TIMEOUT)
                ::InterlockedExchange(&sem->value , 0) ;
            else
                return -1 ;
        }
    }

    return 0 ; 
}

int semctl_ipcstat(win_sem_t * wsem , struct semid_ds * ds)
{
    if(wsem == NULL || wsem->ipc == NULL || ds == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }
    ipc_sem_t * sem = wsem->ipc ;
    ds->sem_perm.__key = wsem->key ;
    ds->sem_nsems = 1 ;
    ds->sem_otime = sem->otime ;
    ds->sem_ctime = sem->ctime ;

    return 0 ;
}

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

    if(cmd == GETVAL)
        return semctl_getval(sem) ;

    if(cmd == SETVAL)
        return ::semctl_setval(wsem , arg.val) ;

    if(cmd == IPC_STAT)
        return ::semctl_ipcstat(wsem , arg.ds) ;

    errno = ENOSYS ;
    return -1 ;
}

int semget (key_t key, int nsems, int semflg)
{
    int semid = ipc_alloc_id(key , IPC_TYPE_SEM , semflg) ;
    if(semid < 0)
        return -1 ;

    win_sem_t * sem = ipc_sem_create(semid) ;
    if(sem == NULL)
        return -1 ;

    /**
        2018-05-18
        在哪个位置创建本地信号，是我一直纠结的事情。主要是windows信号量的值不能设置，在创建时就指定了。
        后面想，只要在记录下前值，在SETVAL时，可以通过ReleaseSemaphore调整，还是可以达到效果。
    */
    if(ipc_sem_init(sem) == false)
    {
        ipc_sem_destroy(sem) ;
        return -1 ;
    }

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



#include <sys/shm.h>
#include <wintf/wipc.h>
#include <wintf/wobj.h>
#include <errno.h>

int shmctl (int shmid, int cmd, struct shmid_ds * buf)
{
    if(cmd == IPC_STAT || cmd == IPC_SET)
    {
        errno = ENOSYS ;
        return -1 ;
    }
    if(cmd != IPC_RMID)
    {
        errno = EINVAL ;
        return -1 ;
    }

    return 0 ;
}

int shmget (key_t key, size_t size, int shmflg)
{
    int shmid = ::ipc_alloc_id(key , IPC_TYPE_SHM , shmflg) ;
    if(shmid < 0)
        return -1 ;

    win_shm_t * shm = ipc_shm_create(shmid , size) ;
    if(shm == NULL)
        return -1 ;

    int oid = wobj_set(WOBJ_SHM , (HANDLE)shmid , shm) ;
    shm->oid = oid ;
    return shmid ;
}

void *shmat (int shmid, const void * shmaddr, int shmflg)
{
    wobj_t * obj = ::wobj_find_by_handle(WOBJ_SHM , (HANDLE)shmid) ;
    if(obj == NULL || obj->addition == NULL)
        return NULL ;

    win_shm_t * shm = (win_shm_t *)obj->addition ;
    if(ipc_shm_init(shm) == false || shm->map_addr == NULL)
        return NULL ;

    void * addr = shm->map_addr ;
    if(ipc_shm_addr_add(addr , shm) == false)
        return NULL ;

    return addr ;
}

int shmdt (const void * shmaddr)
{
    win_shm_t * shm = ipc_shm_addr_find(shmaddr) ;
    if(shm == NULL)
        return -1 ;

    int oid = shm->oid ;
    wobj_del(oid) ;
    ipc_shm_destroy(shm) ;
    ipc_shm_addr_del(shmaddr) ;

    return 0 ;
}


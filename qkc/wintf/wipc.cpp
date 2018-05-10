
#include "wintf/wipc.h"

static const char * __wipc_mtx_name__   = "Global\\qkc.mtx" ;
static const char * __wipc_shm_name__   = "Global\\qkc.shm" ;
static const char * __ipc_super_magic__ = "qkc" ;

static HANDLE __wipc_mtx__ = NULL ;


const char * ipc_mtx_name()
{
    return __wipc_mtx_name__ ;
}

const char * ipc_shm_name()
{
    return __wipc_shm_name__ ;
}

bool ipc_super_init(ipc_super_t * super)
{
    if(ipc_super_validate_magic(super) == true)
        return true ;

    ::memset(super , 0 , IPC_SUPER_SIZE) ;
    ipc_super_assign_magic(super) ;
    return true ;
}

bool ipc_super_final(ipc_super_t * super)
{
    //释放关联的资源
    return false ;
}

bool ipc_super_validate_magic(ipc_super_t * super)
{
    return (::strcmp(super->magic , __ipc_super_magic__) == 0) ;
}

void ipc_super_assign_magic(ipc_super_t * super)
{
    ::memcpy(super->magic , __ipc_super_magic__ , 4) ;
}

bool ipc_global_init(ipc_global_t * global)
{
    return false ;
}

bool ipc_global_final(ipc_global_t * global)
{
    return false ;
}

int  ipc_alloc_id(key_t key , int type)
{
    return 0 ;
}

void  ipc_free_id(int id)
{
    //
}

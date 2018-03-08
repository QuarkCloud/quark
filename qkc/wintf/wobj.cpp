
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>

INIT_ONCE __wobj_inited__ = INIT_ONCE_STATIC_INIT ;
SRWLOCK __wobj_rwlock__ =  SRWLOCK_INIT ;

/**
    暂时先开4096个，后续再扩。
*/
static const int wObjMaxSize =  4096 ;
static int wObjLastIndex = 0 ;
static wobj_t __wobjs__[wObjMaxSize] ;


BOOL CALLBACK InitWObjsFunction(PINIT_ONCE InitOnce , PVOID Parameter , PVOID *lpContext)
{
    ::memset(__wobjs__ , 0 , sizeof(__wobjs__)) ;
    return TRUE ;
}

BOOL ConfirmWObjInited()
{  
    return InitOnceExecuteOnce(&__wobj_inited__,InitWObjsFunction,NULL, NULL);
}

#define WID2XID(wid) (wid - WOBJ_ID_BASE)

wobj_t * wobj_get(int wid)
{
    int xid = WID2XID(wid) ;
    if(xid < 0 || xid >= wObjMaxSize)
        return NULL ;

    wobj_t * ko = NULL ;
    if(ConfirmWObjInited() == TRUE)
    {           
        ::AcquireSRWLockShared(&__wobj_rwlock__) ;
        ko = __wobjs__ + xid ;
        ::ReleaseSRWLockShared(&__wobj_rwlock__) ;
    }

    return ko ;
}

int wobj_set(wobj_type type , HANDLE handle , void * addition)
{
    if(ConfirmWObjInited() == false)
        return INVALID_WOBJ_ID ;

    int wid = INVALID_WOBJ_ID ;
    int xid = wObjLastIndex + 1;
    ::AcquireSRWLockExclusive(&__wobj_rwlock__) ;
    while(xid != wObjLastIndex)
    {
        if(xid >= wObjMaxSize)
            xid = 0 ;

        wobj_t * ko = __wobjs__ + xid ;
        if((ko->handle != NULL && ko->handle != INVALID_HANDLE_VALUE) || ko->type != WOBJ_VOID || ko->wid != 0)
        {
            ++xid ;
            continue ;
        }

        wid = xid + WOBJ_ID_BASE ;

        ko->handle = handle ;
        ko->type = type ;
        ko->wid = wid ;
        ko->addition = addition ;

        wObjLastIndex = xid ;

        break ;    
    }

    ::ReleaseSRWLockExclusive(&__wobj_rwlock__) ;
    return wid ;
}

bool wobj_del(int wid)
{
    if(ConfirmWObjInited() == FALSE)
        return false ;

    int xid = WID2XID(wid) ;
    if(xid < 0 || xid >= wObjMaxSize)
        return false ;

    bool result = false ;
    ::AcquireSRWLockExclusive(&__wobj_rwlock__) ;
    wobj_t * ko = __wobjs__ + xid ;
    if(ko->wid == wid)
    {
        HANDLE handle = ko->handle ;
        if(handle != NULL && handle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(handle) ;
            result = true ;
        }

        ::memset(ko , 0 , sizeof(wobj_t)) ;
    }

    ::ReleaseSRWLockExclusive(&__wobj_rwlock__) ;

    return result ;
}

static HANDLE __global_process_heap__ = NULL ;

HANDLE default_heap_get()
{
	if(__global_process_heap__ == NULL)
		__global_process_heap__ = ::GetProcessHeap() ;

	return __global_process_heap__ ;
}


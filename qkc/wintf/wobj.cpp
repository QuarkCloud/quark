
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>

INIT_ONCE __wobj_inited__ = INIT_ONCE_STATIC_INIT ;
SRWLOCK __wobj_rwlock__ =  SRWLOCK_INIT ;

/**
    暂时先开4096个，后续再扩。
*/
static const int wObjMaxSize =  16384 ;
static int wObjLastIndex = 0 ;
static int wObjCounter = 0;
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

wobj_t *  wobj_find_by_handle(wobj_type type , HANDLE handle)
{
    if(ConfirmWObjInited() == FALSE)
        return NULL ;

    wobj_t * ko = NULL ;
    ::AcquireSRWLockShared(&__wobj_rwlock__) ;

    for(int idx = 0 ; idx < wObjMaxSize ; ++idx)
    {
        wobj_t * obj = __wobjs__ + idx ;
        if(obj->type == type && obj->handle == handle)
        {
            ko = obj ;
            break ;
        }
    }

    ::ReleaseSRWLockShared(&__wobj_rwlock__) ;

    return ko ;
}

int wobj_set(wobj_type type , HANDLE handle , void * addition)
{
    if(ConfirmWObjInited() == false)
        return INVALID_WOBJ_ID ;

    int wid = INVALID_WOBJ_ID ;
    int xid = wObjLastIndex + 1;
    ::AcquireSRWLockExclusive(&__wobj_rwlock__) ;
    while(xid != wObjLastIndex && wObjCounter < wObjMaxSize)
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
		++wObjCounter;

        break ;    
    }
    ::ReleaseSRWLockExclusive(&__wobj_rwlock__) ;

	if (wid == INVALID_WOBJ_ID)
	{
		::printf("failed to alloc wid for type = %d handle = %p \n" , type , handle);
	}
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
        /**
        HANDLE handle = ko->handle ;
        if(handle != NULL && handle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(handle) ;
            result = true ;
        }
        */
        result = true ;
        ::memset(ko , 0 , sizeof(wobj_t)) ;
		--wObjCounter;
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

static SRWLOCK __global_rwlock__ =  SRWLOCK_INIT ;

void global_wrlock()
{
    ::AcquireSRWLockExclusive(&__global_rwlock__) ;
}

void global_wrunlock()
{
    ::ReleaseSRWLockExclusive(&__global_rwlock__) ;
}

void global_rdlock()
{
    ::AcquireSRWLockShared(&__global_rwlock__) ;
}

void global_rdunlock()
{
    ::ReleaseSRWLockShared(&__global_rwlock__) ;
}


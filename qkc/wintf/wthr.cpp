
#include <wintf/wthr.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>
#include "../internal/rlist.h"

SRWLOCK __default_tlsidx_rwlock__ =  SRWLOCK_INIT ;
static bool __default_tlsidx_inited__ = false ;
static DWORD __default_tlsidx__ = TLS_OUT_OF_INDEXES ;

typedef struct __st_tls_node{
    rlist_t link ;
    void * value ;
}tls_node_t ;

typedef struct __st_wthr_tls{
    bool            inited ;
    DWORD           key ;
    tls_cleanup_t   cleanup ;
    rlist_t         nodes ;
}wthr_tls_t ;

static const int kTLSMaxSize = 1024 ;
static wthr_tls_t   __tls_nodes__[kTLSMaxSize] ;
static bool __tls_inited__ = false ;


DWORD default_tlsidx_alloc()
{
    if(__default_tlsidx_inited__ == false)
    {
        ::AcquireSRWLockExclusive(&__default_tlsidx_rwlock__) ;

        if(__default_tlsidx_inited__ == false)
        {
            __default_tlsidx__ = ::TlsAlloc() ;
            __default_tlsidx_inited__ = true ;
        }
        ::ReleaseSRWLockExclusive(&__default_tlsidx_rwlock__) ;        
    }

    return __default_tlsidx__ ;
}

void default_tlsidx_free()
{
    if(__default_tlsidx__ != TLS_OUT_OF_INDEXES)
    {
        ::AcquireSRWLockExclusive(&__default_tlsidx_rwlock__) ;

        if(__default_tlsidx__ != TLS_OUT_OF_INDEXES)
        {
            ::TlsFree(__default_tlsidx__) ;
            __default_tlsidx__ = TLS_OUT_OF_INDEXES ;
        }
        ::ReleaseSRWLockExclusive(&__default_tlsidx_rwlock__) ;            
    }
}

wthr_info_t * wthr_info_get()
{
    DWORD tlsidx = default_tlsidx_alloc() ;
    wthr_info_t * info = (wthr_info_t *)::TlsGetValue(tlsidx) ;
    if(info == NULL)
    {
        info = (wthr_info_t *)::HeapAlloc(::GetProcessHeap() , 0 , sizeof(wthr_info_t)) ;
        if(info != NULL)
        {
            wthr_info_init(info) ;
            ::TlsSetValue(tlsidx , info) ;
        }
    }

    return info ;
}

void wthr_info_init(wthr_info_t * info)
{
    ::memset(info , 0 , sizeof(wthr_info_t)) ;
}

void whtr_info_free(wthr_info_t * info)
{
    if(__default_tlsidx__ == TLS_OUT_OF_INDEXES)
        return ;

    ::TlsSetValue(__default_tlsidx__ , NULL) ;
    if(info != NULL)
    {
        ::HeapFree(::GetProcessHeap() , 0 , info) ;
    }    
}


bool wthr_process_attach()
{
    DWORD tlsidx = default_tlsidx_alloc() ;
    return (tlsidx != TLS_OUT_OF_INDEXES) ;
}

bool wthr_process_detach() 
{
    default_tlsidx_free() ;
    return true ;
}

bool wthr_thread_attach() 
{
    return (::TlsGetValue(default_tlsidx_alloc()) != NULL);
}

bool wthr_thread_detach() 
{
    void * value = ::TlsGetValue(default_tlsidx_alloc()) ;
    if(value != NULL)
    {
        ::HeapFree(default_heap_get() , 0 , value) ;
        ::TlsSetValue(default_tlsidx_alloc() , NULL) ;
    }
    return true ;
}

int whtr_tls_alloc(tls_cleanup_t cleanup)
{

}

void whtr_tls_delete(int tls)
{

}

void whtr_tls_cleanup()
{

}

int whtr_tls_first()
{

}

int whtr_tls_next()
{

}

int whtr_tls_set_val(int pid , int key , void * val)
{

}

void * whtr_tls_get_val(int pid , int key)
{

}


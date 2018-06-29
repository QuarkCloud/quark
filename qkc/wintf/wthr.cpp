
#include <wintf/wthr.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <string.h>

typedef struct __st_wthr_tls{
    bool            inited ;
    DWORD           key ;
    tls_cleanup_t   cleanup ;
}wthr_tls_t ;

SRWLOCK __tlsidx_rwlock__ =  SRWLOCK_INIT ;
static const int kTLSMaxSize = 1024 ;
static int __tls_last_index__  = 1 ;
static wthr_tls_t   __tls_nodes__[kTLSMaxSize] ;
static bool __tls_inited__ = false ;

void tls_nodes_init()
{
    if(__tls_inited__ == true)
        return ;

    ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;
    ::memset(__tls_nodes__ , 0 , sizeof(__tls_nodes__)) ;
    for(int counter = 0 ; counter < kTLSMaxSize ; ++counter)
    {
        __tls_nodes__[counter].key = TLS_OUT_OF_INDEXES ;
    }
    __tls_inited__ = true ;
    ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;     
}


int default_tlsidx_alloc()
{
    wthr_tls_t& tls = __tls_nodes__[0] ;

    ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;
    if(tls.inited == false)
    {
        tls.key = TlsAlloc() ;
        if(tls.key != TLS_OUT_OF_INDEXES)
            tls.inited = true ;
    }
    ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;

    if(tls.inited == true)
        return 0 ;
    else
        return -1 ;
}

void default_tlsidx_free()
{
    wthr_tls_t& tls = __tls_nodes__[0] ;
    if(tls.inited == true)
    {
        ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;
        if(tls.inited == true)
        {
            tls.inited = false ;
            DWORD key = tls.key ;
            tls.key = TLS_OUT_OF_INDEXES ;
            if(key != TLS_OUT_OF_INDEXES)
                ::TlsFree(key) ;
        }
        ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;            
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

void wthr_info_free(wthr_info_t * info)
{
    DWORD key = __tls_nodes__[0].key ;
    if(key == TLS_OUT_OF_INDEXES)
        return ;

    ::TlsSetValue(key , NULL) ;
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

int wthr_tls_alloc(tls_cleanup_t cleanup)
{
    int tlsidx = -1 ;

    ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;

    int last_index = __tls_last_index__;
    for(int counter = 0 ; counter < kTLSMaxSize - 1; ++counter)
    {
        if(last_index == 0 || last_index >= kTLSMaxSize)
            last_index = 1 ;

        wthr_tls_t& tls = __tls_nodes__[last_index] ;
        if(tls.inited == true)
        {
            ++last_index ;
            continue ;
        }

        tls.cleanup = cleanup ;
        tls.key = ::TlsAlloc() ;
        if(tls.key != TLS_OUT_OF_INDEXES)
        {
            tls.inited = true ;
            tlsidx = last_index ;

            __tls_last_index__ = last_index + 1 ;
        }
        break ;
    }

    ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;

    return tlsidx ;
}

void wthr_tls_delete(int tls)
{
    if(tls <= 0 || tls >= kTLSMaxSize)
        return ;

    wthr_tls_t& node = __tls_nodes__[tls] ;

    ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;

    if(node.inited == true)
    {
        if(node.key != TLS_OUT_OF_INDEXES)
        {
            ::TlsFree(node.key) ;
            node.key = TLS_OUT_OF_INDEXES ;
        }

        node.inited = false ;
        node.cleanup = NULL ;
    }

    ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;
}

void wthr_tls_cleanup()
{
    ::AcquireSRWLockExclusive(&__tlsidx_rwlock__) ;
    for(int index = 1 ; index < kTLSMaxSize ; ++index)
    {
        wthr_tls_t& node = __tls_nodes__[index] ;
        if(node.inited == false)
            continue ;

        if(node.key != TLS_OUT_OF_INDEXES)
        {
            ::TlsFree(node.key) ;
            node.key = TLS_OUT_OF_INDEXES ;
        }
        
        node.inited = false ;
        node.cleanup = NULL ;
    }
    ::ReleaseSRWLockExclusive(&__tlsidx_rwlock__) ;
}

int wthr_tls_first()
{
    int result = -1 ;
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    for(int index = 1 ; index < kTLSMaxSize ; ++index)
    {
        wthr_tls_t& node = __tls_nodes__[index] ;
        if(node.inited == false || node.key == TLS_OUT_OF_INDEXES)
            continue ;

        result = index ;
        break ;
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;

    return result ;
}

int wthr_tls_next(int prev)
{
    int result = -1 ;
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    for(int index = prev + 1 ; index < kTLSMaxSize ; ++index)
    {
        wthr_tls_t& node = __tls_nodes__[index] ;
        if(node.inited == false || node.key == TLS_OUT_OF_INDEXES)
            continue ;

        result = index ;
        break ;
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;

    return result ;
}

int wthr_tls_set_val(int key , void * val)
{
    int result = -1 ;
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    if(key > 0 && key < kTLSMaxSize)
    {
        wthr_tls_t&  tls = __tls_nodes__[key] ;
        if(tls.inited == true && tls.key != TLS_OUT_OF_INDEXES)
        {
            if(::TlsSetValue(tls.key , val) == TRUE)
                result = 0 ;
        }
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;
    return result ;
}

void * wthr_tls_get_val(int key)
{
    void * val = NULL ;
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    if(key > 0 && key < kTLSMaxSize)
    {
        wthr_tls_t&  tls = __tls_nodes__[key] ;
        if(tls.inited == true && tls.key != TLS_OUT_OF_INDEXES)
        {
            val = ::TlsGetValue(tls.key) ;
        }
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;
    return val ;
}

 void wthr_tls_free_val(int key) 
 {
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    if(key > 0 && key < kTLSMaxSize)
    {
        wthr_tls_t&  tls = __tls_nodes__[key] ;
        if(tls.inited == true && tls.key != TLS_OUT_OF_INDEXES)
        {
            void *val = ::TlsGetValue(tls.key) ;
            if(val != NULL)
            {
                ::TlsSetValue(tls.key , NULL) ;
                if(tls.cleanup != NULL)
                    tls.cleanup(val) ;
            }
        }
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;
 }

 void wthr_tls_cleanup_vals() 
 {
    ::AcquireSRWLockShared(&__tlsidx_rwlock__) ;
    for(int key = 1 ; key < kTLSMaxSize ; ++key)
    {
        wthr_tls_t&  tls = __tls_nodes__[key] ;
        if(tls.inited == true && tls.key != TLS_OUT_OF_INDEXES)
        {
            void * val = ::TlsGetValue(tls.key) ;
            if(val != NULL)
            {
                ::TlsSetValue(tls.key , NULL) ;
                if(tls.cleanup != NULL)
                    tls.cleanup(val) ;
            }
        }
    }
    ::ReleaseSRWLockShared(&__tlsidx_rwlock__) ;
 }


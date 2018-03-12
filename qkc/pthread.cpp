
#include <pthread.h>
#include <wintf/wobj.h>
#include <wintf/wthr.h>
#include <windows.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

typedef struct _st_wthr_start_data
{
	void * (*start_routine)(void *) ;
    void * param ;
    wthr_info_t * info ;
} wthr_start_data_t;


DWORD WINAPI WinThreadFunction(LPVOID lpParam )
{
    wthr_start_data_t * data = (wthr_start_data_t *)lpParam ;
    wthr_info_t * info  = ::wthr_info_get() ;
    info->wid  = ::wobj_set(WOBJ_THRD , ::GetCurrentThread() , info) ;    
    data->info = info ;
    data->start_routine(data->param) ;
    return 0 ;
}

int pthread_create(pthread_t * newthread , const pthread_attr_t * attr , void *(*start_routine)(void *) , void * arg) 
{
	wthr_start_data_t * data = NULL ;
	size_t data_size = sizeof(wthr_start_data_t) ;
	data = (wthr_start_data_t *)::HeapAlloc(::default_heap_get() , 0 , data_size) ;
    ::memset(data , 0 , data_size) ;

	data->param = arg ;
	data->start_routine = start_routine ;

    DWORD tid = 0 ;
	HANDLE handle = ::CreateThread(NULL , 0 , WinThreadFunction , data , CREATE_SUSPENDED , &tid) ;
	if(handle == NULL || handle == INVALID_HANDLE_VALUE)
	{
		::HeapFree(::default_heap_get() , 0 , data) ;
		return -1 ;
	}
    wthr_info_t * info = data->info ;
	info->handle = (uintptr_t)handle ;
    info->tid = (int)tid ;

    if(newthread != NULL)
        *newthread = info->wid ;

    ::ResumeThread(handle) ;
	return 0;
}

void pthread_exit(void *retval) 
{
    ::ExitThread(0) ;
}

int pthread_join(pthread_t th , void **thread_return)
{
    wobj_t * obj = wobj_get(th) ;
    if(obj == NULL || obj->type != WOBJ_THRD)
        return -1 ;

    HANDLE handle = obj->handle ;
    DWORD ret = WaitForSingleObject(handle , INFINITE) ;
    if(ret == WAIT_OBJECT_0)
        return 0 ;
    else
        return -1 ;
}

int pthread_detach(pthread_t th) 
{
    return 0 ;
}

pthread_t pthread_self(void) 
{
    wthr_info_t * info = wthr_info_get() ;
    if(info == NULL)
        return 0 ;
    else
        return info->wid ;
}

int pthread_equal(pthread_t thread1 , pthread_t thread2) 
{
    return (thread1 == thread2) ;
}

int pthread_yield(void) 
{
    if(::SwitchToThread() == TRUE)
        return 0 ;
    else
        return -1 ;
}

static BOOL WINAPI InitOnceFunc(PINIT_ONCE init_once , PVOID parameter , PVOID * context)
{
    init_routine init = (init_routine) parameter ;
    init() ;
    return TRUE ;
}

int pthread_once(pthread_once_t *once_control , init_routine routine)
{
    if(::InitOnceExecuteOnce((PINIT_ONCE)once_control , InitOnceFunc , routine , NULL) == TRUE)
        return 0 ;
    else
        return -1 ;
}

typedef struct __st_pthread_mutex_data{
    HANDLE handle ;
    LPCRITICAL_SECTION handle_critical_section ;
} pthread_mutex_data;

int pthread_mutex_init(pthread_mutex_t *mutex , const pthread_mutexattr_t *mutexattr) 
{
    HANDLE handle = ::CreateMutex(NULL , FALSE , NULL) ;
    if(handle == INVALID_HANDLE_VALUE)
        return -1 ;

    pthread_mutex_data * data = (pthread_mutex_data *)::malloc(sizeof(pthread_mutex_data)) ;
    data->handle = handle ;
    data->handle_critical_section = NULL ;

    int wid = wobj_set(WOBJ_MUTEX , handle , data) ;
    *mutex = wid ;
    return 0 ;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int wid = *mutex ;
    wobj_t *obj = wobj_get(wid) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX)
        return -1 ;

    pthread_mutex_data * data = (pthread_mutex_data *)obj->addition ;
    if(data == NULL)
        return -1 ;

    HANDLE handle = data->handle ;
    LPCRITICAL_SECTION cs = data->handle_critical_section ;
    if(handle != INVALID_HANDLE_VALUE)
    {
        if(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
        {
            if(cs != NULL)
            {
                ::DeleteCriticalSection(cs) ;
            }
            ::CloseHandle(handle) ;
        }

        data->handle_critical_section = NULL ;
        data->handle = INVALID_HANDLE_VALUE ;
    }

    ::wobj_del(wid) ;
    return 0 ;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) 
{
    int wid = *mutex ;
    wobj_t * obj = wobj_get(wid) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX)
        return -1 ;

    if(::WaitForSingleObject(obj->handle , 0) == WAIT_OBJECT_0)
        return 0 ;
    else
        return -1 ;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) 
{
    int wid = *mutex ;
    wobj_t * obj = wobj_get(wid) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX)
        return -1 ;

    if(::WaitForSingleObject(obj->handle , INFINITE) == WAIT_OBJECT_0)
        return 0 ;
    else
        return -1 ;
}

int pthread_mutex_timedlock(pthread_mutex_t * mutex , const struct timespec * abstime) 
{

}

int pthread_mutex_unlock(pthread_mutex_t *mutex) 
{

}


int pthread_rwlock_init(pthread_rwlock_t * rwlock , const pthread_rwlockattr_t * attr) 
{

}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) 
{

}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) 
{

}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) 
{

}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{

}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) 
{

}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{

}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{

}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) 
{

}



int pthread_cond_init(pthread_cond_t * cond , const pthread_condattr_t * cond_attr) 
{

}

int pthread_cond_destroy(pthread_cond_t *cond) 
{

}

int pthread_cond_signal(pthread_cond_t *cond) 
{

}

int pthread_cond_broadcast(pthread_cond_t *cond) 
{

}

int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex) 
{

}


int pthread_cond_timedwait(pthread_cond_t * cond , pthread_mutex_t * mutex , const struct timespec * abstime) 
{

}

int pthread_condattr_init(pthread_condattr_t *attr) 
{

}

int pthread_condattr_destroy(pthread_condattr_t *attr) 
{

}

int pthread_condattr_getpshared(const pthread_condattr_t * attr , int * pshared) 
{

}

int pthread_condattr_setpshared(pthread_condattr_t *attr , int pshared) 
{

}


int pthread_spin_init(pthread_spinlock_t *lock , int pshared) 
{
    if(lock == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }
    if(pshared != 0)
    {
        errno = ENOSYS ;
        return -1 ;
    }

    lock->allow = 1 ;
    lock->token = 0 ;
    lock->owner = 0 ;

    return 0 ;
}

int pthread_spin_destroy(pthread_spinlock_t *lock) 
{
    lock->allow = lock->token ;
    lock->owner = 0 ;
    return 0 ;
}

int pthread_spin_lock(pthread_spinlock_t *lock) 
{
    LONG token = ::InterlockedIncrement(&lock->token) ;
    uint32_t counter = 0 ;
    while(lock->allow < token)
    {
        ++counter ;
        if(counter >= 1024)
        {
            counter = 0 ;
            ::SwitchToThread() ;
        }
    }

    if(lock->allow != token)
        return -1 ;

    lock->allow = token ;
    lock->owner = ::getpid() ;
    ::MemoryBarrier() ;
    return 0 ;
}

int pthread_spin_trylock(pthread_spinlock_t *lock) 
{
    long token = lock->token;
    if(lock->allow == token + 1)
    {
        if(::InterlockedCompareExchange(&lock->token , token + 1, token) == token + 1)
        {
            lock->owner = ::getpid() ;
            ::MemoryBarrier() ;
            return 0 ;
        }        
    }

    return -1 ;
}

int pthread_spin_unlock(pthread_spinlock_t *lock) 
{
    lock->owner = 0 ;
    lock->allow++ ;
    return 0 ;
}

int pthread_key_create(pthread_key_t *key , void(*destr_function)(void *)) 
{
    DWORD tls = TlsAlloc() ;
    if(tls == TLS_OUT_OF_INDEXES)
    {
        errno = ENOMEM ;
        return -1 ;
    }

    *key = (pthread_key_t)tls;
    return 0 ;
}

int pthread_key_delete(pthread_key_t key) 
{
    DWORD tls = (DWORD)key ;
    if(tls == TLS_OUT_OF_INDEXES)
        return -1 ;

    ::TlsFree(tls) ;
    return 0 ;
}

void *pthread_getspecific(pthread_key_t key) 
{
    return ::TlsGetValue((DWORD)key) ;
}

int pthread_setspecific(pthread_key_t key , const void *pointer) 
{
    if(::TlsSetValue((DWORD)key , (LPVOID)pointer) == TRUE)
        return 0 ;
    else
        return -1 ;
}


int pthread_getcpuclockid(pthread_t thread_id , clockid_t *clock_id)
{
    errno = ENOSYS ;
    return -1 ;
}


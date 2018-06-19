
#include <pthread.h>
#include <wintf/wobj.h>
#include <wintf/wthr.h>
#include <wintf/wtime.h>
#include <windows.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

static SRWLOCK __pthread_mutex_anonymous__ = SRWLOCK_INIT ;
bool pthread_mutex_anonymous_init(pthread_mutex_t *mutex)
{
    bool result = true ;
    if(mutex->index == 0)
    {
        ::AcquireSRWLockExclusive(&__pthread_mutex_anonymous__) ;
        result = (pthread_mutex_init(mutex , NULL) == 0) ;
        ::ReleaseSRWLockExclusive(&__pthread_mutex_anonymous__) ;
    }

    return result ;
}

int pthread_mutex_init(pthread_mutex_t *mutex , const pthread_mutexattr_t *mutexattr) 
{
    HANDLE handle = ::CreateMutex(NULL , FALSE , NULL) ;
    if(handle == INVALID_HANDLE_VALUE)
        return -1 ;

    pthread_mutex_data * data = (pthread_mutex_data *)::malloc(sizeof(pthread_mutex_data)) ;
    data->handle = handle ;
    data->handle_critical_section = NULL ;

    int wid = wobj_set(WOBJ_MUTEX , handle , data) ;
    mutex->index = wid ;
    return 0 ;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int wid = mutex->index ;
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
                ::free(cs) ;
            }
            ::CloseHandle(handle) ;
        }

        data->handle_critical_section = NULL ;
        data->handle = INVALID_HANDLE_VALUE ;
    }

    ::wobj_del(wid) ;
    return 0 ;
}

static inline HANDLE mutex_handle(pthread_mutex_t * mutex)
{
    wobj_t * obj = wobj_get(mutex->index) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX)
        return INVALID_HANDLE_VALUE ;
    else
        return obj->handle ;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) 
{
    if(pthread_mutex_anonymous_init(mutex) == false)
        return -1 ;

    HANDLE handle = mutex_handle(mutex) ;
    if(::WaitForSingleObject(handle , 0) == WAIT_OBJECT_0)
        return 0 ;
    else
        return -1 ;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) 
{
    if(pthread_mutex_anonymous_init(mutex) == false)
        return -1 ;

    HANDLE handle = mutex_handle(mutex) ;
    if(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
        return 0 ;
    else
        return -1 ;
}

int pthread_mutex_timedlock(pthread_mutex_t * mutex , const struct timespec * abstime) 
{
    if(pthread_mutex_anonymous_init(mutex) == false)
        return -1 ;

    DWORD elapse = (DWORD)ElapseToMSec(abstime) ;
    if(elapse == 0)
    {
         if(pthread_mutex_trylock(mutex) == 0)
             return 0 ;

         errno = ETIMEDOUT ;
         return -1 ;
    }

    HANDLE handle = mutex_handle(mutex) ;
    if(::WaitForSingleObject(handle , elapse) == WAIT_OBJECT_0)
        return 0 ;

    errno = ETIMEDOUT ;
    return -1 ;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) 
{
    if(pthread_mutex_anonymous_init(mutex) == false)
        return -1 ;

    HANDLE handle = mutex_handle(mutex) ;
    if(::ReleaseMutex(handle) == TRUE)
        return 0 ;
    else
        return -1 ;
}

typedef struct __st_pthread_rwlock_data{
    HANDLE mutex ;
    SRWLOCK locker ;
    int writers ;
    int readers ;
}  pthread_rwlock_data_t ;


static SRWLOCK __pthread_rwlock_anonymous__ = SRWLOCK_INIT ;
bool pthread_rwlock_anonymous_init(pthread_rwlock_t *rwlock)
{
    bool result = true ;
    if(rwlock->index == 0)
    {
        ::AcquireSRWLockExclusive(&__pthread_rwlock_anonymous__) ;
        if(rwlock->index == 0)
            result = (pthread_rwlock_init(rwlock , NULL) == 0) ;
        ::ReleaseSRWLockExclusive(&__pthread_rwlock_anonymous__) ;
    }

    return result ;
}

bool rwlock_handle(pthread_rwlock_t *rwlock , HANDLE * handle , pthread_rwlock_data_t ** data)
{
    int widx = rwlock->index ;
    wobj_t * obj = wobj_get(widx) ;
    if(obj == NULL || obj->type != WOBJ_RWLOCK || obj->handle == NULL || obj->addition == NULL)
        return false ;

    *handle = obj->handle ;
    *data = (pthread_rwlock_data_t *)obj->addition ;
    return true ;
}


int pthread_rwlock_init(pthread_rwlock_t * rwlock , const pthread_rwlockattr_t * attr) 
{
    HANDLE handle = ::CreateMutex(NULL , TRUE , NULL) ;
    size_t data_size = sizeof(pthread_rwlock_data_t) ;
    pthread_rwlock_data_t * data = (pthread_rwlock_data_t *)::malloc(data_size) ;
    if(data != NULL)        
    {
        ::memset(data , 0 , data_size) ;
        data->mutex = handle ;
        ::InitializeSRWLock(&data->locker) ;

        int wid = wobj_set(WOBJ_RWLOCK , handle , data) ;
        rwlock->index = wid ;
        ::ReleaseMutex(handle) ;
    }
    else
    {
        ::ReleaseMutex(handle) ;
        ::CloseHandle(handle) ;
        return -1 ;
    }
    
    return 0 ;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) 
{
    int widx = rwlock->index ;
    wobj_t * obj = wobj_get(widx) ;
    if(obj == NULL || obj->type != WOBJ_RWLOCK || obj->handle == NULL)
        return -1 ;

    HANDLE handle = obj->handle ;
    if(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
    {
        obj->handle = NULL ;
        pthread_rwlock_data_t * data = (pthread_rwlock_data_t *)obj->addition ;
        if(data != NULL)
        {
            obj->addition = NULL;
            ::free(data) ;
        }

        ::ReleaseMutex(handle) ;
        ::CloseHandle(handle) ;
        wobj_del(widx) ;
    }

    return 0 ;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) 
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    bool owner = false ;
    while(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
    {
        if(data->writers == 0)
        {
            data->readers++ ;
            owner = true ;
            ::ReleaseMutex(handle) ;
            break ;
        }

        ::ReleaseMutex(handle) ;
    }

    if(owner == true)
        ::AcquireSRWLockShared(&data->locker) ;

    return (owner?0:-1) ;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) 
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    if(::WaitForSingleObject(handle , 0) != WAIT_OBJECT_0)
        return -1 ;

    bool owner = false ;
    if(data != NULL)
    {
        if(data->writers == 0)
        {
            data->readers++ ;
            owner = true ;
        }
    }

    if(owner == true)
        ::AcquireSRWLockShared(&data->locker) ;

    ::ReleaseMutex(handle) ;
    return (owner?0:-1) ;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    uint64_t elapse = ElapseToMSec(abstime) ;
    if(::WaitForSingleObject(handle , (DWORD)elapse) != WAIT_OBJECT_0)
        return -1 ;

    bool owner = false ;
    if(data != NULL)
    {
        if(data->writers == 0)
        {
            data->readers++ ;
            owner = true ;
        }
    }

    if(owner == true)
        ::AcquireSRWLockShared(&data->locker) ;

    ::ReleaseMutex(handle) ;
    return (owner?0:-1) ;
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) 
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    bool owner = false ;
    while(::WaitForSingleObject(handle , INFINITE) == WAIT_OBJECT_0)
    {
        if(data->readers == 0)
        {
            data->writers++ ;
            owner = true ;
            ::ReleaseMutex(handle) ;
            break ;
        }
    }

    if(owner == true)
        ::AcquireSRWLockExclusive(&data->locker) ;

    return (owner?0:-1) ;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    if(::WaitForSingleObject(handle , 0) != WAIT_OBJECT_0)
        return -1 ;

    bool owner = false ;
    if(data->readers == 0)
    {
        data->writers++ ;
        owner = true ;
        ::AcquireSRWLockExclusive(&data->locker) ;
    }        

    ::ReleaseMutex(handle) ;
    return (owner?0:-1) ;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
    if(pthread_rwlock_anonymous_init(rwlock) == false)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    uint64_t elapse = ElapseToMSec(abstime) ;
    bool owner = false ;
    if(::WaitForSingleObject(handle , (DWORD)elapse) != WAIT_OBJECT_0)
        return -1 ;

    if(data->readers == 0)
    {
        data->writers++ ;
        owner = true ;
    }
    ::ReleaseMutex(handle) ;

    if(owner == true)
        ::AcquireSRWLockExclusive(&data->locker) ;

    return (owner?0:-1) ;
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) 
{
    if(rwlock->index == 0)
        return -1 ;

    HANDLE handle = NULL ;
    pthread_rwlock_data_t * data = NULL ;
    if(rwlock_handle(rwlock , &handle , &data) == false)
        return -1;

    if(::WaitForSingleObject(handle , INFINITE) != WAIT_OBJECT_0)
        return -1 ;

    if(data->readers != 0)
    {
        data->readers-- ;
        ::ReleaseSRWLockShared(&data->locker) ;
    }
    else if(data->writers != 0)
    {
        data->writers-- ;
        ::ReleaseSRWLockExclusive(&data->locker) ;
    }

    ::ReleaseMutex(handle) ;
    return 0 ;
}


static SRWLOCK __pthread_cond_anonymous__ = SRWLOCK_INIT ;
bool pthread_cond_anonymous_init(pthread_cond_t *cond)
{
    bool result = true ;
    if(cond->flag != 1)
    {
        ::AcquireSRWLockExclusive(&__pthread_cond_anonymous__) ;
        if(cond->flag != 1)
            result = (pthread_cond_init(cond , NULL) == 0) ;
        ::ReleaseSRWLockExclusive(&__pthread_cond_anonymous__) ;
    }

    return result ;
}

int pthread_cond_init(pthread_cond_t * cond , const pthread_condattr_t * cond_attr) 
{
    cond->flag = 1 ;
    cond->pad = 0 ;
    ::InitializeConditionVariable((PCONDITION_VARIABLE)&cond->locker) ;
    return 0 ;
}

int pthread_cond_destroy(pthread_cond_t *cond) 
{
    if(cond->flag == 1)
    {
        ::WakeAllConditionVariable((PCONDITION_VARIABLE)&cond->locker) ;
        cond->flag = 0 ;        
        cond->locker = 0 ;
        return 0 ;
    }
    else
        return -1 ;
}

int pthread_cond_signal(pthread_cond_t *cond) 
{
    if(pthread_cond_anonymous_init(cond) == false)
        return -1 ;

    if(cond->flag == 1)
    {
        ::WakeConditionVariable((PCONDITION_VARIABLE)&cond->locker) ;
        return 0 ;
    }
    else
        return -1 ;
}

int pthread_cond_broadcast(pthread_cond_t *cond) 
{
    if(pthread_cond_anonymous_init(cond) == false)
        return -1 ;

    if(cond->flag == 1)
    {
        ::WakeAllConditionVariable((PCONDITION_VARIABLE)&cond->locker) ;
        return 0 ;
    }
    else
        return -1 ;
}

int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex) 
{
    if(pthread_cond_anonymous_init(cond) == false)
        return -1 ;

    int widx = mutex->index ;
    wobj_t * obj = wobj_get(widx) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX || obj->addition == NULL)
        return -1 ;

    pthread_mutex_data * data = (pthread_mutex_data *)obj->addition ;
    if(data->handle_critical_section == NULL)
    {
        LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)::malloc(sizeof(CRITICAL_SECTION)) ;
        ::InitializeCriticalSection(cs) ;
        data->handle_critical_section = cs ;
    }
    ::EnterCriticalSection(data->handle_critical_section) ;
    ::ReleaseMutex(obj->handle) ;


    BOOL result = ::SleepConditionVariableCS((PCONDITION_VARIABLE)&cond->locker , data->handle_critical_section , INFINITE) ;

    ::WaitForSingleObject(obj->handle , INFINITE) ;
    ::LeaveCriticalSection(data->handle_critical_section) ;

    return (result?0:-1) ;
}

int pthread_cond_timedwait(pthread_cond_t * cond , pthread_mutex_t * mutex , const struct timespec * abstime) 
{
    if(pthread_cond_anonymous_init(cond) == false)
        return -1 ;

    int widx = mutex->index ;
    wobj_t * obj = wobj_get(widx) ;
    if(obj == NULL || obj->type != WOBJ_MUTEX || obj->addition == NULL)
        return -1 ;

    pthread_mutex_data * data = (pthread_mutex_data *)obj->addition ;
    if(data->handle_critical_section == NULL)
    {
        LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)::malloc(sizeof(CRITICAL_SECTION)) ;
        ::InitializeCriticalSection(cs) ;
        data->handle_critical_section = cs ;
    }
    ::EnterCriticalSection(data->handle_critical_section) ;
    ::ReleaseMutex(obj->handle) ;

    uint64_t timeout = ElapseToMSec(abstime) ;
    BOOL result = ::SleepConditionVariableCS((PCONDITION_VARIABLE)&cond->locker , data->handle_critical_section , (DWORD)timeout) ;

    ::WaitForSingleObject(obj->handle , INFINITE) ;
    ::LeaveCriticalSection(data->handle_critical_section) ;

    return (result?0:-1) ;
}

int pthread_condattr_init(pthread_condattr_t *attr) 
{
    return 0 ;
}

int pthread_condattr_destroy(pthread_condattr_t *attr) 
{
    return 0 ;
}

int pthread_condattr_getpshared(const pthread_condattr_t * attr , int * pshared) 
{
    return 0 ;
}

int pthread_condattr_setpshared(pthread_condattr_t *attr , int pshared) 
{
    return 0 ;
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

int pthread_sigmask (int how , const sigset_t * newmask, sigset_t * oldmask)
{
    return 0 ;
}

int pthread_kill (pthread_t threadid, int signo)
{
    return 0 ;
}


int pthread_mutexattr_init (pthread_mutexattr_t * attr)
{
    return 0 ;
}

int pthread_mutexattr_destroy (pthread_mutexattr_t *attr)
{
    return 0 ;
}

int pthread_mutexattr_getpshared (const pthread_mutexattr_t * attr, int * pshared)
{
    return 0 ;
}

int pthread_mutexattr_setpshared (pthread_mutexattr_t * attr, int pshared)
{
    return 0 ;
}

int pthread_mutexattr_gettype (const pthread_mutexattr_t * attr, int * kind)
{
    return 0 ;
}

int pthread_mutexattr_settype (pthread_mutexattr_t * attr, int kind)
{
    return 0 ;
}

int pthread_mutexattr_getprotocol (const pthread_mutexattr_t * attr , int * protocol)
{
    return 0 ;
}

int pthread_mutexattr_setprotocol (pthread_mutexattr_t * attr, int protocol)
{
    return 0 ;
}

int pthread_mutexattr_getprioceiling (const pthread_mutexattr_t * attr, int * prioceiling)
{
    return 0 ;
}

int pthread_mutexattr_setprioceiling (pthread_mutexattr_t * attr, int prioceiling)
{
    return 0 ;
}

int pthread_mutexattr_getrobust_np (const pthread_mutexattr_t * attr, int * robustness)
{
    return 0 ;
}

int pthread_mutexattr_setrobust_np (pthread_mutexattr_t * attr, int robustness)
{
    return 0 ;
}



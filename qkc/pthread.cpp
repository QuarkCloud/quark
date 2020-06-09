
#include <pthread.h>
#include <windows.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "wobjs/DateTime.h"
#include "wobjs/Thread.h"
#include "wobjs/ThreadLocal.h"
#include "wobjs/Object.h"
#include "wobjs/ObjMgr.h"
#include "wimpl/PThreadMutex.h"
#include "wimpl/PThreadRWLocker.h"
#include "wimpl/PThreadCond.h"

typedef void * (*pthread_start_routine_t)(void * param);
class pthread : public qkc::Thread {
public:
	pthread(pthread_start_routine_t routine, void * param) { routine_ = routine; param_ = param; }
	virtual ~pthread() {}

	virtual void Run()
	{
		if (routine_ != NULL)
			routine_(param_);
	}
private:
	pthread_start_routine_t routine_;
	void * param_;
};

int pthread_create(pthread_t * newthread , const pthread_attr_t * attr , void *(*start_routine)(void *) , void * arg) 
{
	pthread * thr = new pthread(start_routine, arg);
	if (thr->Init() == false)
	{
		delete thr;
		return -1;
	}

	if (newthread != NULL)
		*newthread = thr->OIndex();
	return 0;
}

void pthread_exit(void *retval) 
{
    ::ExitThread(0) ;
}

int pthread_join(pthread_t th , void **thread_return)
{
    if(thread_return != NULL)
        *thread_return = NULL ;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(th);
	if (obj == NULL || obj->IsThread() == false)
		return -1;

	qkc::Thread * thread = (qkc::Thread *)obj;
	thread->Wait();

	if (thread_return != NULL)
		*thread_return = (void *)thread->ExitCode();

	return 0;
}

int pthread_detach(pthread_t th) 
{
    return 0 ;
}

pthread_t pthread_self(void) 
{
	const qkc::ThreadInfo * info = qkc::Thread::DefaultInfo();
	return info->Owner()->OIndex();
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

int pthread_mutex_init(pthread_mutex_t *mutex , const pthread_mutexattr_t *mutexattr) 
{
	qkc::PThreadMutex * locker = new qkc::PThreadMutex();
	mutex->index = locker->OIndex();
    return 0 ;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	if (mutex == NULL)
		return -1;

	int index = mutex->index;
	qkc::Object *obj = qkc::ObjMgr::Singleton().Get(index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return -1;

	qkc::Object *dobj = NULL;
	if (qkc::ObjMgr::Singleton().Delete(index, dobj) == false || dobj != obj)
		return -1;

	delete obj;
	return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) 
{
	if (mutex == NULL)
		return -1;
	if(qkc::EnsurePThreadMutexInited(&mutex->index) == false)
        return -1 ;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(mutex->index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return -1;

	qkc::PThreadMutex * locker = (qkc::PThreadMutex *)obj;
	return locker->TryLock();
}

int pthread_mutex_lock(pthread_mutex_t *mutex) 
{
	if (mutex == NULL)
		return -1;
	if (qkc::EnsurePThreadMutexInited(&mutex->index) == false)
		return -1;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(mutex->index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return -1;

	qkc::PThreadMutex * locker = (qkc::PThreadMutex *)obj;
	return locker->Lock();
}

int pthread_mutex_timedlock(pthread_mutex_t * mutex , const struct timespec * abstime) 
{
	if (mutex == NULL)
		return -1;
	if (qkc::EnsurePThreadMutexInited(&mutex->index) == false)
		return -1;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(mutex->index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return -1;

	qkc::PThreadMutex * locker = (qkc::PThreadMutex *)obj;

	return locker->TimedLock((int)qkc::ElapseToMSec(abstime));
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) 
{
	if (mutex == NULL)
		return -1;
	if (qkc::EnsurePThreadMutexInited(&mutex->index) == false)
		return -1;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(mutex->index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return -1;

	qkc::PThreadMutex * locker = (qkc::PThreadMutex *)obj;

	return locker->Unlock();
}

int pthread_rwlock_init(pthread_rwlock_t * rwlock , const pthread_rwlockattr_t * attr) 
{
	if (rwlock == NULL)
		return -1;

	qkc::PThreadRWLocker * locker = new qkc::PThreadRWLocker();
	rwlock->index = locker->OIndex();
	return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) 
{
	if (rwlock == NULL)
		return -1;
	int index = rwlock->index;
	if(index == 0)
		return 0;
	rwlock->index = 0;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(index);
	if (obj == NULL || obj->IsPThreadRWLock() == false)
		return -1;

	qkc::Object * dobj = NULL;
	if (qkc::ObjMgr::Singleton().Delete(index, dobj) == false || dobj != obj)
		return -1;

	delete dobj;
	return 0;	
}

inline qkc::PThreadRWLocker * ConfirmRWLocker(pthread_rwlock_t * rwlock)
{
	if (rwlock == NULL)
		return NULL;

	if (qkc::EnsurePThreadRWLockerInited(&rwlock->index) == false)
		return NULL;

	int index = rwlock->index;
	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(index);
	if (obj == NULL || obj->IsPThreadRWLock() == false)
		return NULL;

	return (qkc::PThreadRWLocker *)obj;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;

	return locker->RdLock();
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;

	return locker->TryRdLock();
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;

	int msec = (int)qkc::ElapseToMSec(abstime);
	return locker->TimedRdLock(msec);
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;
	return locker->WrLock();
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;
	return locker->TryWrLock();
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;

	int msec = (int)qkc::ElapseToMSec(abstime);
	return locker->TimedWrLock(msec);
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) 
{
	qkc::PThreadRWLocker * locker = ConfirmRWLocker(rwlock);
	if (locker == NULL)
		return -1;
	return locker->RWUnLock();
}

inline qkc::PThreadCond * ConfirmCond(pthread_cond_t * cond)
{
	if (cond == NULL)
		return NULL;

	if (qkc::EnsurePThreadCondInited(&cond->index) == false)
		return NULL;

	int index = cond->index;
	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(index);
	if (obj == NULL || obj->IsPThreadCond() == false)
		return NULL;

	return (qkc::PThreadCond *)obj;
}

int pthread_cond_init(pthread_cond_t * cond , const pthread_condattr_t * cond_attr) 
{
	if (cond == NULL)
		return -1;

	qkc::PThreadCond * locker = new qkc::PThreadCond();
	if (qkc::ObjMgr::Singleton().Add(locker->Handle(), locker) <= 0)
	{
		delete locker;
		return -1;
	}

	cond->index = locker->OIndex();
    return 0 ;
}

int pthread_cond_destroy(pthread_cond_t *cond) 
{
	if (cond == NULL || cond->index == 0)
		return -1;

	int index = cond->index;
	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(index);
	if (obj == NULL || obj->IsPThreadCond() == false)
		return -1;

	qkc::Object * dobj = NULL;
	if (qkc::ObjMgr::Singleton().Delete(index, dobj) == false || dobj == NULL)
		return -1;

	delete dobj;
	return 0;	
}

int pthread_cond_signal(pthread_cond_t *cond) 
{
	if (cond == NULL)
		return -1;

	qkc::PThreadCond * locker = ConfirmCond(cond);
	if (locker == NULL)
		return -1;

	return locker->Signal();
}

int pthread_cond_broadcast(pthread_cond_t *cond) 
{
	if (cond == NULL)
		return -1;

	qkc::PThreadCond * locker = ConfirmCond(cond);
	if (locker == NULL)
		return -1;

	return locker->BroadCast();
}

inline qkc::PThreadMutex * GetPThreadMutex(pthread_mutex_t * mutex)
{
	if (mutex == NULL || mutex->index <= 0)
		return NULL;

	qkc::Object * obj = qkc::ObjMgr::Singleton().Get(mutex->index);
	if (obj == NULL || obj->IsPThreadMutex() == false)
		return NULL;
	return (qkc::PThreadMutex *)obj;
}

int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex) 
{
	if (cond == NULL || mutex == NULL)
		return -1;

	qkc::PThreadMutex * ptm = GetPThreadMutex(mutex);
	if (ptm == NULL)
		return -1;

	qkc::PThreadCond * ptc = ConfirmCond(cond);
	if (ptc == NULL)
		return -1;

	return ptc->Wait(ptm);
}

int pthread_cond_timedwait(pthread_cond_t * cond , pthread_mutex_t * mutex , const struct timespec * abstime) 
{
	if (cond == NULL || mutex == NULL)
		return -1;

	qkc::PThreadMutex * ptm = GetPThreadMutex(mutex);
	if (ptm == NULL)
		return -1;

	qkc::PThreadCond * ptc = ConfirmCond(cond);
	if (ptc == NULL)
		return -1;

	return ptc->TimedWait(ptm , (int)qkc::ElapseToMSec(abstime));
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

int pthread_condattr_getclock (const pthread_condattr_t * attr, clockid_t * clock_id)
{
    return -1 ;
}

int pthread_condattr_setclock (pthread_condattr_t *attr , clockid_t clock_id)
{
    return -1 ;
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
    ::MemoryBarrier() ;
    return 0 ;
}

int pthread_key_create(pthread_key_t *key , void(*destr_function)(void *)) 
{
	if (key == NULL)
		return -1;

	qkc::ThreadLocal * tls = new qkc::ThreadLocal(destr_function);
	
	*key = tls->Index();
	return 0;
}

int pthread_key_delete(pthread_key_t key) 
{
	qkc::ThreadLocal * tls = NULL;
	qkc::ThreadLocalLogger::Singleton().DelByIndex(key, tls);

	if (tls != NULL)
		delete tls;
    return 0 ;
}

void *pthread_getspecific(pthread_key_t key) 
{
	qkc::ThreadLocal * tls = qkc::ThreadLocalLogger::Singleton().GetByIndex(key);
	if (tls == NULL)
		return NULL;
	return tls->GetData();
}

int pthread_setspecific(pthread_key_t key , const void *pointer) 
{
	qkc::ThreadLocal * tls = qkc::ThreadLocalLogger::Singleton().GetByIndex(key);
	if (tls == NULL)
		return -1;
	return tls->SetData(pointer);
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


int pthread_attr_init (pthread_attr_t *attr) 
{
    return -1 ;
}

int pthread_attr_destroy (pthread_attr_t *attr)
{
    return -1 ;
}

int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate)
{
    return -1 ;
}

int pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate)
{
    return -1 ;
}

int pthread_attr_getguardsize (const pthread_attr_t *attr, size_t *guardsize)
{
    return -1 ;
}

int pthread_attr_setguardsize (pthread_attr_t *attr, size_t guardsize)
{
    return -1 ;
}

int pthread_attr_getschedparam (const pthread_attr_t * attr, struct sched_param * param)
{
    return -1 ;
}

int pthread_attr_setschedparam (pthread_attr_t * attr, const struct sched_param * param)
{
    return -1 ;
}

int pthread_attr_getschedpolicy (const pthread_attr_t * attr, int * policy)
{
    return -1 ;
}

int pthread_attr_setschedpolicy (pthread_attr_t *attr, int policy)
{
    return -1 ;
}

int pthread_attr_getinheritsched (const pthread_attr_t * attr, int * inherit)
{
    return -1 ;
}

int pthread_attr_setinheritsched (pthread_attr_t *attr, int inherit)
{
    return -1 ;
}

int pthread_attr_getscope (const pthread_attr_t * attr, int * scope)
{
    return -1 ;
}

int pthread_attr_setscope (pthread_attr_t *attr, int scope)
{
    return -1 ;
}

int pthread_attr_getstackaddr (const pthread_attr_t * attr, void ** stackaddr)
{
    return -1 ;
}

int pthread_attr_setstackaddr (pthread_attr_t *attr, void *stackaddr)
{
    return -1 ;
}

int pthread_attr_getstacksize (const pthread_attr_t * attr, size_t * stacksize)
{
    return -1 ;
}

int pthread_attr_setstacksize (pthread_attr_t *attr, size_t stacksize)
{
    return -1 ;
}

int pthread_attr_getstack (const pthread_attr_t * attr, void ** stackaddr, size_t * stacksize)
{
    return -1 ;
}

int pthread_attr_setstack (pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
    return -1 ;
}

int pthread_attr_setaffinity_np (pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset)
{
    return -1 ;
}

int pthread_attr_getaffinity_np (const pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset)
{
    return -1 ;
}

int pthread_getattr_np (pthread_t th, pthread_attr_t *attr)
{
    return -1 ;
}





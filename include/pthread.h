
#ifndef __QKC_PTHREAD_H
#define __QKC_PTHREAD_H 1

#include <quark_compile.h>
#include <time.h>
#include <sched.h>
#include <signal.h>
#include <sys/types.h>


__BEGIN_DECLS

enum
{
  PTHREAD_CREATE_JOINABLE,
#define PTHREAD_CREATE_JOINABLE PTHREAD_CREATE_JOINABLE
  PTHREAD_CREATE_DETACHED
#define PTHREAD_CREATE_DETACHED PTHREAD_CREATE_DETACHED
};

/* Mutex types.  */
enum
{
  PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_ADAPTIVE_NP,
  PTHREAD_MUTEX_NORMAL = PTHREAD_MUTEX_TIMED_NP,
  PTHREAD_MUTEX_RECURSIVE = PTHREAD_MUTEX_RECURSIVE_NP,
  PTHREAD_MUTEX_ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK_NP,
  PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL, 
  PTHREAD_MUTEX_FAST_NP = PTHREAD_MUTEX_TIMED_NP
};


/* pthread attrs暂不实现 ，用得不多*/
typedef struct __st_pthread_attr{int dummy ;} pthread_attr_t;
typedef struct __st_pthread_mutex_attr{int dummy ;} pthread_mutexattr_t;
typedef struct __st_pthread_rwlock_attr{int dummy ;} pthread_rwlockattr_t;
typedef struct __st_pthread_cond_attr{int dummy ;} pthread_condattr_t;


typedef int pthread_t;
typedef struct __st_pthread_once{
    void * ptr ;
} pthread_once_t ;
#define PTHREAD_ONCE_INIT {NULL}

typedef struct __st_pthread_mutex{
    int index ;
} pthread_mutex_t ;
#define PTHREAD_MUTEX_INITIALIZER {0} 

/**
    系统原生的读写锁，不支持超时和try所以自行实现。
    暂时比较粗糙，能完成功能即可，等待后续优化。
*/
typedef struct __st_pthread_rwlock{int index ;} pthread_rwlock_t ;
#define PTHREAD_RWLOCK_INITIALIZER {0}

#define PTHREAD_COND_SIZE 16 
typedef struct __st_pthread_cond{
    int flag ;
	int index;
	int64_t pad;
} pthread_cond_t ;
#define PTHREAD_COND_INITIALIZER {0 , 0 , 0}

typedef struct __st_pthread_spinlock{
    volatile long token ;
    long allow ;
    pid_t owner ;
} pthread_spinlock_t;

typedef int pthread_key_t;


//1、线程本身，直接使用windows的线程
QKCAPI int pthread_create(pthread_t * newthread , const pthread_attr_t * attr , void *(*start_routine)(void *) , void * arg) ;
QKCAPI void pthread_exit(void *retval) ;
QKCAPI int pthread_join(pthread_t th , void **thread_return);
QKCAPI int pthread_detach(pthread_t th) ;
QKCAPI pthread_t pthread_self(void) ;
QKCAPI int pthread_equal(pthread_t thread1 , pthread_t thread2) ;
QKCAPI int pthread_yield(void) ;

typedef void (*init_routine)() ;
QKCAPI int pthread_once(pthread_once_t *once_control , init_routine) ;

QKCAPI int pthread_mutex_init(pthread_mutex_t *mutex , const pthread_mutexattr_t *mutexattr) ;
QKCAPI int pthread_mutex_destroy(pthread_mutex_t *mutex);
QKCAPI int pthread_mutex_trylock(pthread_mutex_t *mutex) ;
QKCAPI int pthread_mutex_lock(pthread_mutex_t *mutex) ;
QKCAPI int pthread_mutex_timedlock(pthread_mutex_t * mutex , const struct timespec * abstime) ;
QKCAPI int pthread_mutex_unlock(pthread_mutex_t *mutex) ;

QKCAPI int pthread_rwlock_init(pthread_rwlock_t * rwlock , const pthread_rwlockattr_t * attr) ;
QKCAPI int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) ;
QKCAPI int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) ;
QKCAPI int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) ;
QKCAPI int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) ;
QKCAPI int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) ;
QKCAPI int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
QKCAPI int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) ;
QKCAPI int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) ;


QKCAPI int pthread_cond_init(pthread_cond_t * cond , const pthread_condattr_t * cond_attr) ;
QKCAPI int pthread_cond_destroy(pthread_cond_t *cond) ;
QKCAPI int pthread_cond_signal(pthread_cond_t *cond) ;
QKCAPI int pthread_cond_broadcast(pthread_cond_t *cond) ;
QKCAPI int pthread_cond_wait(pthread_cond_t * cond , pthread_mutex_t * mutex) ;
QKCAPI int pthread_cond_timedwait(pthread_cond_t * cond , pthread_mutex_t * mutex , const struct timespec * abstime) ;

QKCAPI int pthread_condattr_init(pthread_condattr_t *attr) ;
QKCAPI int pthread_condattr_destroy(pthread_condattr_t *attr) ;
QKCAPI int pthread_condattr_getpshared(const pthread_condattr_t * attr , int * pshared) ;
QKCAPI int pthread_condattr_setpshared(pthread_condattr_t *attr , int pshared) ;
QKCAPI int pthread_condattr_getclock (const pthread_condattr_t * attr, clockid_t * clock_id) ;
QKCAPI int pthread_condattr_setclock (pthread_condattr_t *attr , clockid_t clock_id) ;


QKCAPI int pthread_spin_init(pthread_spinlock_t *lock , int pshared) ;
QKCAPI int pthread_spin_destroy(pthread_spinlock_t *lock) ;
QKCAPI int pthread_spin_lock(pthread_spinlock_t *lock) ;
QKCAPI int pthread_spin_trylock(pthread_spinlock_t *lock) ;
QKCAPI int pthread_spin_unlock(pthread_spinlock_t *lock) ;

QKCAPI int pthread_key_create(pthread_key_t *key , void(*destr_function)(void *)) ;
QKCAPI int pthread_key_delete(pthread_key_t key) ;
QKCAPI void *pthread_getspecific(pthread_key_t key) ;
QKCAPI int pthread_setspecific(pthread_key_t key , const void *pointer) ;

QKCAPI int pthread_getcpuclockid(pthread_t thread_id , clockid_t *clock_id);


/* Modify the signal mask for the calling thread.  The arguments have
   the same meaning as for sigprocmask(2). */
QKCAPI int pthread_sigmask (int how , const sigset_t * newmask, sigset_t * oldmask) ;

/* Send signal SIGNO to the given thread. */
QKCAPI int pthread_kill (pthread_t threadid, int signo) ;


QKCAPI int pthread_mutexattr_init (pthread_mutexattr_t * attr) ;
QKCAPI int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);
QKCAPI int pthread_mutexattr_getpshared (const pthread_mutexattr_t * attr, int * pshared);
QKCAPI int pthread_mutexattr_setpshared (pthread_mutexattr_t * attr, int pshared);
QKCAPI int pthread_mutexattr_gettype (const pthread_mutexattr_t * attr, int * kind);
QKCAPI int pthread_mutexattr_settype (pthread_mutexattr_t * attr, int kind);
QKCAPI int pthread_mutexattr_getprotocol (const pthread_mutexattr_t * attr , int * protocol);
QKCAPI int pthread_mutexattr_setprotocol (pthread_mutexattr_t * attr, int protocol);
QKCAPI int pthread_mutexattr_getprioceiling (const pthread_mutexattr_t * attr, int * prioceiling);
QKCAPI int pthread_mutexattr_setprioceiling (pthread_mutexattr_t * attr, int prioceiling);
QKCAPI int pthread_mutexattr_getrobust_np (const pthread_mutexattr_t * attr, int * robustness) ;
QKCAPI int pthread_mutexattr_setrobust_np (pthread_mutexattr_t * attr, int robustness);


QKCAPI int pthread_attr_init (pthread_attr_t *attr) ;
QKCAPI int pthread_attr_destroy (pthread_attr_t *attr);
QKCAPI int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate) ;
QKCAPI int pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate) ;
QKCAPI int pthread_attr_getguardsize (const pthread_attr_t *attr, size_t *guardsize) ;
QKCAPI int pthread_attr_setguardsize (pthread_attr_t *attr, size_t guardsize) ;
QKCAPI int pthread_attr_getschedparam (const pthread_attr_t * attr, struct sched_param * param) ;
QKCAPI int pthread_attr_setschedparam (pthread_attr_t * attr, const struct sched_param * param) ;
QKCAPI int pthread_attr_getschedpolicy (const pthread_attr_t * attr, int * policy) ;
QKCAPI int pthread_attr_setschedpolicy (pthread_attr_t *attr, int policy) ;
QKCAPI int pthread_attr_getinheritsched (const pthread_attr_t * attr, int * inherit) ;
QKCAPI int pthread_attr_setinheritsched (pthread_attr_t *attr, int inherit) ;
QKCAPI int pthread_attr_getscope (const pthread_attr_t * attr, int * scope) ;
QKCAPI int pthread_attr_setscope (pthread_attr_t *attr, int scope) ;
QKCAPI int pthread_attr_getstackaddr (const pthread_attr_t * attr, void ** stackaddr) ;
QKCAPI int pthread_attr_setstackaddr (pthread_attr_t *attr, void *stackaddr) ;
QKCAPI int pthread_attr_getstacksize (const pthread_attr_t * attr, size_t * stacksize) ;
QKCAPI int pthread_attr_setstacksize (pthread_attr_t *attr, size_t stacksize) ;
QKCAPI int pthread_attr_getstack (const pthread_attr_t * attr, void ** stackaddr, size_t * stacksize) ;
QKCAPI int pthread_attr_setstack (pthread_attr_t *attr, void *stackaddr, size_t stacksize) ;
QKCAPI int pthread_attr_setaffinity_np (pthread_attr_t *attr, size_t cpusetsize, const cpu_set_t *cpuset) ;
QKCAPI int pthread_attr_getaffinity_np (const pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset) ;
QKCAPI int pthread_getattr_np (pthread_t th, pthread_attr_t *attr) ;

__END_DECLS

#endif /* __QKC_PTHREAD_H */

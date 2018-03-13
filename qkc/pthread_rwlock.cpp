
#include <pthread.h>


int pthread_rwlock_init(pthread_rwlock_t * rwlock , const pthread_rwlockattr_t * attr) 
{
    return 0 ;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock) 
{
    return 0 ;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) 
{
    return 0 ;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) 
{
    return 0 ;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
    return 0 ;
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) 
{
    return 0 ;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    return 0 ;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t * rwlock , const struct timespec * abstime) 
{
    return 0 ;
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock) 
{
    return 0 ;
}

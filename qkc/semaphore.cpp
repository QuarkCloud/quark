
#include <semaphore.h>

int sem_init(sem_t * sem , int pshared, unsigned int value)
{
    return -1 ;
}

int sem_destroy(sem_t *sem)
{
    return -1 ;
}

sem_t * sem_open(const char * name , int oflag , mode_t mode , uint32_t value)
{
    return NULL ;
}

int sem_close(sem_t *sem)
{
    return -1 ;
}

int sem_unlink(const char * name)
{
    return -1 ;
}

int sem_wait(sem_t *sem)
{
    return -1 ;
}

int sem_timedwait(sem_t *sem ,  const struct timespec * abstime)
{
    return -1 ;
}

int sem_trywait(sem_t *sem)
{
    return -1 ;
}

int sem_post(sem_t *sem)
{
    return -1 ;
}

int sem_getvalue(sem_t *sem , int * sval)
{
    return -1 ;
}



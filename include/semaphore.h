#ifndef __QKC_SEMAPHORE_H
#define __QKC_SEMAPHORE_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <fcntl.h>

__BEGIN_DECLS

#define SEM_FAILED      ((sem_t *) 0)
#define SEM_VALUE_MAX   (2147483647)

#if __WORDSIZE == 64
    #define __SIZEOF_SEM_T	32
#else
    #define __SIZEOF_SEM_T	16
#endif

typedef union
{
    char      size[__SIZEOF_SEM_T] ;
    long int  align ;
} sem_t;

/**
    ��windows�£��������̱Ƚ����ѣ���Ҫ���ڹ����ڴ���ʵ�֣��ȷ��ţ��Ⱥ����ٽ����
*/
QKCAPI int sem_init(sem_t * sem , int pshared, unsigned int value);

QKCAPI int sem_destroy(sem_t *sem) ;

QKCAPI sem_t * sem_open(const char * name , int oflag , mode_t mode , uint32_t value) ;

QKCAPI int sem_close(sem_t *sem) ;

QKCAPI int sem_unlink(const char * name) ;

QKCAPI int sem_wait(sem_t *sem);

QKCAPI int sem_timedwait(sem_t *sem ,  const struct timespec * abstime);

QKCAPI int sem_trywait(sem_t *sem) ;

QKCAPI int sem_post(sem_t *sem) ;

QKCAPI int sem_getvalue(sem_t *sem , int * sval) ;


__END_DECLS


#endif	/** __QKC_SEMAPHORE_H */

#ifndef __QKC_SYS_SEM_H
#define __QKC_SYS_SEM_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>

__BEGIN_DECLS

/* Flags for `semop'.  */
#define SEM_UNDO	0x1000		/* undo the operation on exit */

/* Commands for `semctl'.  */
#define GETPID		11		/* get sempid */
#define GETVAL		12		/* get semval */
#define GETALL		13		/* get all semval's */
#define GETNCNT		14		/* get semncnt */
#define GETZCNT		15		/* get semzcnt */
#define SETVAL		16		/* set semval */
#define SETALL		17		/* set all semval's */


/* Data structure describing a set of semaphores.  */
struct semid_ds
{
    struct ipc_perm sem_perm;		    /* operation permission struct */
    time_t sem_otime;			        /* last semop() time */
    unsigned long int __unused1;
    time_t sem_ctime;			        /* last time changed by semctl() */
    unsigned long int __unused2;
    unsigned long int sem_nsems;		/* number of semaphores in set */
    unsigned long int __unused3;
    unsigned long int __unused4;
};

//#define _SEM_SEMUN_UNDEFINED	1

/* ipcs ctl cmds */
# define SEM_STAT 18
# define SEM_INFO 19

struct  seminfo
{
  int semmap;   /* # of entries in semaphore map */
  int semmni;   /* Maximum number of unique semaphore sets, system wide. */
  int semmns;   /* Maximum number of semaphores,system wide. */
  int semmnu;   /* Maximum number of undo structures, system wide. */
  int semmsl;   /* Maximum number of semaphores persemaphore set. */
  int semopm;   /* Maximum number of operations persemop call. */
  int semume;   /* Maximum number of undo entries per undo structure. */
  int semusz;   /* size in bytes of undo structure */
  int semvmx;   /* Maximum semaphore value. */
  int semaem;   /* Maximum adjust-on-exit value. */
};

struct sembuf
{
    unsigned short int sem_num;	/* semaphore number */
    short int sem_op;		/* semaphore operation */
    short int sem_flg;		/* operation flag */
};

union semun
{
     int val;                           //<= value for SETVAL
     struct semid_ds *buf;              //<= buffer for IPC_STAT & IPC_SET
     unsigned short int *array;         //<= array for GETALL & SETALL
     struct seminfo *   info;           //<= buffer for IPC_INFO
};

QKCAPI int semctl(int semid, int semnum, int cmd, ...) ;

QKCAPI int semget(key_t key, int nsems, int semflg) ;

QKCAPI int semop(int semid, struct sembuf * sops, size_t nsops) ;

__END_DECLS

#endif /** __QKC_SYS_SEM_H */

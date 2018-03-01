#ifndef __QKC_SYS_SEM_H
#define __QKC_SYS_SEM_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>
#include <sys/ipc.h>


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
  struct ipc_perm sem_perm;		/* operation permission struct */
  time_t sem_otime;			/* last semop() time */
  unsigned long int __unused1;
  time_t sem_ctime;			/* last time changed by semctl() */
  unsigned long int __unused2;
  unsigned long int sem_nsems;		/* number of semaphores in set */
  unsigned long int __unused3;
  unsigned long int __unused4;
};

/* The user should define a union like the following to use it for arguments
   for `semctl'.

   union semun
   {
     int val;				<= value for SETVAL
     struct semid_ds *buf;		<= buffer for IPC_STAT & IPC_SET
     unsigned short int *array;		<= array for GETALL & SETALL
     struct seminfo *__buf;		<= buffer for IPC_INFO
   };

   Previous versions of this file used to define this union but this is
   incorrect.  One can test the macro _SEM_SEMUN_UNDEFINED to see whether
   one must define the union or not.  */
#define _SEM_SEMUN_UNDEFINED	1

/* ipcs ctl cmds */
# define SEM_STAT 18
# define SEM_INFO 19

struct  seminfo
{
  int semmap;
  int semmni;
  int semmns;
  int semmnu;
  int semmsl;
  int semopm;
  int semume;
  int semusz;
  int semvmx;
  int semaem;
};

struct sembuf
{
  unsigned short int sem_num;	/* semaphore number */
  short int sem_op;		/* semaphore operation */
  short int sem_flg;		/* operation flag */
};


#ifdef __cplusplus
extern "C" {
#endif

//Î´ÊµÏÖ
QKCAPI int semctl (int semid, int semnum, int cmd, ...) ;

QKCAPI int semget (key_t key, int nsems, int semflg) ;

QKCAPI int semop (int semid, struct sembuf * sops, size_t nsops) ;

QKCAPI int semtimedop (int semid, struct sembuf *sops , size_t nsops , const struct timespec * timeout) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_SEM_H */

#ifndef __QKC_SYS_SHM_H
#define __QKC_SYS_SHM_H 1

#include <quark_compile.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

/* Permission flag for shmget.  */
#define SHM_R		0400		/* or S_IRUGO from <linux/stat.h> */
#define SHM_W		0200		/* or S_IWUGO from <linux/stat.h> */

/* Flags for `shmat'.  */
#define SHM_RDONLY	010000		/* attach read-only else read-write */
#define SHM_RND		020000		/* round attach address to SHMLBA */
#define SHM_REMAP	040000		/* take-over region on attach */

/* Commands for `shmctl'.  */
#define SHM_LOCK	11		/* lock segment (root only) */
#define SHM_UNLOCK	12		/* unlock segment (root only) */


/* Segment low boundary address multiple.  */
#define SHMLBA		(__getpagesize ())



/* Type to count number of attaches.  */
typedef unsigned long int shmatt_t;

/* Data structure describing a set of semaphores.  */
struct shmid_ds
{
    struct ipc_perm shm_perm;		/* operation permission struct */
    size_t shm_segsz;			/* size of segment in bytes */
    time_t shm_atime;			/* time of last shmat() */
#if __WORDSIZE == 32
    unsigned long int __unused1;
#endif
    time_t shm_dtime;			/* time of last shmdt() */
#if __WORDSIZE == 32
    unsigned long int __unused2;
#endif
    time_t shm_ctime;			/* time of last change by shmctl() */
#if __WORDSIZE == 32
    unsigned long int __unused3;
#endif
    pid_t shm_cpid;			/* pid of creator */
    pid_t shm_lpid;			/* pid of last shmop */
    shmatt_t shm_nattch;		/* number of current attaches */
    unsigned long int __unused4;
    unsigned long int __unused5;
};

/* ipcs ctl commands */
# define SHM_STAT 	13
# define SHM_INFO 	14

/* shm_mode upper byte flags */
# define SHM_DEST	01000	/* segment will be destroyed on last detach */
# define SHM_LOCKED	02000   /* segment will not be swapped */
# define SHM_HUGETLB	04000	/* segment is mapped via hugetlb */
# define SHM_NORESERVE	010000	/* don't check for reservations */

struct	shminfo
{
    unsigned long int shmmax;
    unsigned long int shmmin;
    unsigned long int shmmni;
    unsigned long int shmseg;
    unsigned long int shmall;
    unsigned long int __unused1;
    unsigned long int __unused2;
    unsigned long int __unused3;
    unsigned long int __unused4;
};

struct shm_info
{
    int used_ids;
    unsigned long int shm_tot;	/* total allocated shm */
    unsigned long int shm_rss;	/* total resident shm */
    unsigned long int shm_swp;	/* total swapped shm */
    unsigned long int swap_attempts;
    unsigned long int swap_successes;
};



#ifdef __cplusplus
extern "C" {
#endif
    
//Œ¥ µœ÷
QKCAPI int shmctl (int shmid, int __cmd, struct shmid_ds * buf) ;

QKCAPI int shmget (key_t key, size_t size, int shmflg) ;

QKCAPI void *shmat (int shmid, const void * shmaddr, int shmflg) ;

QKCAPI int shmdt (const void * shmaddr) ;

QKCAPI int __getpagesize (void) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_SHM_H */

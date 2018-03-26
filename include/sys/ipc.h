#ifndef __QKC_SYS_IPC_H
#define __QKC_SYS_IPC_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>


/* Mode bits for `msgget', `semget', and `shmget'.  */
#define IPC_CREAT	01000		/* Create key if key does not exist. */
#define IPC_EXCL	02000		/* Fail if key exists.  */
#define IPC_NOWAIT	04000		/* Return error on wait.  */

/* Control commands for `msgctl', `semctl', and `shmctl'.  */
#define IPC_RMID	0		/* Remove identifier.  */
#define IPC_SET		1		/* Set `ipc_perm' options.  */
#define IPC_STAT	2		/* Get `ipc_perm' options.  */
#define IPC_INFO	3		/* See ipcs.  */

/* Special key values.  */
#define IPC_PRIVATE	((__key_t) 0)	/* Private key.  */

struct ipc_perm
{
    key_t __key;			/* Key.  */
    uid_t uid;			/* Owner's user ID.  */
    gid_t gid;			/* Owner's group ID.  */
    uid_t cuid;			/* Creator's user ID.  */
    gid_t cgid;			/* Creator's group ID.  */
    unsigned short int mode;		/* Read/write permission.  */
    unsigned short int __pad1;
    unsigned short int __seq;		/* Sequence number.  */
    unsigned short int __pad2;
    unsigned long int __unused1;
    unsigned long int __unused2;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
    cygwin实现ftok基本遵循linux的约定，但key_t仅仅只是需要一个全局唯一的，
    因此可以用windows的Atom来替代。

    2018-03-26
    Atom不能自动删除，还要显式删除，可能会消耗操作系统的资源，换成哈希函数
*/
QKCAPI key_t ftok (const char *pathname , int proj_id) ;

#ifdef __cplusplus
}
#endif


#endif /** __QKC_SYS_IPC_H */

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
#define IPC_PRIVATE	((key_t) 0)	/* Private key.  */

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

__BEGIN_DECLS

/**
    cygwinʵ��ftok������ѭlinux��Լ������key_t����ֻ����Ҫһ��ȫ��Ψһ�ģ�
    ��˿�����windows��Atom�������

    2018-03-26
    Atom�����Զ�ɾ������Ҫ��ʽɾ�������ܻ����Ĳ���ϵͳ����Դ�����ɹ�ϣ����

    2018-05-11
    ��stat�����У�������st_ino��st_dev���ο�cygwin�Ľ���
*/
QKCAPI key_t ftok (const char *pathname , int proj_id) ;

__END_DECLS

#endif /** __QKC_SYS_IPC_H */

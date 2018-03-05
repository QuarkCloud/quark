
#ifndef __QKC_SYS_STAT_H
#define __QKC_SYS_STAT_H 1

#include <quark_compile.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct stat
{
    dev_t st_dev;		/* Device.  */
    ino_t st_ino;			/* 32bit file serial number.	*/
    nlink_t st_nlink;		/* Link count.  */
    mode_t st_mode;		/* File mode.  */

    uid_t st_uid;		/* User ID of the file's owner.	*/
    gid_t st_gid;		/* Group ID of the file's group.*/

    dev_t st_rdev;		/* Device number, if device.  */

    off64_t st_size;			/* Size of file, in bytes.  */
    time_t st_atime;			/* Time of last access.  */
    time_t st_mtime;			/* Time of last modification.  */
    time_t st_ctime;			/* Time of last status change.  */
};

/* Tell code we have these members.  */
#define	_STATBUF_ST_BLKSIZE
#define _STATBUF_ST_RDEV
/* Nanosecond resolution time values are supported.  */
#define _STATBUF_ST_NSEC

/* Encoding of the file mode.  */

#define	__S_IFMT	0170000	/* These bits determine file type.  */

/* File types.  */
#define	__S_IFDIR	0040000	/* Directory.  */
#define	__S_IFCHR	0020000	/* Character device.  */
#define	__S_IFBLK	0060000	/* Block device.  */
#define	__S_IFREG	0100000	/* Regular file.  */
#define	__S_IFIFO	0010000	/* FIFO.  */
#define	__S_IFLNK	0120000	/* Symbolic link.  */
#define	__S_IFSOCK	0140000	/* Socket.  */

/* POSIX.1b objects.  Note that these macros always evaluate to zero.  But
   they do it by enforcing the correct use of the macros.  */
#define __S_TYPEISMQ(buf)  ((buf)->st_mode - (buf)->st_mode)
#define __S_TYPEISSEM(buf) ((buf)->st_mode - (buf)->st_mode)
#define __S_TYPEISSHM(buf) ((buf)->st_mode - (buf)->st_mode)

/* Protection bits.  */

#define	__S_ISUID	04000	/* Set user ID on execution.  */
#define	__S_ISGID	02000	/* Set group ID on execution.  */
#define	__S_ISVTX	01000	/* Save swapped text after use (sticky).  */
#define	__S_IREAD	0400	/* Read by owner.  */
#define	__S_IWRITE	0200	/* Write by owner.  */
#define	__S_IEXEC	0100	/* Execute by owner.  */


#define S_IFMT	__S_IFMT
#define S_IFDIR	__S_IFDIR
#define S_IFCHR	__S_IFCHR
#define S_IFBLK	__S_IFBLK
#define S_IFREG	__S_IFREG
#define S_IFIFO	__S_IFIFO
#define S_IFLNK	__S_IFLNK

/* Test macros for file types.	*/

#define	__S_ISTYPE(mode, mask)	(((mode) & __S_IFMT) == (mask))

#define	S_ISDIR(mode)	 __S_ISTYPE((mode), __S_IFDIR)
#define	S_ISCHR(mode)	 __S_ISTYPE((mode), __S_IFCHR)
#define	S_ISBLK(mode)	 __S_ISTYPE((mode), __S_IFBLK)
#define	S_ISREG(mode)	 __S_ISTYPE((mode), __S_IFREG)
#define S_ISFIFO(mode)	 __S_ISTYPE((mode), __S_IFIFO)
#define S_ISLNK(mode)	 __S_ISTYPE((mode), __S_IFLNK)


/* Protection bits.  */
#define	S_ISUID __S_ISUID	/* Set user ID on execution.  */
#define	S_ISGID	__S_ISGID	/* Set group ID on execution.  */

#define	S_IRUSR	__S_IREAD	/* Read by owner.  */
#define	S_IWUSR	__S_IWRITE	/* Write by owner.  */
#define	S_IXUSR	__S_IEXEC	/* Execute by owner.  */
#define	S_IRWXU	(__S_IREAD|__S_IWRITE|__S_IEXEC)

# define S_IREAD	S_IRUSR
# define S_IWRITE	S_IWUSR
# define S_IEXEC	S_IXUSR

#define	S_IRGRP	(S_IRUSR >> 3)	/* Read by group.  */
#define	S_IWGRP	(S_IWUSR >> 3)	/* Write by group.  */
#define	S_IXGRP	(S_IXUSR >> 3)	/* Execute by group.  */
/* Read, write, and execute by group.  */
#define	S_IRWXG	(S_IRWXU >> 3)

#define	S_IROTH	(S_IRGRP >> 3)	/* Read by others.  */
#define	S_IWOTH	(S_IWGRP >> 3)	/* Write by others.  */
#define	S_IXOTH	(S_IXGRP >> 3)	/* Execute by others.  */
/* Read, write, and execute by others.  */
#define	S_IRWXO	(S_IRWXG >> 3)


QKCAPI int stat (const char * file, struct stat * buf) ;
QKCAPI int lstat (const char * path, struct stat * statbuf) ;
QKCAPI int fstat (int fd , struct stat *buf) ;
QKCAPI int chmod (const char *file, mode_t mode);

QKCAPI int lchmod (const char *file, mode_t mode) ;
QKCAPI int fchmod (int fd , mode_t mode) ;

QKCAPI int mkdir (const char *path, mode_t mode);


#ifdef	__cplusplus
}
#endif

#endif /**__QKC_SYS_STAT_H*/



#ifndef __QKC_FCNTL_H
#define __QKC_FCNTL_H 1

#include <quark_compile.h>

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define O_ACCMODE	   0003
#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_CREAT		   0100	/* not fcntl */
#define O_EXCL		   0200	/* not fcntl */
#define O_NOCTTY	   0400	/* not fcntl */
#define O_TRUNC		  01000	/* not fcntl */
#define O_APPEND	  02000
#define O_NONBLOCK	  04000
#define O_NDELAY	O_NONBLOCK
#define O_SYNC		 010000
#define O_FSYNC		 O_SYNC
#define O_ASYNC		 020000


# define O_DIRECT	 040000	/* Direct disk access.	*/
# define O_DIRECTORY	0200000	/* Must be a directory.	 */
# define O_NOFOLLOW	0400000	/* Do not follow links.	 */
# define O_NOATIME     01000000 /* Do not set atime.  */

# if __WORDSIZE == 64
#  define O_LARGEFILE	0
# else
#  define O_LARGEFILE	0100000
# endif

#define F_DUPFD		0	/* Duplicate file descriptor.  */
#define F_GETFD		1	/* Get file descriptor flags.  */
#define F_SETFD		2	/* Set file descriptor flags.  */
#define F_GETFL		3	/* Get file status flags.  */
#define F_SETFL		4	/* Set file status flags.  */

# define F_GETLK	5	/* Get record locking info.  */
# define F_SETLK	6	/* Set record locking info (non-blocking).  */
# define F_SETLKW	7	/* Set record locking info (blocking).	*/

# define F_SETOWN	8	/* Get owner of socket (receiver of SIGIO).  */
# define F_GETOWN	9	/* Set owner of socket (receiver of SIGIO).  */

# define F_SETSIG	10	/* Set number of signal to be sent.  */
# define F_GETSIG	11	/* Get number of signal to be sent.  */

# define F_SETLEASE	1024	/* Set a lease.	 */
# define F_GETLEASE	1025	/* Enquire what lease is active.  */
# define F_NOTIFY	1026	/* Request notfications on a directory.	 */

/* For F_[GET|SET]FD.  */
#define FD_CLOEXEC	1	/* actually anything with low bit set goes */

/* For posix fcntl() and `l_type' field of a `struct flock' for lockf().  */
#define F_RDLCK		0	/* Read lock.  */
#define F_WRLCK		1	/* Write lock.	*/
#define F_UNLCK		2	/* Remove lock.	 */

/* For old implementation of bsd flock().  */
#define F_EXLCK		4	/* or 3 */
#define F_SHLCK		8	/* or 4 */

struct flock
{
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    off_t l_start;	/* Offset where the lock begins.  */
    off_t l_len;	/* Size of the locked area; zero means until EOF.  */
    pid_t l_pid;	/* Process holding the lock.  */
};

struct flock64
{
    short int l_type;	/* Type of lock: F_RDLCK, F_WRLCK, or F_UNLCK.	*/
    short int l_whence;	/* Where `l_start' is relative to (like `lseek').  */
    off64_t l_start;	/* Offset where the lock begins.  */
    off64_t l_len;	/* Size of the locked area; zero means until EOF.  */
    pid_t l_pid;	/* Process holding the lock.  */
};


# ifndef R_OK			/* Verbatim from <unistd.h>.  Ugh.  */
#  define R_OK	4		/* Test for read permission.  */
#  define W_OK	2		/* Test for write permission.  */
#  define X_OK	1		/* Test for execute permission.  */
#  define F_OK	0		/* Test for existence.  */
# endif

# define SEEK_SET	0	/* Seek from beginning of file.  */
# define SEEK_CUR	1	/* Seek from current position.  */
# define SEEK_END	2	/* Seek from end of file.  */


QKCAPI int fcntl (int fd, int cmd , ...);


# define F_ULOCK 0	/* Unlock a previously locked region.  */
# define F_LOCK  1	/* Lock a region for exclusive use.  */
# define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
# define F_TEST  3	/* Test a region for other processes locks.  */


QKCAPI int creat(const char * file , mode_t mode) ;

QKCAPI int open(const char * file , int flag,mode_t mode) ;

#ifdef __cplusplus
}
#endif


#endif  /** __QKC_FCNTL_H */


#ifndef __QKC_UNISTD_H
#define __QKC_UNISTD_H 1

#include <quark_compile.h>
#include <sys/types.h>

/* Values for the second argument to access.
  These may be OR'd together. */
#define	R_OK	4		/* Test for read permission. */
#define	W_OK	2		/* Test for write permission. */
#define	X_OK	1		/* Test for execute permission. */
#define	F_OK	0		/* Test for existence. */

/* Values for the WHENCE argument to lseek. */
#ifndef SEEK_SET
#define SEEK_SET	0	/* Seek from beginning of file. */
#endif

#ifndef SEEK_CUR
#define SEEK_CUR	1	/* Seek from current position. */
#endif

#ifndef SEEK_END
#define SEEK_END	2	/* Seek from end of file. */
#endif

# define F_ULOCK 0	/* Unlock a previously locked region.  */
# define F_LOCK  1	/* Lock a region for exclusive use.  */
# define F_TLOCK 2	/* Test and lock a region for exclusive use.  */
# define F_TEST  3	/* Test a region for other processes locks.  */


#ifdef __cplusplus
extern "C" {
#endif

/**
  被注释掉的函数，都是在vs中，已经被实现的
*/
QKCAPI off_t lseek(int fd , off_t offset , int whence) ;
QKCAPI off64_t lseek64(int fd , off64_t offset , int whence) ;

QKCAPI int close(int fd) ;
QKCAPI ssize_t read(int fd , void * buf , size_t nbytes) ;
QKCAPI ssize_t write(int fd , const void * buf , size_t n) ;

QKCAPI unsigned int sleep(unsigned int seconds) ;
QKCAPI int usleep(useconds_t useconds) ;
QKCAPI int pause();

QKCAPI int chown(const char * file , uid_t owner , gid_t group) ;
QKCAPI int chdir(const char * path) ;
QKCAPI char *getcwd(char * buf , size_t size) ;
QKCAPI int dup(int fd) ;
QKCAPI int dup2(int fd , int fd2) ;

//太多了，需要的时候再实现
QKCAPI long int sysconf(int name) ;

QKCAPI pid_t getpid(void) ;

QKCAPI pid_t fork(void) ;

QKCAPI int rmdir(const char * path) ;

QKCAPI int gethostname(char * name , size_t len) ;
QKCAPI int sethostname(const char * name , size_t len) ;

QKCAPI int daemon(int nochdir , int noclose) ;

QKCAPI int fsync(int fd) ;
QKCAPI void sync(void) ;

QKCAPI int getpagesize(void) ;
QKCAPI int truncate(const char * file , off_t length) ;
QKCAPI int truncate64(const char * file , off64_t length) ;

QKCAPI int ftruncate(int fd , off_t length) {return ::chsize(fd , length) ;}
QKCAPI int ftruncate64(int fd , off64_t length) {return ::_chsize_s(fd , length) ;}

QKCAPI int brk(void * addr) ;

QKCAPI void *sbrk(intptr_t delta) ;

QKCAPI long int syscall(long int sysno , ...) ;

QKCAPI int lockf(int fd , int cmd , off_t len) ;
QKCAPI int lockf64(int fd , int cmd , off64_t len) ;

QKCAPI char *crypt(const char *key , const char *salt) ;

QKCAPI void encrypt(char *block , int edflag) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_UNISTD_H */

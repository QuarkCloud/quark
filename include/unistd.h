
#ifndef __QKC_UNISTD_H
#define __QKC_UNISTD_H 1

#include <quark_compile.h>
#include <sys/types.h>
#include <bits/confname.h>

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


#define STDIN_FILENO    0       /* Standard input.  */
#define STDOUT_FILENO   1       /* Standard output.  */
#define STDERR_FILENO   2       /* Standard error output.  */


__BEGIN_DECLS

QUARK_LINKAGE int access (const char * name, int type) ;

QKCAPI off_t lseek(int fd , off_t offset , int whence) ;
QKCAPI off64_t lseek64(int fd , off64_t offset , int whence) ;

QKCAPI int close(int fd) ;
QKCAPI ssize_t read(int fd , void * buf , size_t nbytes) ;
QKCAPI ssize_t write(int fd , const void * buf , size_t nbytes) ;

QKCAPI unsigned int sleep(unsigned int seconds) ;
QKCAPI int usleep(useconds_t useconds) ;
QKCAPI int pause();

QKCAPI int pipe (int pipedes[2]) ;

QKCAPI int chown(const char * file , uid_t owner , gid_t group) ;
QKCAPI int chdir(const char * path) ;
QKCAPI char *getcwd(char * buf , size_t size) ;
QKCAPI int dup(int fd) ;
QKCAPI int dup2(int fd1 , int fd2) ;
QUARK_LINKAGE int _exit(int code) ;

QUARK_LINKAGE int execl(const char * file_name, const char * args, ...);
QUARK_LINKAGE int execle(const char * file_name, const char * args, ...);
QUARK_LINKAGE int execlp(const char * file_name, const char * args, ...);
QUARK_LINKAGE int execv(const char * file_name, const char * const * args);
QUARK_LINKAGE int execve(const char * file_name, const char * const * args, const char * const * env);
QUARK_LINKAGE int execvp(const char * file_name, const char * const * args);

//太多了，需要的时候再实现
QKCAPI long int sysconf(int name) ;

QKCAPI pid_t getpid() ;

QKCAPI uid_t getuid () ;

QKCAPI uid_t geteuid () ;

QKCAPI gid_t getgid () ;

QKCAPI gid_t getegid () ;

QKCAPI int setuid (uid_t uid) ;

QKCAPI int setreuid (uid_t ruid, uid_t euid) ;

QKCAPI int seteuid (uid_t uid) ;

QKCAPI int setgid (gid_t gid) ;

QKCAPI int setegid (gid_t gid) ;

QKCAPI pid_t setsid (void) ;


QKCAPI ssize_t pread (int fd, void *buf, size_t nbytes, off_t offset) ;

QKCAPI ssize_t pwrite (int fd, const void *buf, size_t n,off_t offset);

QKCAPI long int pathconf (const char *path, int name);

QKCAPI long int fpathconf (int fd, int name) ;

QKCAPI int symlink (const char * from, const char * to);

QKCAPI ssize_t readlink (const char *path, char *buf, size_t len);

QUARK_LINKAGE int unlink (const char * name) ;

QKCAPI int link (const char * from, const char * to);

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

QKCAPI int ftruncate(int fd , off_t length) ;
QKCAPI int ftruncate64(int fd , off64_t length) ;

QKCAPI int brk(void * addr) ;

QKCAPI void *sbrk(intptr_t delta) ;

QKCAPI long int syscall(long int sysno , ...) ;

QKCAPI int lockf(int fd , int cmd , off_t len) ;
QKCAPI int lockf64(int fd , int cmd , off64_t len) ;

QKCAPI char *crypt(const char *key , const char *salt) ;

QKCAPI void encrypt(char *block , int edflag) ;

QKCAPI int fchown (int fd, uid_t owner, gid_t group) ;

QKCAPI char *ttyname (int fd) ;

QKCAPI int ttyname_r (int fd, char *buf, size_t buflen) ;

QKCAPI int isatty (int fd) ;

__END_DECLS

#endif /** __QKC_UNISTD_H */

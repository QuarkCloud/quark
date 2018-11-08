
#ifndef __QKC_WINTF_WCRT_H
#define __QKC_WINTF_WCRT_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <windows.h>

__BEGIN_DECLS

typedef struct {
        intptr_t osfhnd;    /* underlying OS file HANDLE */
        char osfile;        /* attributes of file (e.g., open in text mode?) */
        char pipech;        /* one char buffer for handles opened on pipes */
        int lockinitflag;
        CRITICAL_SECTION lock;
}   ioinfo;

QUARK_LINKAGE ioinfo * __pioinfo[];

#define IOINFO_L2E          5
#define IOINFO_ARRAY_ELTS   (1 << IOINFO_L2E)
#define IOINFO_ARRAYS       64
#define _pioinfo(i) ( __pioinfo[(i) >> IOINFO_L2E] + ((i) & (IOINFO_ARRAY_ELTS - 1)) )

//io
QUARK_LINKAGE intptr_t _get_osfhandle(int fd);
QUARK_LINKAGE int _open_osfhandle(intptr_t handle, int flags);
QUARK_LINKAGE int _unlink(const char * filename);

QUARK_LINKAGE int _chmod(const char * filename , int mode);
QUARK_LINKAGE int _pipe(int * fds, unsigned int size, int mode);

typedef unsigned int _dev_t;
typedef unsigned short _ino_t;
typedef __int64 __time64_t ;

struct _stat64 {
    _dev_t     st_dev;
    _ino_t     st_ino;
    unsigned short st_mode;
    short      st_nlink;
    short      st_uid;
    short      st_gid;
    _dev_t     st_rdev;
    __int64    st_size;
    __time64_t st_atime;
    __time64_t st_mtime;
    __time64_t st_ctime;
};
QUARK_LINKAGE int _fstat64(int fd , struct _stat64 * stat_buf);
QUARK_LINKAGE int _stat64(const char * name, struct _stat64 * stat_buf);
QUARK_LINKAGE int _mkdir(const char * path);

typedef int errno_t;

#if _MSC_VER >= 1912
QKCAPI void _dosmaperr(unsigned long);
QUARK_LINKAGE char***    __p__environ(void);
#define _environ  (*__p__environ()) 
#else
QUARK_LINKAGE void _dosmaperr(unsigned long);
QUARK_LINKAGE extern char ** _environ;
#endif

QUARK_LINKAGE unsigned long * __doserrno(void);
QUARK_LINKAGE errno_t _set_doserrno(unsigned long value);
QUARK_LINKAGE  errno_t _get_doserrno(unsigned long * value);
#define _doserrno (*__doserrno())


//TIMEÏà¹Ø
QUARK_LINKAGE void _tzset(void);
QUARK_LINKAGE double _difftime64(__time64_t time1 , __time64_t time2);
QUARK_LINKAGE char * _ctime64(const __time64_t * time);
QUARK_LINKAGE errno_t _ctime64_s(char *buf, size_t bytes , const __time64_t * time);

QUARK_LINKAGE struct tm * _gmtime64(const __time64_t * time );
QUARK_LINKAGE errno_t _gmtime64_s(struct tm * tm, const __time64_t *time);

QUARK_LINKAGE struct tm * _localtime64(const __time64_t * time);
QUARK_LINKAGE errno_t _localtime64_s(struct tm *  tm , const __time64_t * time);

QUARK_LINKAGE __time64_t _mktime64(struct tm * tm);
QUARK_LINKAGE __time64_t _mkgmtime64(struct tm * tm);
QUARK_LINKAGE __time64_t _time64(__time64_t * time);

//fcntl
QUARK_LINKAGE int _creat(const char * file , mode_t mode) ;
QUARK_LINKAGE int _open(const char * file , int flag , mode_t mode) ;


//stdio
QUARK_LINKAGE char *tmpnam_s (char *s);
QUARK_LINKAGE int _fcloseall (void);
QUARK_LINKAGE FILE *_fdopen (int fd , const char *modes);
QUARK_LINKAGE int _snprintf (char * s , size_t maxlen , const char * format , ...);
QUARK_LINKAGE int _vsnprintf (char * s , size_t maxlen , const char * format , va_list arg);
QUARK_LINKAGE int _getw (FILE *stream);
QUARK_LINKAGE int _putw (int w , FILE *stream);

QUARK_LINKAGE int _fseeki64 (FILE *stream , off_t off , int whence);
QUARK_LINKAGE off_t _ftelli64 (FILE *stream);
QUARK_LINKAGE int _fileno (FILE *stream);

QUARK_LINKAGE void _lock_file (FILE *stream);
QUARK_LINKAGE void _unlock_file (FILE *stream);

#if _MSC_VER >= 1912

typedef void * _locale_t;

QUARK_LINKAGE  int __stdio_common_vsprintf(unsigned __int64 options, char* buffer, size_t buffer_count,
    char const* format, _locale_t locale,   va_list arglist );

QUARK_LINKAGE  int __stdio_common_vsprintf_s(unsigned __int64 options, char* buffer, size_t buffer_count,
    char const* format, _locale_t locale, va_list arglist);

QUARK_LINKAGE  int __stdio_common_vsnprintf_s(unsigned __int64 options, char* buffer, size_t buffer_count,
    char const* format, _locale_t locale, va_list arglist);

QUARK_LINKAGE  int __stdio_common_vsprintf_p(unsigned __int64 options, char* buffer, size_t buffer_count,
    char const* format, _locale_t locale, va_list arglist);

QUARK_LINKAGE int __stdio_common_vfprintf(unsigned __int64 options, FILE* stream ,char const* format ,
    _locale_t locale,va_list arglist);

QUARK_LINKAGE int __stdio_common_vfprintf_s(unsigned __int64 options, FILE* stream, char const* format,
    _locale_t locale, va_list arglist);

QUARK_LINKAGE int __stdio_common_vfprintf_p(unsigned __int64 options, FILE* stream, char const* format,
    _locale_t locale, va_list arglist);

QUARK_LINKAGE int __stdio_common_vfscanf(unsigned __int64 options, FILE* stream,char const* format,
    _locale_t locale, va_list arglist);

QUARK_LINKAGE int __stdio_common_vsscanf(unsigned __int64 options,char const* buffer,size_t buffer_count,
    char const* format,_locale_t locale,va_list arglist);


#endif

//string
QUARK_LINKAGE char *  strtok_s(char * str , const char * delim,  char ** context);

//unistd
QUARK_LINKAGE off_t _lseek(int fd , off_t offset , int whence) ;
QUARK_LINKAGE off64_t _lseeki64(int fd , off64_t offset , int whence) ;
QUARK_LINKAGE int _close(int fd) ;
QUARK_LINKAGE ssize_t _read(int fd , void * buf , size_t nbytes) ;
QUARK_LINKAGE ssize_t _write(int fd , const void * buf , size_t n) ;
QUARK_LINKAGE void _sleep(unsigned int seconds) ;
QUARK_LINKAGE int _chdir(const char * path) ;
QUARK_LINKAGE int _chsize(int fd ,long size);
QUARK_LINKAGE int _chsize_s(int fd ,__int64 size);
QUARK_LINKAGE char *_getcwd(char * buf , size_t size) ;
QUARK_LINKAGE int _dup(int fd) ;
QUARK_LINKAGE int _dup2(int fd , int fd2) ;
QUARK_LINKAGE pid_t _getpid(void) ;
QUARK_LINKAGE int _rmdir(const char * path) ;
QUARK_LINKAGE int _locking(int fd , int cmd , off_t len) ;

//utime
QUARK_LINKAGE int _utime64(const char * file , struct utimbuf * file_times) ;

//wait
QUARK_LINKAGE intptr_t _cwait(int * status , intptr_t process , int action);

__END_DECLS

#endif /** __QKC_WINTF_WCRT_H */

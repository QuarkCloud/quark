
#ifndef __QKC_WINTF_WOBJ_H
#define __QKC_WINTF_WOBJ_H 1

#include <quark_compile.h>

#ifdef	__cplusplus
extern "C" {
#endif

QUARK_LINKAGE intptr_t _get_osfhandle(int fd);
QUARK_LINKAGE int _open_osfhandle(intptr_t handle, int flags);
QUARK_LINKAGE int _open(const char * filename , int oflag , int mode = 0);
QUARK_LINKAGE int _unlink(const char * filename);

QUARK_LINKAGE int _chmod(const char * filename , int mode);

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

QUARK_LINKAGE void _dosmaperr(unsigned long);
QUARK_LINKAGE unsigned long * __doserrno(void);
QUARK_LINKAGE errno_t _set_doserrno(unsigned long value);
QUARK_LINKAGE  errno_t _get_doserrno(unsigned long * value);


#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WOBJ_H */

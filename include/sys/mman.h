#ifndef __QKC_SYS_MMAN_H
#define __QKC_SYS_MMAN_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>


#define PROT_READ	0x1		/* Page can be read.  */
#define PROT_WRITE	0x2		/* Page can be written.  */
#define PROT_EXEC	0x4		/* Page can be executed.  */
#define PROT_NONE	0x0		/* Page can not be accessed.  */
#define PROT_GROWSDOWN	0x01000000	/* Extend change to start of growsdown vma (mprotect only).  */
#define PROT_GROWSUP	0x02000000	/* Extend change to start of growsup vma (mprotect only).  */

/* Sharing types (must choose one and only one of these).  */
#define MAP_SHARED	0x01		/* Share changes.  */
#define MAP_PRIVATE	0x02		/* Changes are private.  */
#define MAP_TYPE	0x0f		/* Mask for type of mapping.  */


/* Other flags.  */
#define MAP_FIXED	0x10		/* Interpret addr exactly.  */
# define MAP_FILE	0
# define MAP_ANONYMOUS	0x20		/* Don't use a file.  */
# define MAP_ANON	MAP_ANONYMOUS
# define MAP_32BIT	0x40		/* Only give out 32-bit addresses.  */

/* These are Linux-specific.  */
# define MAP_GROWSDOWN	0x00100		/* Stack-like segment.  */
# define MAP_DENYWRITE	0x00800		/* ETXTBSY */
# define MAP_EXECUTABLE	0x01000		/* Mark it as an executable.  */
# define MAP_LOCKED	0x02000		/* Lock the mapping.  */
# define MAP_NORESERVE	0x04000		/* Don't check for reservations.  */
# define MAP_POPULATE	0x08000		/* Populate (prefault) pagetables.  */
# define MAP_NONBLOCK	0x10000		/* Do not block on IO.  */
# define MAP_STACK      0x20000         /* Allocation is for a stack.  */


/* Flags to `msync'.  */
#define MS_ASYNC	1		/* Sync memory asynchronously.  */
#define MS_SYNC		4		/* Synchronous memory sync.  */
#define MS_INVALIDATE	2		/* Invalidate the caches.  */

/* Flags for `mlockall'.  */
#define MCL_CURRENT	1		/* Lock all currently mapped pages.  */
#define MCL_FUTURE	2		/* Lock all additions to addressspace.  */

#define MAP_FAILED	((void *) -1)


/* Advice to `madvise'.  */
# define MADV_NORMAL     0      /* No further special treatment.  */
# define MADV_RANDOM     1      /* Expect random page references.  */
# define MADV_SEQUENTIAL 2      /* Expect sequential page references.  */
# define MADV_WILLNEED   3      /* Will need these pages.  */
# define MADV_DONTNEED   4      /* Don't need these pages.  */
# define MADV_REMOVE     9      /* Remove these pages and resources.  */
# define MADV_DONTFORK   10     /* Do not inherit across fork.  */
# define MADV_DOFORK     11     /* Do inherit across fork.  */

/* The POSIX people had to invent similar names for the same things.  */
# define POSIX_MADV_NORMAL      0 /* No further special treatment.  */
# define POSIX_MADV_RANDOM      1 /* Expect random page references.  */
# define POSIX_MADV_SEQUENTIAL  2 /* Expect sequential page references.  */
# define POSIX_MADV_WILLNEED    3 /* Will need these pages.  */
# define POSIX_MADV_DONTNEED    4 /* Don't need these pages.  */


#ifdef __cplusplus
extern "C" {
#endif


/**
    2018-06-15
    mmap的fd可以是正常的文件句柄，也可以shm_open的句柄，用于共享内存。
    这两个特性可以用windows的文件映射来处理。
    在处理jemalloc的时候，munmap居然可以释放一个整块的内存中，某个特定
    连续区间的内存块。这个特性在windows没有对应的函数。
*/
QKCAPI void *mmap (void * addr, size_t len, int prot, int flags, int fd , off_t offset) ;

QKCAPI int munmap (void *addr, size_t len) ;

QKCAPI int mprotect (void *addr, size_t len, int prot) ;

QKCAPI int msync (void *addr, size_t len, int flags);

QKCAPI int madvise (void *addr, size_t len, int advice);

QKCAPI int posix_madvise (void *addr, size_t len, int advice);

QKCAPI int mlock (const void *addr, size_t len) ;

QKCAPI int munlock (const void *addr, size_t len) ;

QKCAPI int mlockall (int flags) ;

QKCAPI int munlockall (void) ;

QKCAPI int shm_open (const char *name , int oflag , mode_t mode);

QKCAPI int shm_unlink (const char *name);

#ifdef __cplusplus
}
#endif


#endif /** __QKC_SYS_MMAN_H */

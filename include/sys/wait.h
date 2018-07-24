#ifndef __QKC_SYS_WAIT_H
#define __QKC_SYS_WAIT_H 1

#include <quark_compile.h>
#include <sys/types.h>
#include <bits/waitflags.h>

#ifdef __cplusplus
extern "C" {
#endif

#  define __WAIT_INT(status)	(status)
#  define __WAIT_STATUS		int *
#  define __WAIT_STATUS_DEFN	int *

#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)
#define	__WTERMSIG(status)	((status) & 0x7f)
#define	__WSTOPSIG(status)	__WEXITSTATUS(status)
#define	__WIFEXITED(status)	(__WTERMSIG(status) == 0)
#define __WIFSIGNALED(status) (((signed char) (((status) & 0x7f) + 1) >> 1) > 0)
#define	__WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)
#define	__WCOREDUMP(status)	((status) & __WCOREFLAG)

#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__W_STOPCODE(sig)	((sig) << 8 | 0x7f)
#define __W_CONTINUED		0xffff
#define	__WCOREFLAG		0x80

# define WEXITSTATUS(status)	__WEXITSTATUS(__WAIT_INT(status))
# define WTERMSIG(status)	__WTERMSIG(__WAIT_INT(status))
# define WSTOPSIG(status)	__WSTOPSIG(__WAIT_INT(status))
# define WIFEXITED(status)	__WIFEXITED(__WAIT_INT(status))
# define WIFSIGNALED(status)	__WIFSIGNALED(__WAIT_INT(status))
# define WIFSTOPPED(status)	__WIFSTOPPED(__WAIT_INT(status))

QKCAPI pid_t wait (__WAIT_STATUS stat_loc);
QKCAPI pid_t waitpid (pid_t pid, int * stat_loc, int options);

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_WAIT_H */


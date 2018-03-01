#ifndef __QKC_SYS_WAIT_H
#define __QKC_SYS_WAIT_H 1

#include <quark_compile.h>
#include <sys/types.h>
#include <process.h>

#ifdef __cplusplus
extern "C" {
#endif

QKCAPI pid_t wait (int * stat_loc) ;

QKCAPI pid_t waitpid (pid_t pid, int * stat_loc, int options);

#ifdef __cplusplus
}
#endif

#endif /**__QKC_SYS_WAIT_H */

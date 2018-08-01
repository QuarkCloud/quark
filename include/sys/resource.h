#ifndef __QKC_SYS_RESOURCE_H
#define __QKC_SYS_RESOURCE_H 1

#include <quark_compile.h>
#include <sys/time.h>

__BEGIN_DECLS

#define RUSAGE_SELF     0
#define RUSAGE_CHILDREN (-1)
#define RUSAGE_BOTH     (-2)            /* sys_wait4() uses this */
#define RUSAGE_THREAD   1               /* only the calling thread */


struct rusage {
    struct timeval ru_utime; /* user CPU time used */
    struct timeval ru_stime; /* system CPU time used */
    long   ru_maxrss;        /* maximum resident set size */
    long   ru_ixrss;         /* integral shared memory size */
    long   ru_idrss;         /* integral unshared data size */
    long   ru_isrss;         /* integral unshared stack size */
    long   ru_minflt;        /* page reclaims (soft page faults) */
    long   ru_majflt;        /* page faults (hard page faults) */
    long   ru_nswap;         /* swaps */
    long   ru_inblock;       /* block input operations */
    long   ru_oublock;       /* block output operations */
    long   ru_msgsnd;        /* IPC messages sent */
    long   ru_msgrcv;        /* IPC messages received */
    long   ru_nsignals;      /* signals received */
    long   ru_nvcsw;         /* voluntary context switches */
    long   ru_nivcsw;        /* involuntary context switches */
};

QKCAPI int getrusage(int who, struct rusage *usage);


__END_DECLS


#endif /** __QKC_SYS_RESOURCE_H */

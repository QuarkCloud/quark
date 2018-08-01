
#ifndef __QKC_SYS_TIME_H
#define __QKC_SYS_TIME_H 1

#include <quark_compile.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>

//#define CLOCKS_PER_SEC  1000000l

/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME		0
/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC		1
/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID	2
/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID	3

/* Flag to indicate time is absolute.  */
#define TIMER_ABSTIME		1

__BEGIN_DECLS

QKCAPI int gettimeofday (struct timeval * tv , void * tz) ;

QKCAPI int utimes (const char * file, const struct timeval tvp[2]) ;


__END_DECLS

#endif  /** __QKC_SYS_TIME_H */

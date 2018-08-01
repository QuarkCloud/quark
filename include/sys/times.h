#ifndef __QKC_SYS_TIMES_H
#define __QKC_SYS_TIMES_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>
#include <time.h>

/* Structure describing CPU time used by a process and its children.  */
struct tms
{
    clock_t tms_utime;		/* User CPU time.  */
    clock_t tms_stime;		/* System CPU time.  */

    clock_t tms_cutime;		/* User CPU time of dead children.  */
    clock_t tms_cstime;		/* System CPU time of dead children.  */
};

__BEGIN_DECLS

//Œ¥ µœ÷
QKCAPI clock_t times (struct tms * buffer) ;

__END_DECLS

#endif /** __QKC_SYS_TIMES_H */

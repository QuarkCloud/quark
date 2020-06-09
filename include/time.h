
#ifndef __QKC_TIME_H
#define __QKC_TIME_H 1

#include <quark_compile.h>
#include <sys/types.h>

__BEGIN_DECLS

#  define CLOCKS_PER_SEC  1000000l

/* Identifier for system-wide realtime clock.  */
#   define CLOCK_REALTIME		0
/* Monotonic system-wide clock.  */
#   define CLOCK_MONOTONIC		1
/* High-resolution timer from the CPU.  */
#   define CLOCK_PROCESS_CPUTIME_ID	2
/* Thread-specific CPU-time clock.  */
#   define CLOCK_THREAD_CPUTIME_ID	3
/* Flag to indicate time is absolute.  */
#   define TIMER_ABSTIME		1

struct timeval
{
    time_t tv_sec;		/* Seconds.  */
    suseconds_t tv_usec;	/* Microseconds.  */
};

struct timespec
{
    time_t tv_sec;		/* Seconds.  */
    long int tv_nsec;		/* Nanoseconds.  */
};

struct tm
{
    int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
    int tm_min;			/* Minutes.	[0-59] */
    int tm_hour;			/* Hours.	[0-23] */
    int tm_mday;			/* Day.		[1-31] */
    int tm_mon;			/* Month.	[0-11] */
    int tm_year;			/* Year	- 1900.  */
    int tm_wday;			/* Day of week.	[0-6] */
    int tm_yday;			/* Days in year.[0-365]	*/
    int tm_isdst;			/* DST.		[-1/0/1]*/
};

typedef void * timer_t ;

QUARK_LINKAGE clock_t clock (void) ;
QKCAPI time_t time (time_t *ts) ;
QKCAPI double difftime (time_t t1, time_t t0) ;
QKCAPI time_t mktime (struct tm * tp) ;

QUARK_LINKAGE size_t strftime (char * s, size_t maxsize, const char * format, const struct tm * tp) ;

QKCAPI struct tm *gmtime (const time_t *ts) ;
QKCAPI struct tm *localtime (const time_t *ts) ;
QKCAPI struct tm *gmtime_r (const time_t *ts, struct tm * tp) ;
QKCAPI struct tm *localtime_r (const time_t * ts, struct tm * tp) ;

/* Defined in localtime.c.  */
//QUARK_LINKAGE extern char *__tzname[2];	/* Current timezone names.  */
QUARK_LINKAGE extern int __daylight;		/* If daylight-saving time is ever in use.  */
//QUARK_LINKAGE extern long int __timezone;	/* Seconds west of UTC.  */

/* Same as above.  */
QUARK_LINKAGE extern char *tzname[2];

/* Set time conversion information from the TZ environment variable.
   If TZ is not defined, a locale-dependent default is used.  */
QKCAPI void tzset (void) ;

QKCAPI int nanosleep (const struct timespec * ts ,    struct timespec * remaining);

QKCAPI int clock_getres (clockid_t clock_id, struct timespec * res) ;
QKCAPI int clock_gettime (clockid_t clock_id, struct timespec * tp) ;
QKCAPI int clock_settime (clockid_t clock_id, const struct timespec * tp) ;
QKCAPI int clock_nanosleep (clockid_t clock_id, int flags,   const struct timespec *req,  struct timespec *rem);
QKCAPI int clock_getcpuclockid (pid_t pid, clockid_t *clock_id) ;

QKCAPI int timer_create (clockid_t clock_id, struct sigevent * evp, timer_t *timerid) ;
QKCAPI int timer_delete (timer_t timerid) ;
QKCAPI int timer_settime (timer_t timerid, int flags,  const struct itimerspec *value, struct itimerspec * ovalue) ;
QKCAPI int timer_gettime (timer_t timerid, struct itimerspec * value);

__END_DECLS

#endif  /** __QKC_TIME_H */

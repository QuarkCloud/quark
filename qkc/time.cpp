
#include <time.h>

clock_t clock (void)
{
    return 0 ;
}

time_t time (time_t *ts)
{
    return 0 ;
}

double difftime (time_t t1, time_t t0)
{
    return 0 ;
}

time_t mktime (struct tm * tp)
{
    return 0 ;
}

struct tm *gmtime (const time_t *ts)
{
    return NULL ;
}

struct tm *localtime (const time_t *ts)
{
    return NULL ;
}

struct tm *gmtime_r (const time_t *ts, struct tm * tp)
{
    return NULL ;
}

struct tm *localtime_r (const time_t * ts, struct tm * tp)
{
    return NULL ;
}

void tzset (void) {}

int nanosleep (const struct timespec * ts ,    struct timespec * remaining)
{
    return 0 ;
}

int clock_getres (clockid_t clock_id, struct timespec * res)
{
    return 0 ;
}

int clock_gettime (clockid_t clock_id, struct timespec * tp) 
{
    return 0 ;
}

int clock_settime (clockid_t clock_id, const struct timespec * tp) 
{
    return 0 ;
}

int clock_nanosleep (clockid_t clock_id, int flags,   const struct timespec *req,  struct timespec *rem)
{
    return 0 ;
}

int clock_getcpuclockid (pid_t pid, clockid_t *clock_id) 
{
    return 0 ;
}

int timer_create (clockid_t clock_id, struct sigevent * evp, timer_t *timerid) 
{
    return 0 ;
}

int timer_delete (timer_t timerid) 
{
    return 0 ;
}

int timer_settime (timer_t timerid, int flags,  const struct itimerspec *value, struct itimerspec * ovalue) 
{
    return 0 ;
}

int timer_gettime (timer_t timerid, struct itimerspec * value)
{
    return 0 ;
}


#include <time.h>
#include <wintf/wcrt.h>

/**

//TIMEœ‡πÿ
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
*/

time_t time (time_t *ts)
{
    __time64_t t64 = 0 ;
    ::_time64(&t64) ;

    if(ts != NULL)
        *ts = t64 ;
    return t64 ;
}

double difftime (time_t t1, time_t t0)
{
    return _difftime64(t1 , t0) ;
}

time_t mktime (struct tm * tp)
{
    return _mktime64(tp) ;
}

struct tm *gmtime (const time_t *ts)
{
    return _gmtime64(ts) ;
}

struct tm *localtime (const time_t *ts)
{
    return _localtime64(ts) ;
}

struct tm *gmtime_r (const time_t *ts, struct tm * tp)
{
    if(_gmtime64_s(tp , ts) == 0)
        return tp ;
    else
        return NULL ;
}

struct tm *localtime_r (const time_t * ts, struct tm * tp)
{
    if(_localtime64_s(tp , ts) == 0)
        return tp ;
    else
        return NULL ;

}

void tzset (void) 
{
    _tzset() ;
}

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

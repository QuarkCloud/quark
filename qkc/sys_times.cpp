
#include <sys/times.h>
#include <windows.h>
#include <errno.h>

#define NSPERSEC 10000000LL
static clock_t __stdcall __to_clock_t (PFILETIME src)
{
    uint64_t total = src->dwHighDateTime ;
    total = (total << 32) + src->dwLowDateTime ;
  total /= NSPERSEC / CLOCKS_PER_SEC;
  return (clock_t)total;
}

clock_t times (struct tms * buffer)
{
    clock_t tc = (clock_t) -1;

    FILETIME idle_time , kernel_time , user_time ;
    if(::GetSystemTimes(&idle_time , &kernel_time , &user_time) == FALSE)
        return tc ;

    buffer->tms_stime = __to_clock_t (&kernel_time);
    buffer->tms_utime = __to_clock_t (&user_time);
    buffer->tms_cstime = 0 ;
    buffer->tms_cutime = 0 ;

    return tc;
}



#include <sys/times.h>
#include <windows.h>
#include <errno.h>

static inline void get_system_time(PLARGE_INTEGER systime)
{
    FILETIME ft ;
    ::GetSystemTimeAsFileTime(&ft) ;
    systime->LowPart = ft.dwLowDateTime ;
    systime->HighPart = ft.dwHighDateTime ;
}
#define NSPERSEC 10000000LL
static clock_t __stdcall __to_clock_t (PLARGE_INTEGER src, int flag)
{
  uint64_t total = src->QuadPart;
  total /= NSPERSEC / CLOCKS_PER_SEC;
  return (clock_t)total;
}

clock_t times (struct tms * buffer)
{

    static SYSTEM_TIMEOFDAY_INFORMATION stodi;
    KERNEL_USER_TIMES kut;
    LARGE_INTEGER ticks;
    clock_t tc = (clock_t) -1;

    try
    {
        if (!stodi.BootTime.QuadPart)
            NtQuerySystemInformation (SystemTimeOfDayInformation, &stodi, sizeof stodi, NULL);

        NtQueryInformationProcess (NtCurrentProcess (), ProcessTimes,  &kut, sizeof kut, NULL);
        get_system_time (&ticks);

        ticks.QuadPart -= stodi.BootTime.QuadPart;
        tc = (clock_t) (ticks.QuadPart * CLOCKS_PER_SEC / NSPERSEC);

        buffer->tms_stime = __to_clock_t (&kut.KernelTime, 0);
        buffer->tms_utime = __to_clock_t (&kut.UserTime, 0);
        buffer->tms_cstime = __to_clock_t (&kut.KernelTime, 1);
        buffer->tms_cutime = __to_clock_t (&kut.UserTime, 1);
    }
    catch(...)
    {
        tc = (clock_t) -1;
    }
    return tc;
}


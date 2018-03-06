
#include <sys/times.h>
#include <windows.h>

//struct tms
//{
//    clock_t tms_utime;		/* User CPU time.  */
//    clock_t tms_stime;		/* System CPU time.  */

//    clock_t tms_cutime;		/* User CPU time of dead children.  */
//    clock_t tms_cstime;		/* System CPU time of dead children.  */
//};
/**
clock_t times (struct tms * buffer)
{

    static SYSTEM_TIMEOFDAY_INFORMATION stodi;
    KERNEL_USER_TIMES kut;
    LARGE_INTEGER ticks;
    clock_t tc = (clock_t) -1;

    __try
    {
      if (!stodi.BootTime.QuadPart)
    NtQuerySystemInformation (SystemTimeOfDayInformation,
			      &stodi, sizeof stodi, NULL);

      NtQueryInformationProcess (NtCurrentProcess (), ProcessTimes,
			     &kut, sizeof kut, NULL);
      get_system_time (&ticks);

      ticks.QuadPart -= stodi.BootTime.QuadPart;
      tc = (clock_t) (ticks.QuadPart * CLOCKS_PER_SEC / NSPERSEC);

      buf->tms_stime = __to_clock_t (&kut.KernelTime, 0);
      buf->tms_utime = __to_clock_t (&kut.UserTime, 0);
      timeval_to_filetime (&myself->rusage_children.ru_stime, &kut.KernelTime);
      buf->tms_cstime = __to_clock_t (&kut.KernelTime, 1);
      timeval_to_filetime (&myself->rusage_children.ru_utime, &kut.UserTime);
      buf->tms_cutime = __to_clock_t (&kut.UserTime, 1);
    }
    __except (EFAULT)
    {
      tc = (clock_t) -1;
    }
    __endtry
    syscall_printf ("%D = times(%p)", tc, buf);
    return tc;
}
*/

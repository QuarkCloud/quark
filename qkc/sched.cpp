
#include <sched.h>
#include <errno.h>
#include <windows.h>

int sched_yield (void)
{
    ::SwitchToThread() ;
    return 0 ;
}


/* copy from pthread w32 project
 * On Windows98, THREAD_PRIORITY_LOWEST is (-2) and 
 * THREAD_PRIORITY_HIGHEST is 2, and everything works just fine.
 * 
 * On WinCE 3.0, it so happen that THREAD_PRIORITY_LOWEST is 5
 * and THREAD_PRIORITY_HIGHEST is 1 (yes, I know, it is funny:
 * highest priority use smaller numbers) and the following happens:
 * 
 * sched_get_priority_min() returns 5
 * sched_get_priority_max() returns 1
 *
 * The following table shows the base priority levels for combinations
 * of priority class and priority value in Win32.
 *
 *   Process Priority Class               Thread Priority Level
 *   -----------------------------------------------------------------
 *   1 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_IDLE
 *   1 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_IDLE
 *   1 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_IDLE
 *   1 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_IDLE
 *   1 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_IDLE
 *   2 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_LOWEST
 *   3 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_BELOW_NORMAL
 *   4 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_NORMAL
 *   4 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_LOWEST
 *   5 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_ABOVE_NORMAL
 *   5 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_BELOW_NORMAL
 *   5 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_LOWEST
 *   6 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_HIGHEST
 *   6 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_NORMAL
 *   6 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_BELOW_NORMAL
 *   7 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_ABOVE_NORMAL
 *   7 Background NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_NORMAL
 *   7 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_LOWEST
 *   8 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_HIGHEST
 *   8 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_ABOVE_NORMAL
 *   8 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_BELOW_NORMAL
 *   8 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_LOWEST
 *   9 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_HIGHEST
 *   9 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_NORMAL
 *   9 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_BELOW_NORMAL
 *  10 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_ABOVE_NORMAL
 *  10 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_NORMAL
 *  11 Foreground NORMAL_PRIORITY_CLASS   THREAD_PRIORITY_HIGHEST
 *  11 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_ABOVE_NORMAL
 *  11 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_LOWEST
 *  12 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_HIGHEST
 *  12 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_BELOW_NORMAL
 *  13 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_NORMAL
 *  14 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_ABOVE_NORMAL
 *  15 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_HIGHEST
 *  15 HIGH_PRIORITY_CLASS                THREAD_PRIORITY_TIME_CRITICAL
 *  15 IDLE_PRIORITY_CLASS                THREAD_PRIORITY_TIME_CRITICAL
 *  15 BELOW_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_TIME_CRITICAL
 *  15 NORMAL_PRIORITY_CLASS              THREAD_PRIORITY_TIME_CRITICAL
 *  15 ABOVE_NORMAL_PRIORITY_CLASS        THREAD_PRIORITY_TIME_CRITICAL
 *  16 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_IDLE
 *  17 REALTIME_PRIORITY_CLASS            -7
 *  18 REALTIME_PRIORITY_CLASS            -6
 *  19 REALTIME_PRIORITY_CLASS            -5
 *  20 REALTIME_PRIORITY_CLASS            -4
 *  21 REALTIME_PRIORITY_CLASS            -3
 *  22 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_LOWEST
 *  23 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_BELOW_NORMAL
 *  24 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_NORMAL
 *  25 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_ABOVE_NORMAL
 *  26 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_HIGHEST
 *  27 REALTIME_PRIORITY_CLASS             3
 *  28 REALTIME_PRIORITY_CLASS             4
 *  29 REALTIME_PRIORITY_CLASS             5
 *  30 REALTIME_PRIORITY_CLASS             6
 *  31 REALTIME_PRIORITY_CLASS            THREAD_PRIORITY_TIME_CRITICAL
 *
 * Windows NT:  Values -7, -6, -5, -4, -3, 3, 4, 5, and 6 are not supported.
 */

int sched_get_priority_min (int policy)
{
    if(policy < SCHED_MIN || policy > SCHED_MAX)
        return -1;

    return THREAD_PRIORITY_IDLE ;
}

int sched_get_priority_max (int policy)
{
    if(policy < SCHED_MIN || policy > SCHED_MAX)
        return -1;

    return THREAD_PRIORITY_TIME_CRITICAL ;
}

int sched_setscheduler (pid_t pid, int policy)
{
    return SCHED_OTHER;
}

int sched_setaffinity (pid_t pid, size_t cpusetsize, cpu_set_t *set)
{
    DWORD_PTR vProcessMask;
    DWORD_PTR vSystemMask ;
    HANDLE h;
    int targetPid = (int)(size_t) pid;
    int result = 0;

    if(set == NULL)
        result = EFAULT;
    else
    {
        if(targetPid == 0)
	        targetPid = (int) GetCurrentProcessId ();

        h = OpenProcess (PROCESS_QUERY_INFORMATION|PROCESS_SET_INFORMATION, FALSE, (DWORD) targetPid);

        if(h == NULL)
	    {
	        result = (((0xFF & ERROR_ACCESS_DENIED) == GetLastError()) ? EPERM : ESRCH);
	    }
        else
	    {
            if(::GetProcessAffinityMask(h, &vProcessMask, &vSystemMask))
		    {
		        DWORD_PTR newMask = vSystemMask & ((_sched_cpu_set_vector_*)set)->_cpuset;

		        if(newMask)
			    {
			        if(SetProcessAffinityMask(h, newMask) == 0)
			        {
				        switch (GetLastError())
				        {
				  	        case (0xFF & ERROR_ACCESS_DENIED):
				  		        result = EPERM;
				  		        break;
				  	        case (0xFF & ERROR_INVALID_PARAMETER):
				  		        result = EINVAL;
				  		        break;
				  	        default:
				  		        result = EAGAIN;
				  		        break;
				        }
                    }
			    }
		        else
			    {
			        result = EINVAL;
			    }
		    }
	        else
		    {
		        result = EAGAIN;
		    }
	    }
        ::CloseHandle(h);
    }

    return ((result != 0) ? -1 : 0) ;
}

int sched_getaffinity (pid_t pid, size_t cpusetsize, cpu_set_t * set)
{
    DWORD_PTR vProcessMask;
    DWORD_PTR vSystemMask;
    HANDLE h;
    int targetPid = (int)(size_t) pid;
    int result = 0;

    if(set == NULL)
    {
        result = EFAULT;
    }
    else
    {
        if(targetPid == 0)
            targetPid = (int)::GetCurrentProcessId ();

        h = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD) targetPid);
        if(h == NULL)
        {
	        result = (((0xFF & ERROR_ACCESS_DENIED) == GetLastError()) ? EPERM : ESRCH);
        }
        else
        {
            if(::GetProcessAffinityMask (h, &vProcessMask, &vSystemMask))
	        {
		        ((_sched_cpu_set_vector_*)set)->_cpuset = vProcessMask;
	        }
	        else
	        {
		        result = EAGAIN;
	        }
        }
        ::CloseHandle(h);
    }

    return ((result != 0) ? -1 : 0) ;
}

int _sched_affinitycpucount (const cpu_set_t *set)
{
    size_t tset;
    int count;

    for(count = 0 , tset = ((_sched_cpu_set_vector_*)set)->_cpuset; tset; tset >>= 1)
    {
        if(tset & (size_t)1)
  	    {
	        count++;
  	    }
    }
    return count;
}

void _sched_affinitycpuzero (cpu_set_t *pset)
{
    ((_sched_cpu_set_vector_*)pset)->_cpuset = (size_t)0;
}

void _sched_affinitycpuset (int cpu, cpu_set_t *pset)
{
    ((_sched_cpu_set_vector_*)pset)->_cpuset |= ((size_t)1 << cpu);
}

void _sched_affinitycpuclr (int cpu, cpu_set_t *pset)
{
    ((_sched_cpu_set_vector_*)pset)->_cpuset &= ~((size_t)1 << cpu);
}

int _sched_affinitycpuisset (int cpu , const cpu_set_t *pset)
{
	return ((((_sched_cpu_set_vector_*)pset)->_cpuset & ((size_t)1 << cpu)) != (size_t)0) ;
}

void _sched_affinitycpuand(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2)
{
	((_sched_cpu_set_vector_*)pdestset)->_cpuset =
			(((_sched_cpu_set_vector_*)psrcset1)->_cpuset &
					((_sched_cpu_set_vector_*)psrcset2)->_cpuset);
}

void _sched_affinitycpuor(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2)
{
	((_sched_cpu_set_vector_*)pdestset)->_cpuset =
			(((_sched_cpu_set_vector_*)psrcset1)->_cpuset |
					((_sched_cpu_set_vector_*)psrcset2)->_cpuset);
}

void _sched_affinitycpuxor(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2)
{
	((_sched_cpu_set_vector_*)pdestset)->_cpuset =
			(((_sched_cpu_set_vector_*)psrcset1)->_cpuset ^
					((_sched_cpu_set_vector_*)psrcset2)->_cpuset);
}

int _sched_affinitycpuequal (const cpu_set_t *pset1, const cpu_set_t *pset2)
{
    return (((_sched_cpu_set_vector_*)pset1)->_cpuset ==  ((_sched_cpu_set_vector_*)pset2)->_cpuset);

}


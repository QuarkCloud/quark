
#include <sys/resource.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "wintf/werr.h"
#include "wintf/wps.h"

int getrusage(int who, struct rusage *usage)
{
    if(who != RUSAGE_SELF && who != RUSAGE_THREAD)
    {
        errno = ENOSYS ;
        return -1 ;
    }

    FILETIME createTime, exitTime, kernelTime, userTime;
    SYSTEMTIME kernelSystemTime, userSystemTime;
    process_memory_info_t memCounters ;
    IO_COUNTERS ioCounters;
    int ret;

    ret = GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime);
    if (ret == 0)
    {
        errno = oserr_map(GetLastError());
        return -1 ;
    }

    ret = FileTimeToSystemTime(&kernelTime, &kernelSystemTime);
    if (ret == 0)
    {
        errno = oserr_map(GetLastError());
        return -1 ;
    }

    ret = FileTimeToSystemTime(&userTime, &userSystemTime);
    if (ret == 0)
    {
        errno = oserr_map(GetLastError());
        return -1 ;
    }

    //ret = GetProcessMemoryInfo(GetCurrentProcess(),&memCounters,sizeof(memCounters));
    if(get_process_memory_info(0 , &memCounters) == false)
        return -1 ;

    ret = GetProcessIoCounters(GetCurrentProcess(), &ioCounters);
    if (ret == 0)
    {
        errno = oserr_map(GetLastError());
        return -1 ;
    }

    memset(usage, 0, sizeof(*usage));

    usage->ru_utime.tv_sec = userSystemTime.wHour * 3600 +
                               userSystemTime.wMinute * 60 +
                               userSystemTime.wSecond;
    usage->ru_utime.tv_usec = userSystemTime.wMilliseconds * 1000;

    usage->ru_stime.tv_sec = kernelSystemTime.wHour * 3600 +
                               kernelSystemTime.wMinute * 60 +
                               kernelSystemTime.wSecond;
    usage->ru_stime.tv_usec = kernelSystemTime.wMilliseconds * 1000;

    usage->ru_majflt = (uint64_t) memCounters.PageFaultCount;
    usage->ru_maxrss = (uint64_t) memCounters.PeakWorkingSetSize / 1024;

    usage->ru_oublock = (long) ioCounters.WriteOperationCount;
    usage->ru_inblock = (long) ioCounters.ReadOperationCount;

    return 0;
}

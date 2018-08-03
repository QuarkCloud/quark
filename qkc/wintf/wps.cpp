
#include <wintf/wps.h>
#include <wintf/werr.h>
#include <windows.h>
#include <errno.h>

typedef struct _PROCESS_MEMORY_COUNTERS {
    DWORD cb;
    DWORD PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} PROCESS_MEMORY_COUNTERS;

typedef BOOL (WINAPI *GetProcessMemoryInfo_t)(HANDLE Process,PROCESS_MEMORY_COUNTERS * ppsmemCounters,DWORD cb);

bool get_process_memory_info(pid_t pid , process_memory_info_t * info)
{
    HMODULE module = ::GetModuleHandle("kernel32.dll") ;
    if(module == NULL)
    {
        errno = oserr_map(::GetLastError()) ;
        return false ;
    }

    GetProcessMemoryInfo_t K32GetProcessMemoryInfo = (GetProcessMemoryInfo_t)::GetProcAddress(module , "K32GetProcessMemoryInfo") ;
    if(K32GetProcessMemoryInfo == NULL)
    {
        errno = oserr_map(::GetLastError()) ;
        return false ;
    }

    PROCESS_MEMORY_COUNTERS memory ;
    if(K32GetProcessMemoryInfo(::GetCurrentProcess() , &memory , sizeof(memory)) == FALSE)
    {
        errno = oserr_map(::GetLastError()) ;
        return false ;
    }

    info->PageFaultCount = memory.PageFaultCount ;
    info->PeakWorkingSetSize = memory.PeakWorkingSetSize ;
    info->WorkingSetSize = memory.WorkingSetSize ;
    info->QuotaPeakPagedPoolUsage = memory.QuotaPeakPagedPoolUsage ;
    info->QuotaPagedPoolUsage = memory.QuotaPagedPoolUsage ;

    info->QuotaPeakNonPagedPoolUsage = memory.QuotaPeakNonPagedPoolUsage ;
    info->QuotaNonPagedPoolUsage = memory.QuotaNonPagedPoolUsage ;
    info->PagefileUsage = memory.PagefileUsage ;
    info->PeakPagefileUsage = memory.PeakPagefileUsage ;

    return true ;
}

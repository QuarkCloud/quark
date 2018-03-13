
#include <windows.h>
#include <wintf/wcap.h>


static wcap_t __wcap_info__ ;
static bool __wcap_info_inited__ = false ;


const wcap_t * GetSysInfo(void)
{
    if(__wcap_info_inited__ == false)
    {
        SYSTEM_INFO sysinfo ;
        ::GetSystemInfo(&sysinfo) ;

        __wcap_info__.PageSize = sysinfo.dwPageSize ;
        __wcap_info__.ActiveProcessorMask = sysinfo.dwActiveProcessorMask ;
        __wcap_info__.NumberOfProcessors = sysinfo.dwNumberOfProcessors ;
        __wcap_info__.AllocationGranularity = sysinfo.dwAllocationGranularity ;

        __wcap_info_inited__ = true ;        
    }

    return &__wcap_info__ ;
}

DWORD GetPageSize(void)
{
    const wcap_t * wcap = GetSysInfo() ;
    return wcap->PageSize ;
}

DWORD_PTR GetActiveProcessorMask(void)
{
    const wcap_t * wcap = GetSysInfo() ;
    return wcap->ActiveProcessorMask ;
}

DWORD GetNumberOfProcessors(void)
{
    const wcap_t * wcap = GetSysInfo() ;
    return wcap->NumberOfProcessors ;
}

DWORD GetAllocationGranularity(void)
{
    const wcap_t * wcap = GetSysInfo() ;
    return wcap->AllocationGranularity ;
}



#ifndef __QKC_WINTF_WCAP_H
#define __QKC_WINTF_WCAP_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>

__BEGIN_DECLS

typedef struct __st_wcap{

    DWORD PageSize ;
    DWORD_PTR ActiveProcessorMask ;
    DWORD NumberOfProcessors ;
    DWORD AllocationGranularity;
} wcap_t ;


QKCAPI const wcap_t * GetSysInfo(void) ;
QKCAPI DWORD GetPageSize(void) ;
QKCAPI DWORD_PTR GetActiveProcessorMask(void) ;
QKCAPI DWORD GetNumberOfProcessors(void) ;
QKCAPI DWORD GetAllocationGranularity(void) ;

__END_DECLS

#endif /** __QKC_WINTF_WCAP_H */


#ifndef __QKC_WINTF_WPS_H
#define __QKC_WINTF_WPS_H 1

#include <quark_compile.h>
#include <sys/types.h>

__BEGIN_DECLS

typedef struct __st_process_memory_info{
    size_t PageFaultCount;
    size_t PeakWorkingSetSize;
    size_t WorkingSetSize;
    size_t QuotaPeakPagedPoolUsage;
    size_t QuotaPagedPoolUsage;
    size_t QuotaPeakNonPagedPoolUsage;
    size_t QuotaNonPagedPoolUsage;
    size_t PagefileUsage;
    size_t PeakPagefileUsage;
} process_memory_info_t ;

QKCAPI bool get_process_memory_info(pid_t pid , process_memory_info_t * info);

__END_DECLS

#endif /** __QKC_WINTF_WPS_H */

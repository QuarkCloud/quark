
#ifndef __QUARK_WOBJS_PROCESS_MEMORY_INFO_H
#define __QUARK_WOBJS_PROCESS_MEMORY_INFO_H 1

#include <quark_compile.h>
#include <sys/types.h>

namespace qkc {

	class QKCAPI ProcessMemoryInfo {
	public :
		ProcessMemoryInfo();
		~ProcessMemoryInfo();

		size_t PageFaultCount;
		size_t PeakWorkingSetSize;
		size_t WorkingSetSize;
		size_t QuotaPeakPagedPoolUsage;
		size_t QuotaPagedPoolUsage;
		size_t QuotaPeakNonPagedPoolUsage;
		size_t QuotaNonPagedPoolUsage;
		size_t PagefileUsage;
		size_t PeakPagefileUsage;

		bool Load();
	};

}

#endif  /** __QUARK_WOBJS_PROCESS_MEMORY_INFO_H */


#include <windows.h>
#include <string.h>
#include <errno.h>

#include "wobjs/ProcessMemoryInfo.h"
#include "wobjs/ErrorMap.h"

namespace qkc {

	ProcessMemoryInfo::ProcessMemoryInfo()
	{
		PageFaultCount = 0;
		PeakWorkingSetSize = 0;
		WorkingSetSize = 0;
		QuotaPeakPagedPoolUsage = 0;
		QuotaPagedPoolUsage = 0;
		QuotaPeakNonPagedPoolUsage = 0;
		QuotaNonPagedPoolUsage = 0;
		PagefileUsage = 0;
		PeakPagefileUsage = 0;
	}
	ProcessMemoryInfo::~ProcessMemoryInfo()
	{
		//
	}

	/**
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

	typedef BOOL(WINAPI *GetProcessMemoryInfo_t)(HANDLE Process, PROCESS_MEMORY_COUNTERS * ppsmemCounters, DWORD cb);
	*/
	bool ProcessMemoryInfo::Load()
	{
		/**
		HMODULE module = ::GetModuleHandle("kernel32.dll");
		if (module == NULL)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		GetProcessMemoryInfo_t K32GetProcessMemoryInfo = (GetProcessMemoryInfo_t)::GetProcAddress(module, "K32GetProcessMemoryInfo");
		if (K32GetProcessMemoryInfo == NULL)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}
		*/

		PROCESS_MEMORY_COUNTERS memory;
		if (GetProcessMemoryInfo(::GetCurrentProcess(), &memory, sizeof(memory)) == FALSE)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		PageFaultCount = memory.PageFaultCount;
		PeakWorkingSetSize = memory.PeakWorkingSetSize;
		WorkingSetSize = memory.WorkingSetSize;
		QuotaPeakPagedPoolUsage = memory.QuotaPeakPagedPoolUsage;
		QuotaPagedPoolUsage = memory.QuotaPagedPoolUsage;

		QuotaPeakNonPagedPoolUsage = memory.QuotaPeakNonPagedPoolUsage;
		QuotaNonPagedPoolUsage = memory.QuotaNonPagedPoolUsage;
		PagefileUsage = memory.PagefileUsage;
		PeakPagefileUsage = memory.PeakPagefileUsage;

		return true;
	}

}

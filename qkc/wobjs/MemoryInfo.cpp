
#include "wobjs/MemoryInfo.h"
#include "wobjs/ErrorMap.h"
#include <errno.h>


namespace qkc {

	MemoryInfo::MemoryInfo()
	{
		module_ = NULL;
		Clear();
	}

	MemoryInfo::~MemoryInfo()
	{
		if (module_ != NULL && module_ != INVALID_HANDLE_VALUE)
		{
			module_ = NULL;
		}
	}

	bool MemoryInfo::Load()
	{
		PROCESS_MEMORY_COUNTERS memory;
		if (GetProcessMemoryInfo(::GetCurrentProcess(), &memory, sizeof(memory)) == FALSE)
		{
			errno = oserr_map(::GetLastError());
			return false;
		}

		page_fault_count_ = memory.PageFaultCount;
		peak_working_set_size_ = memory.PeakWorkingSetSize;
		working_set_size_ = memory.WorkingSetSize;
		quota_peak_paged_pool_usage_ = memory.QuotaPeakPagedPoolUsage;
		quota_paged_pool_usage_ = memory.QuotaPagedPoolUsage;
		quota_peak_non_paged_pool_usage_ = memory.QuotaPeakNonPagedPoolUsage;
		quota_non_paged_pool_usage_ = memory.QuotaNonPagedPoolUsage;
		page_file_usage_ = memory.PagefileUsage;
		peak_page_file_usage_ = memory.PeakPagefileUsage;

		return true;	
	}

	void MemoryInfo::Clear()
	{
		page_fault_count_ = 0;
		peak_working_set_size_ = 0;
		working_set_size_ = 0;

		quota_peak_paged_pool_usage_ = 0;
		quota_paged_pool_usage_ = 0;
		quota_peak_non_paged_pool_usage_ = 0;
		quota_non_paged_pool_usage_ = 0;

		page_file_usage_ = 0;
		peak_page_file_usage_ = 0;
	}


}
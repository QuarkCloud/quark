
#include "wobjs/SysInfo.h"
#include <windows.h>

namespace qkc {

	SysInfo::SysInfo()
	{
		inited_ = false;
	}

	SysInfo::~SysInfo()
	{
		//
	}

	bool SysInfo::Init()
	{
		if (inited_ == true)
			return true;

		SYSTEM_INFO sysinfo;
		::GetSystemInfo(&sysinfo);

		page_size_ = sysinfo.dwPageSize;
		active_processor_mask_ = sysinfo.dwActiveProcessorMask;
		number_of_processors_ = sysinfo.dwNumberOfProcessors;
		allocation_granularity_ = sysinfo.dwAllocationGranularity;

		inited_ = true;
		return true;
	}

	SysInfo& SysInfo::Singleton()
	{
		static SysInfo __sysinfo_singleton__;

		__sysinfo_singleton__.Init() ;
		return __sysinfo_singleton__;		
	}
}


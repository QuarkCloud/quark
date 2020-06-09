
#ifndef __QUARK_WOBJS_MEMORY_INFO_H
#define __QUARK_WOBJS_MEMORY_INFO_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>

namespace qkc {
	class QKCAPI MemoryInfo {
	public:
		MemoryInfo();
		~MemoryInfo();

		bool Load();
		void Clear();

		inline uint32_t PageFaultCount() const { return page_fault_count_; }
		inline uint32_t PeakWorkingSetSize() const { return peak_working_set_size_; }
		inline uint32_t WorkingSetSize() const { return working_set_size_; }
		inline uint32_t QuotaPeakPagedPoolUsage() const { return quota_peak_paged_pool_usage_; }
		inline uint32_t QuotaPagedPoolUsage() const { return quota_paged_pool_usage_; }
		inline uint32_t QuotaPeakNonPagedPoolUsage() const { return quota_peak_non_paged_pool_usage_; }
		inline uint32_t QuotaNonPagedPoolUsage() const { return quota_non_paged_pool_usage_; }
		inline uint32_t PagefileUsage() const { return page_file_usage_; }
		inline uint32_t PeakPagefileUsage() const { return peak_page_file_usage_; }

	private:

		uint32_t page_fault_count_;
		uint32_t peak_working_set_size_;
		uint32_t working_set_size_;
		uint32_t quota_peak_paged_pool_usage_;
		uint32_t quota_paged_pool_usage_;
		uint32_t quota_peak_non_paged_pool_usage_;
		uint32_t quota_non_paged_pool_usage_;
		uint32_t page_file_usage_ ;
		uint32_t peak_page_file_usage_;

		HMODULE module_;
	};

}

#endif  /** __QUARK_WOBJS_MEMORY_INFO_H */

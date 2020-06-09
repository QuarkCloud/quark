
#ifndef __QUARK_WOBJS_SYS_INFO_H
#define __QUARK_WOBJS_SYS_INFO_H 1

#include <quark_compile.h>
#include <stdint.h>

namespace qkc {
	class QKCAPI SysInfo {
	public:
		SysInfo();
		~SysInfo();

		bool Init();

		inline uint32_t PageSize() const { return page_size_; }
		inline uintptr_t ActiveProcessorMask() const { return active_processor_mask_; }
		inline uint32_t NumberOfProcessors() const { return number_of_processors_; }
		inline uint32_t AllocationGranularity() const { return allocation_granularity_; }

		static SysInfo& Singleton();
	private:
		uint32_t page_size_;
		uintptr_t active_processor_mask_;
		uint32_t number_of_processors_;
		uint32_t allocation_granularity_;

		bool inited_;
	};
}

#endif  /** __QUARK_WOBJS_SYS_INFO_H */

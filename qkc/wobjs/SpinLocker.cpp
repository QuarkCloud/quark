
#include "wobjs/SpinLocker.h"
#include <stdint.h>

namespace qkc {

	SpinLocker::SpinLocker()
	{
		token_ = 0;
		allow_ = 1;
		owner_ = 0;

		OID(Object::AllocID());
		OType(Object::kSpinLocker);
		OName(Object::Type2Name(OType()));	
	}

	SpinLocker::~SpinLocker()
	{
		allow_ = token_;
		owner_ = 0;
	}

	int SpinLocker::Lock()
	{
		LONG token = ::InterlockedIncrement(&token_);
		uint32_t counter = 0;
		while (allow_ < token)
		{
			++counter;
			if (counter >= 256)
			{
				counter = 0;
				::SwitchToThread();
			}
		}

		if (allow_ != token)
			return -1;

		allow_ = token;
		owner_ = ::GetCurrentThreadId();
		::MemoryBarrier();
		return 0;
	}

	int SpinLocker::TryLock()
	{
		LONG token = token_;
		if (allow_ == token + 1)
		{
			if (::InterlockedCompareExchange(&token_, token + 1, token) == token + 1)
			{
				owner_ = ::GetCurrentThreadId();
				::MemoryBarrier();
				return 0;
			}
		}

		return -1;

	}

	int SpinLocker::Unlock()
	{
		owner_ = 0;
		allow_++;
		::MemoryBarrier();
		return 0;
	}

}


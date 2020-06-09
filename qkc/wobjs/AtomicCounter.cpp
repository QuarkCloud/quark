
#include "wobjs/AtomicCounter.h"

namespace qkc {


	AtomicCounter::AtomicCounter()
	{
		value_ = 0;
	}

	AtomicCounter::~AtomicCounter()
	{
		//
	}

	LONG AtomicCounter::Increment()
	{
		return ::InterlockedIncrement(&value_);
	}

	LONG AtomicCounter::Decrement()
	{
		return ::InterlockedDecrement(&value_);
	}

	LONG AtomicCounter::Value() const
	{
		return ::InterlockedCompareExchange(&value_, 0, value_);
	}

}


#ifndef __QUARK_WOBJS_ATOMIC_COUNTER_H
#define __QUARK_WOBJS_ATOMIC_COUNTER_H 1

#include <quark_compile.h>
#include <windows.h>

namespace qkc {

	class QKCAPI AtomicCounter{
	public:
		AtomicCounter();
		virtual ~AtomicCounter();

		LONG Increment();
		LONG Decrement();
		LONG Value() const;

	private:
		AtomicCounter(const AtomicCounter& counter);
		AtomicCounter& operator=(const AtomicCounter& counter);
		mutable volatile LONG value_;
	};
}

#endif  /** __QUARK_WOBJS_ATOMIC_COUNTER_H */

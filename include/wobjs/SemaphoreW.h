
#ifndef __QUARK_WOBJS_SEMAPHORE_H
#define __QUARK_WOBJS_SEMAPHORE_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"

namespace qkc {

	class QKCAPI Semaphore : public Object {
	public:
		Semaphore();
		Semaphore(int value);
		virtual ~Semaphore();

		void SetInfo();

		int Post(int count = 1);
		int Wait();
		int TimedWait(int timeout);
		int Value() const;
	private:
		Semaphore(const Semaphore& locker);
		Semaphore& operator=(const Semaphore& locker);
		HANDLE handle_;
		LONG value_;
	};
}

#endif  /** __QUARK_WOBJS_SEMAPHORE_H */

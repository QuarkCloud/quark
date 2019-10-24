
#ifndef __QUARK_WOBJS_CONDITION_H
#define __QUARK_WOBJS_CONDITION_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"
#include "wobjs/Mutex.h"
#include "wobjs/SemaphoreW.h"

namespace qkc {

	class QKCAPI Condition : public Object {
	public:
		Condition();
		virtual ~Condition();

		int Notify();
		int Broadcast();
		int Wait(Mutex& locker);
		int TimedWait(Mutex& locker , int timeout);
	protected:
		void SetInfo();
	private:
		Condition(const Condition& locker);
		Condition& operator=(const Condition& locker);
		Mutex guard_;
		Semaphore sema_;
	};
}

#endif  /** __QUARK_WOBJS_CONDITION_H */

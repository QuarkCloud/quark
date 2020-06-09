
#ifndef __QUARK_WOBJS_MUTEX_H
#define __QUARK_WOBJS_MUTEX_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"

namespace qkc {

	class QKCAPI Mutex : public Object {
	public:
		Mutex();
		virtual ~Mutex();

		int Lock();
		int TryLock();
		int TimedLock(int msec);
		int Unlock();

		class QKCAPI ScopedLock {
		public:
			ScopedLock(Mutex& locker) : locker_(locker) { locker_.Lock(); }
			~ScopedLock() { locker_.Unlock(); }
		private:
			ScopedLock(const ScopedLock& locker);
			ScopedLock& operator=(const ScopedLock&);

			Mutex& locker_;
		};
	
	protected:
		void SetInfo();
	private:
		Mutex(const Mutex& locker);
		Mutex& operator=(const Mutex& locker);
		HANDLE handle_;
	};
}

#endif  /** __QUARK_WOBJS_MUTEX_H */

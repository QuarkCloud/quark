
#ifndef __QUARK_WOBJS_RWLOCKER_H
#define __QUARK_WOBJS_RWLOCKER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"

namespace qkc {

	class QKCAPI RWLocker : public Object{
	public:
		RWLocker();
		virtual ~RWLocker();

		void Lock();
		void Unlock();
		void LockShared() const;
		void UnlockShared() const;

		class QKCAPI ScopedLock {
		public:
			ScopedLock(RWLocker& locker) : locker_(locker) { locker_.Lock(); }
			~ScopedLock() { locker_.Unlock(); }
		private:
			ScopedLock(const ScopedLock& locker);
			ScopedLock& operator=(const ScopedLock&) { return *this; }

			RWLocker& locker_;
		};

		class QKCAPI ScopedSharedLock {
		public:
			ScopedSharedLock(RWLocker& locker) : locker_(locker) { locker_.LockShared(); }
			~ScopedSharedLock() { locker_.UnlockShared(); }
		private:
			ScopedSharedLock(const ScopedSharedLock& locker);
			ScopedSharedLock& operator=(const ScopedSharedLock&) { return *this; }

			RWLocker& locker_;
		};

	protected:
		void SetInfo();
	private:
		RWLocker(const RWLocker& locker);
		RWLocker& operator=(const RWLocker& locker) { return (*this); }
		mutable SRWLOCK locker_;
	};
}

#endif  /** __QUARK_WOBJS_RWLOCKER_H */

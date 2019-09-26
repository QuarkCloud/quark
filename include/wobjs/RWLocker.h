
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
		void LockShared();
		void UnlockShared();
	private:
		RWLocker(const RWLocker& locker);
		RWLocker& operator=(const RWLocker& locker);
		SRWLOCK locker_;
	};
}

#endif  /** __QUARK_WOBJS_RWLOCKER_H */


#ifndef __QUARK_WOBJS_SPIN_LOCKER_H
#define __QUARK_WOBJS_SPIN_LOCKER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"

namespace qkc {

	class QKCAPI SpinLocker : public Object {
	public:
		SpinLocker();
		virtual ~SpinLocker();

		int Lock();
		int TryLock();
		int Unlock();
	protected:
		void SetInfo();
	private:
		SpinLocker(const SpinLocker& locker);
		SpinLocker& operator=(const SpinLocker& locker);
		mutable volatile LONG token_;
		LONG allow_;
		DWORD owner_;//Ïß³Ì¾ä±ú
	};
}

#endif  /** __QUARK_WOBJS_SPIN_LOCKER_H */

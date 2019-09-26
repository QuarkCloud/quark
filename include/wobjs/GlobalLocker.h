
#ifndef __QUARK_WOBJS_GLOBAL_LOCKER_H
#define __QUARK_WOBJS_GLOBAL_LOCKER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		由于是全局读写锁，因此影响比较大，不能被频繁调用。最好用于创建和释放对象的保护。
	*/
	class QKCAPI GlobalLocker : public RWLocker {
	public:
		GlobalLocker();
		virtual ~GlobalLocker();

		static GlobalLocker kLocker;

	private:
		GlobalLocker(const GlobalLocker& locker);
		GlobalLocker& operator =(const GlobalLocker& locker);
	};
}

#endif  /** __QUARK_WOBJS_GLOBAL_LOCKER_H */

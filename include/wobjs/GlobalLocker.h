
#ifndef __QUARK_WOBJS_GLOBAL_LOCKER_H
#define __QUARK_WOBJS_GLOBAL_LOCKER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		������ȫ�ֶ�д�������Ӱ��Ƚϴ󣬲��ܱ�Ƶ�����á�������ڴ������ͷŶ���ı�����
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

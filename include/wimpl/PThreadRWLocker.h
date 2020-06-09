
#ifndef __QKC_PTHREAD_RWLOCKER_H
#define __QKC_PTHREAD_RWLOCKER_H 1

#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/Mutex.h"

/**
	2020-03-23
	和pthread_mutex一样，因为initialer在定义中只有索引，还有在初始化时，该索引可以被设定为0。
*/
namespace qkc {

	//请勿从外部直接使用该类，只供pthread_mutex使用
	class QKCAPI PThreadRWLocker : public Mutex{
	public:
		PThreadRWLocker();
		virtual ~PThreadRWLocker();

		int RdLock();
		int TryRdLock();
		int TimedRdLock(int msec);

		int WrLock();
		int TryWrLock();
		int TimedWrLock(int msec);

		int RWUnLock();
	protected:
		void SetInfo();
	private:

		PThreadRWLocker(const PThreadRWLocker& locker);
		PThreadRWLocker& operator=(const PThreadRWLocker& locker);

		SRWLOCK  rwlocker_;
		int writers_;
		int readers_;
	};

	QKCAPI bool EnsurePThreadRWLockerInited(int * index);
}

#endif /** __QKC_PTHREAD_RWLOCKER_H */
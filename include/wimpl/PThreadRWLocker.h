
#ifndef __QKC_PTHREAD_RWLOCKER_H
#define __QKC_PTHREAD_RWLOCKER_H 1

#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/Mutex.h"

/**
	2020-03-23
	��pthread_mutexһ������Ϊinitialer�ڶ�����ֻ�������������ڳ�ʼ��ʱ�����������Ա��趨Ϊ0��
*/
namespace qkc {

	//������ⲿֱ��ʹ�ø��ֻ࣬��pthread_mutexʹ��
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
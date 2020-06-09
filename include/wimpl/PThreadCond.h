
#ifndef __QKC_PTHREAD_COND_H
#define __QKC_PTHREAD_COND_H 1

#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/Mutex.h"
#include "wimpl/PThreadMutex.h"

/**
	2020-03-23
	��pthread_mutexһ������Ϊinitialer�ڶ�����ֻ�������������ڳ�ʼ��ʱ�����������Ա��趨Ϊ0��
*/
namespace qkc {

	//������ⲿֱ��ʹ�ø��ֻ࣬��pthread_mutexʹ��
	class QKCAPI PThreadCond : public Object{
	public:
		PThreadCond();
		virtual ~PThreadCond();

		int Signal();
		int BroadCast();
		int Wait(PThreadMutex * mutex);
		int TimedWait(PThreadMutex * mutex , int msec);

		inline void * Handle() { return &cond_variable_; }

	protected:
		void SetInfo();
	private:
		PThreadCond(const PThreadCond& locker);
		PThreadCond& operator=(const PThreadCond& locker);

		int flag_;
		int pad_;
		CONDITION_VARIABLE cond_variable_;
	};

	QKCAPI bool EnsurePThreadCondInited(int * index);
}

#endif /** __QKC_PTHREAD_COND_H */
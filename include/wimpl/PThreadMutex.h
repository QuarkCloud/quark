
#ifndef __QKC_PTHREAD_MUTEX_H
#define __QKC_PTHREAD_MUTEX_H 1

#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/Mutex.h"

/**
	2020-03-21
	pthread_mutex������pthread.h�ж�������������ֱ��ʹ��wobj/Mutex.h��
	��Ҫ����Ϊpthread_cond��windows��Ӧ��SRWLock����ʹ�õ���CriticalSection������Mutex��
	2020-03-22
	����һ��ԭ��pthread_mutex�ڶ�����ֻ�������������ڳ�ʼ��ʱ�����������Ա��趨Ϊ0��
*/
namespace qkc {

	//������ⲿֱ��ʹ�ø��ֻ࣬��pthread_mutexʹ��
	class QKCAPI PThreadMutex : public Mutex{
	public:
		PThreadMutex();
		virtual ~PThreadMutex();

		//����Enter֮ǰ��ȷ���Ѿ�ʹ�ù�Lock��
		int Enter();
		int Leave();

		inline LPCRITICAL_SECTION CriticalSection() { return critical_section_ ;}

	protected:
		void SetInfo();
	private:
		PThreadMutex(const PThreadMutex& locker);
		PThreadMutex& operator=(const PThreadMutex& locker);

		LPCRITICAL_SECTION critical_section_;	//����pthread_cond������
	};

	QKCAPI bool EnsurePThreadMutexInited(int * index);
}

#endif /** __QKC_PTHREAD_MUTEX_H */
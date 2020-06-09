
#ifndef __QKC_PTHREAD_MUTEX_H
#define __QKC_PTHREAD_MUTEX_H 1

#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/Mutex.h"

/**
	2020-03-21
	pthread_mutex单独从pthread.h中独立出来而不是直接使用wobj/Mutex.h，
	主要是因为pthread_cond和windows对应的SRWLock，他使用的是CriticalSection而不是Mutex。
	2020-03-22
	另外一个原因，pthread_mutex在定义中只有索引，还有在初始化时，该索引可以被设定为0。
*/
namespace qkc {

	//请勿从外部直接使用该类，只供pthread_mutex使用
	class QKCAPI PThreadMutex : public Mutex{
	public:
		PThreadMutex();
		virtual ~PThreadMutex();

		//调用Enter之前，确保已经使用过Lock了
		int Enter();
		int Leave();

		inline LPCRITICAL_SECTION CriticalSection() { return critical_section_ ;}

	protected:
		void SetInfo();
	private:
		PThreadMutex(const PThreadMutex& locker);
		PThreadMutex& operator=(const PThreadMutex& locker);

		LPCRITICAL_SECTION critical_section_;	//用于pthread_cond的锁定
	};

	QKCAPI bool EnsurePThreadMutexInited(int * index);
}

#endif /** __QKC_PTHREAD_MUTEX_H */
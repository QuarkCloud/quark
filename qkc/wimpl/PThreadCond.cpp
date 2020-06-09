
#include <quark_compile.h>
#include <time.h>
#include <errno.h>
#include <windows.h>
#include "wobjs/RWLocker.h"
#include "wobjs/DateTime.h"
#include "wobjs/ObjMgr.h"
#include "wimpl/PThreadCond.h"

namespace qkc {

	PThreadCond::PThreadCond()
	{
		::InitializeConditionVariable(&cond_variable_);
	}

	PThreadCond::~PThreadCond()
	{
		::WakeAllConditionVariable(&cond_variable_);
	}

	int PThreadCond::Signal()
	{
		::WakeConditionVariable(&cond_variable_);
		return 0;
	}

	int PThreadCond::BroadCast()
	{
		::WakeAllConditionVariable(&cond_variable_);
		return 0;
	}

	int PThreadCond::Wait(PThreadMutex * mutex)
	{
		if (mutex == NULL)
			return -1;

		mutex->Enter();
		BOOL result = ::SleepConditionVariableCS(&cond_variable_, mutex->CriticalSection() , INFINITE);
		mutex->Leave();

		return (result ? 0 : -1);
	}

	int PThreadCond::TimedWait(PThreadMutex * mutex, int msec)
	{
		if (mutex == NULL)
			return -1;

		mutex->Enter();

		int result = 0;
		if (::SleepConditionVariableCS(&cond_variable_ , mutex->CriticalSection() , (DWORD)msec) == FALSE)
		{
			if (::GetLastError() == ERROR_TIMEOUT)
				errno = ETIMEDOUT;
			result = -1;
		}

		mutex->Leave();

		return result;
	}

	void PThreadCond::SetInfo()
	{
		Object::OType(Object::kPthreadCond);
	}

	static RWLocker __pthread_cond_inited_guard__;
	bool EnsurePThreadCondInited(int * index)
	{
		if (index == NULL)
			return false;

		if (*index == 0)
		{
			__pthread_cond_inited_guard__.Lock();
			if (*index == 0)
			{
				PThreadCond * cond = new PThreadCond();
				*index = cond->OIndex();
			}
			__pthread_cond_inited_guard__.Unlock();
		}

		return (*index != 0);
	}

}

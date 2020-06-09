
#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wimpl/PThreadMutex.h"
#include "wobjs/MemMgr.h"
#include "wobjs/RWLocker.h"
#include "wobjs/ObjMgr.h"

namespace qkc {

	PThreadMutex::PThreadMutex()
	{
		critical_section_ = NULL;
		ObjMgr::Singleton().Add(&critical_section_, this);
	}

	PThreadMutex::~PThreadMutex()
	{
		Mutex::ScopedLock locker(*this);
		if (critical_section_ != NULL)
		{
			::DeleteCriticalSection(critical_section_);
			critical_section_ = NULL;
		}
	}

	int PThreadMutex::Enter()
	{
		if (critical_section_ == NULL)
		{
			LPCRITICAL_SECTION cs = (LPCRITICAL_SECTION)MemAlloc(sizeof(CRITICAL_SECTION) , true);
			if (cs == NULL)
				return -1;

			::InitializeCriticalSection(cs);
			critical_section_ = cs;
		}

		EnterCriticalSection(critical_section_);

		Unlock();
		return 0;
	}

	int PThreadMutex::Leave()
	{
		if (critical_section_ == NULL)
			return -1;

		::LeaveCriticalSection(critical_section_);
		return Lock();	
	}


	void PThreadMutex::SetInfo()
	{
		OType(Object::kPthreadMutex);
	}

	static RWLocker __pthread_mutex_inited_guard__;
	bool EnsurePThreadMutexInited(int * index)
	{
		if (index == NULL)
			return false;

		if (*index == 0)
		{
			__pthread_mutex_inited_guard__.Lock();
			if (*index == 0)
			{
				PThreadMutex * mutex = new PThreadMutex();
				*index = mutex->OIndex();
			}
			__pthread_mutex_inited_guard__.Unlock();
		}

		return (*index != 0);
	}
}

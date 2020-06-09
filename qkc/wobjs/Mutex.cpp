
#include "wobjs/Mutex.h"

namespace qkc {

	Mutex::Mutex()
	{
		handle_ = ::CreateMutexA(NULL, FALSE, NULL);
	}

	void Mutex::SetInfo()
	{
		OType(Object::kMutex);
	}

	Mutex::~Mutex()
	{
		if (handle_ != NULL)
		{
			::CloseHandle(handle_);
			handle_ = NULL;
		}
	}

	int Mutex::Lock()
	{
		if (::WaitForSingleObject(handle_, INFINITE) != WAIT_OBJECT_0)
			return -1;
		else
			return 0;
	}

	int Mutex::TryLock()
	{
		if (::WaitForSingleObject(handle_, 0) != WAIT_OBJECT_0)
			return -1;
		else
			return 0;

	}

	int Mutex::TimedLock(int msec)
	{
		if (msec < 0)
			return Lock();
		else if (msec == 0)
			return TryLock();
	
	}

	int Mutex::Unlock()
	{
		if (::ReleaseMutex(handle_) == TRUE)
			return 0;
		else
			return -1;
	}

}

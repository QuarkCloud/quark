
#include "wobjs/Mutex.h"

namespace qkc {

	Mutex::Mutex()
	{
		OID(Object::AllocID());
		OType(Object::kMutex);
		OName(Object::Type2Name(OType()));

		handle_ = ::CreateMutexA(NULL, FALSE, NULL);
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

	int Mutex::Unlock()
	{
		if (::ReleaseMutex(handle_) == TRUE)
			return 0;
		else
			return -1;
	}

}

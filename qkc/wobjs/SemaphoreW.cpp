
#include "wobjs/Semaphore.h"
#include <errno.h>

namespace qkc {

	Semaphore::Semaphore()
	{
		value_ = 1;
		handle_ = ::CreateSemaphore(NULL , value_ , 65536 , NULL);
	}

	Semaphore::Semaphore(int value)
	{
		value_ = value;
		handle_ = ::CreateSemaphore(NULL, value, 65536, NULL);
	}

	Semaphore::~Semaphore()
	{
		value_ = 0;
		if (handle_ != NULL)
		{
			::CloseHandle(handle_);
			handle_ = NULL;
		}
	}

	void Semaphore::SetInfo()
	{
		OType(Object::kSemaphore);
		OName(Object::Type2Name(OType()));
	}

	int Semaphore::Post(int count)
	{
		if (::ReleaseSemaphore(handle_, (LONG)count, &value_) == TRUE)
			return 0;
		else
			return -1;
	}

	int Semaphore::Wait()
	{
		if (::WaitForSingleObject(handle_, INFINITE) == WAIT_OBJECT_0)
			return 0;
		else
			return -1;
	}

	int Semaphore::TimedWait(int timeout)
	{
		if (::WaitForSingleObject(handle_, timeout) == WAIT_OBJECT_0)
			return 0;

		DWORD errcode = ::GetLastError();
		if (errcode == WAIT_TIMEOUT)
			errno = ETIMEDOUT;
		return -1;			
	}

	int Semaphore::Value() const
	{
		return (int)::InterlockedCompareExchange((volatile LONG *)&value_, 0, 0);
	}

}

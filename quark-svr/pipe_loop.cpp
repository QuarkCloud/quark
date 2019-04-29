
#include "pipe_loop.h"

PipeLoop::PipeLoop()
{
	terminated_ = false;
	finaled_ = false;
	handle_ = INVALID_HANDLE_VALUE;
	guard_ = SRWLOCK_INIT;
}

PipeLoop::~PipeLoop()
{
	Final();
}

bool PipeLoop::Init()
{
	::AcquireSRWLockExclusive(&guard_);
	HANDLE iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (iocp != INVALID_HANDLE_VALUE)
	{
		handle_ = iocp;
	}

	::ReleaseSRWLockExclusive(&guard_);
	return (iocp != INVALID_HANDLE_VALUE);
}

void PipeLoop::Final()
{
	::AcquireSRWLockExclusive(&guard_);
	if (finaled_ == false)
	{
		finaled_ = true;
		terminated_ = true;
		::CloseHandle(handle_);
	}
	::ReleaseSRWLockExclusive(&guard_);
}

void PipeLoop::Terminate()
{
	terminated_ = true;
}

bool PipeLoop::Bind(HANDLE handle, PipeBase * obj)
{
	if (obj == NULL || obj->Valid() == false)
		return false;

	bool result = false;
	::AcquireSRWLockShared(&guard_);

	if (::CreateIoCompletionPort(handle , handle_, (ULONG_PTR)obj, 0) == handle_)
		result = true;
	::ReleaseSRWLockShared(&guard_);

	return result;
}

bool PipeLoop::Wait(DWORD& bytes, PipeBase* & obj, PipeOvlp *& ovlp, int timeout)
{
	OVERLAPPED * povlp = NULL;
	ULONG_PTR key = 0;
	DWORD msec = 0;
	if (timeout < 0)
		msec = INFINITE;
	else if (timeout > 0)
		msec = timeout * 1000;

	bytes = 0;
	obj = NULL;
	ovlp = NULL;

	bool result = false;
	::AcquireSRWLockShared(&guard_);

	if (::GetQueuedCompletionStatus(handle_, &bytes, &key, &povlp, msec) == TRUE)
	{
		if (key != 0)
			obj = (PipeBase *)key;
		if (povlp != NULL)
			ovlp = (PipeOvlp *)povlp;

		result = true;
	}
	else
	{
		DWORD errcode = ::GetLastError();
	}

	::ReleaseSRWLockShared(&guard_);
	return result;
}


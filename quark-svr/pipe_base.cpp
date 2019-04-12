

#include "pipe_base.h"
#include "pipe_util.h"

PipeBase::PipeBase()
{
	type_ = kPipeConnection;
	ref_counter_ = 0;
}

PipeBase::~PipeBase()
{
	//
}

long PipeBase::Refer()
{
	return ::InterlockedIncrement(&ref_counter_);
}

long PipeBase::Refer() const
{
	return ::InterlockedIncrement(&ref_counter_);
}


long PipeBase::Derefer()
{
	return ::InterlockedDecrement (&ref_counter_);
}

long PipeBase::Derefer() const
{
	return ::InterlockedDecrement(&ref_counter_);
}

PipeOvlp::PipeOvlp()
{
	::memset(&ovlp_, 0, sizeof(ovlp_));
	ovlp_.hEvent = ::CreateEventA(NULL, TRUE, FALSE, NULL);
}

PipeOvlp::~PipeOvlp()
{
	if (ovlp_.hEvent != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(ovlp_.hEvent);
		ovlp_.hEvent = INVALID_HANDLE_VALUE;
	}
}

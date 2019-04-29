
#include "pipe_spinlock.h"


PipeSpinlock::PipeSpinlock()
{
	token_ = 0;
	lock_ = 1 ;
}

PipeSpinlock::~PipeSpinlock()
{
	//
}

void PipeSpinlock::Lock()
{
	long token = ::InterlockedIncrement(&token_);
	size_t counter = 0;
	while (token_ != lock_)
	{
		++counter;
		if (counter >= 1024)
		{
			counter = 0;
			::SwitchToThread();
		}
	}
}

void PipeSpinlock::Unlock()
{
	::InterlockedIncrement(&lock_);
}



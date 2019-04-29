
#ifndef __QUARK_PIPE_SPINLOCK_H
#define __QUARK_PIPE_SPINLOCK_H 1

#include <windows.h>

class PipeSpinlock{
public:
	PipeSpinlock();
	~PipeSpinlock();

	void Lock();
	void Unlock();

private:
	PipeSpinlock(const PipeSpinlock&);
	volatile long token_;
	volatile long lock_;
};

#endif // ! __QUARK_PIPE_SPINLOCK_H



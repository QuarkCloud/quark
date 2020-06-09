
#include "wobjs/Condition.h"

namespace qkc {


	Condition::Condition()
	{
		//
	}

	void Condition::SetInfo()
	{
		OType(Object::kCondition);
	}

	Condition::	~Condition()
	{
		//
	}

	int Condition::Notify()
	{
		Mutex::ScopedLock locker(guard_);
		return sema_.Post();
	}

	int Condition::Broadcast()
	{
		Mutex::ScopedLock locker(guard_);
		int count = sema_.Value();
		return sema_.Post(count);
	}

	int Condition::Wait(Mutex& locker)
	{
		int result = 0;
		locker.Unlock();
		result = sema_.Wait();
		locker.Lock();
		return result;
	}

	int Condition::TimedWait(Mutex& locker, int timeout)
	{
		int result = 0;
		locker.Unlock();
		result = sema_.TimedWait(timeout);
		locker.Lock();
		return result;
	}
}

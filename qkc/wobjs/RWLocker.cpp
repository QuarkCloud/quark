
#include "wobjs/RWLocker.h"

namespace qkc {

	RWLocker::RWLocker()
	{
		locker_ = SRWLOCK_INIT;
	}
	RWLocker::~RWLocker()
	{
		//
	}

	void RWLocker::Lock()
	{
		::AcquireSRWLockExclusive(&locker_);
	}

	void RWLocker::Unlock()
	{
		::ReleaseSRWLockExclusive(&locker_);
	}

	void RWLocker::LockShared()
	{
		::AcquireSRWLockShared(&locker_);
	}

	void RWLocker::UnlockShared()
	{
		::ReleaseSRWLockShared(&locker_);
	}
}


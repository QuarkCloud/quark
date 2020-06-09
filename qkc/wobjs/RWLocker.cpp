
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

	void RWLocker::SetInfo()
	{
		OType(Object::kRWLocker);
	}

	void RWLocker::Lock()
	{
		::AcquireSRWLockExclusive(&locker_);
	}

	void RWLocker::Unlock()
	{
		::ReleaseSRWLockExclusive(&locker_);
	}

	void RWLocker::LockShared() const
	{
		::AcquireSRWLockShared(&locker_);
	}

	void RWLocker::UnlockShared() const
	{
		::ReleaseSRWLockShared(&locker_);
	}
}


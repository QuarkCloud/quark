
#include <quark_compile.h>
#include <time.h>
#include <windows.h>
#include "wobjs/RWLocker.h"
#include "wobjs/DateTime.h"
#include "wobjs/ObjMgr.h"
#include "wimpl/PThreadRWLocker.h"

namespace qkc {


	PThreadRWLocker::PThreadRWLocker()
	{
		::InitializeSRWLock(&rwlocker_);
		writers_ = 0;
		readers_ = 0;	

		ObjMgr::Singleton().Add(&rwlocker_, this);
	}

	PThreadRWLocker::~PThreadRWLocker()
	{
		Mutex::ScopedLock locker(*this);

		//	
	}

	int PThreadRWLocker::RdLock()
	{
		bool owner = false;
		while (owner == false)
		{
			Lock();

			if (writers_ == 0)
			{
				owner = true;
				readers_++;
			}

			Unlock();
		}

		if (owner == true)
			::AcquireSRWLockShared(&rwlocker_);

		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::TryRdLock()
	{
		bool owner = false;
		if (TryLock() != 0)
			return -1;

		if (writers_ == 0)
		{
			owner = true;
			readers_++;
		}

		Unlock();

		if (owner == true)
			::AcquireSRWLockShared(&rwlocker_);
	
		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::TimedRdLock(int msec)
	{
		if (msec <= 0)
			return TryRdLock();

		bool owner = false;

		int end_time = (int)::GetTickCount() + msec;
		int left_time = 0;

		while ((left_time = end_time - (int)::GetTickCount()) > 0)
		{
			if (TimedLock(left_time) != 0)
				return -1;

			if (writers_ == 0)
			{
				owner = true;
				readers_++;
				break;
			}

			Unlock();
		}

		if(owner == true)
			::AcquireSRWLockShared(&rwlocker_);
	
		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::WrLock()
	{
		bool owner = false;
		while (owner == false)
		{
			Lock();

			if (readers_ == 0)
			{
				owner = true;
				writers_++;
			}

			Unlock();
		}

		if (owner == true)
			::AcquireSRWLockExclusive(&rwlocker_);

		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::TryWrLock()
	{
		bool owner = false;
		if (TryLock() != 0)
			return -1;

		if (writers_ == 0)
		{
			owner = true;
			readers_++;
		}

		Unlock();

		if (owner == true)
			::AcquireSRWLockExclusive(&rwlocker_);

		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::TimedWrLock(int msec)
	{
		if (msec <= 0)
			return TryWrLock();

		bool owner = false;

		int end_time = (int)::GetTickCount() + msec;
		int left_time = 0;

		while ((left_time = end_time - (int)::GetTickCount()) > 0)
		{
			if (TimedLock(left_time) != 0)
				return -1;

			if (readers_ == 0)
			{
				owner = true;
				writers_++;
				break;
			}

			Unlock();
		}

		if (owner == true)
			::AcquireSRWLockExclusive(&rwlocker_);

		return (owner ? 0 : -1);
	}

	int PThreadRWLocker::RWUnLock()
	{
		Mutex::ScopedLock locker(*this);

		if (readers_ != 0)
		{
			::ReleaseSRWLockShared(&rwlocker_);
			--readers_;
		}
		else if (writers_ != 0)
		{
			::ReleaseSRWLockExclusive(&rwlocker_);
			--writers_;
		}

		return 0;	
	}

	void PThreadRWLocker::SetInfo()
	{
		OType(Object::kPthreadRWLock);
	}

	static RWLocker __pthread_rwlocker_inited_guard__;
	bool EnsurePThreadRWLockerInited(int * index)
	{
		if (index == NULL)
			return false;

		if (*index == 0)
		{
			__pthread_rwlocker_inited_guard__.Lock();
			if (*index == 0)
			{
				PThreadRWLocker * rwlocker = new PThreadRWLocker();
				*index = rwlocker->OIndex();
			}
			__pthread_rwlocker_inited_guard__.Unlock();
		}

		return (*index != 0);
	}
	
}

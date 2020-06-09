
#include "wobjs/ThreadLocal.h"
#include "wobjs/ErrorMap.h"
#include <errno.h>
#include <string.h>

namespace qkc {

	ThreadLocal::ThreadLocal(DataFreeFunc f)
	{
		tls_key_ = ::TlsAlloc();
		func_ = f;
		index_ = -1;
		if (tls_key_ != TLS_OUT_OF_INDEXES)
			index_ = ThreadLocalLogger::Singleton().Add(this);
	}

	ThreadLocal::~ThreadLocal()
	{
		DWORD key = tls_key_;
		tls_key_ = TLS_OUT_OF_INDEXES;

		if (key != TLS_OUT_OF_INDEXES)
		{
			ThreadLocal * tls = NULL;
			ThreadLocalLogger::Singleton().DelByKey(key , tls);
		}
		index_ = -1;
	}

	bool ThreadLocal::SetData(const void * data)
	{
		if (tls_key_ == TLS_OUT_OF_INDEXES)
			return false;

		if (::TlsSetValue(tls_key_, (void *)data) == TRUE)
			return true;

		errno = oserr_map(::GetLastError());
		return false;
	}

	void * ThreadLocal::GetData()
	{
		return (void *)::TlsGetValue(tls_key_);
	}

	void ThreadLocal::Clear()
	{
		if (func_ != NULL)
		{
			void * data = GetData();
			if (data != NULL)
				func_(data);
		}	
	}

	ThreadLocalLogger::ThreadLocalLogger()
	{
		::memset(nodes_, 0, sizeof(nodes_));
		index_ = 0;	
	}

	ThreadLocalLogger::~ThreadLocalLogger()
	{
		Mutex::ScopedLock locker(guard_);
		//
	}


	int ThreadLocalLogger::Add(ThreadLocal * data)
	{
		Mutex::ScopedLock locker(guard_);
		if (data == NULL)
			return -1;
		int index = index_;
		for (int times = 0; times < kMaxTLSKeySize; ++times)
		{
			if (index >= kMaxTLSKeySize)
				index = 0;
			if (nodes_[index] == NULL)
			{
				nodes_[index] = data;
				index_ = index + 1;
				return index;
			}
		}
		return -1;
	}

	void ThreadLocalLogger::DelByKey(DWORD key, ThreadLocal*& data)
	{
		Mutex::ScopedLock locker(guard_);
		for (int i = 0; i < kMaxTLSKeySize; ++i)
		{
			ThreadLocal * tls = nodes_[i];
			if (tls == NULL || tls->Key() != key)
				continue;

			nodes_[i] = NULL;
			if (data != NULL)
				data = tls;
		}
	}

	void ThreadLocalLogger::DelByIndex(int index, ThreadLocal*& data)
	{
		Mutex::ScopedLock locker(guard_);
		if (index < 0 || index >= kMaxTLSKeySize)
			return;
		if(data != NULL)
			data = nodes_[index];
		nodes_[index] = NULL;	
	}

	ThreadLocal * ThreadLocalLogger::GetByKey(DWORD key)
	{
		Mutex::ScopedLock locker(guard_);
		for (int i = 0; i < kMaxTLSKeySize; ++i)
		{
			ThreadLocal * data = nodes_[i];
			if (data == NULL)
				continue;

			if (data->Key() == key)
				return data;
		}
		return NULL;
	}

	ThreadLocal * ThreadLocalLogger::GetByIndex(int index)
	{
		Mutex::ScopedLock locker(guard_);

		if (index < 0 || index >= kMaxTLSKeySize)
			return NULL;

		return nodes_[index];	
	}

	void ThreadLocalLogger::ThreadClear()
	{
		Mutex::ScopedLock locker(guard_);
		for (int i = 0; i < kMaxTLSKeySize; ++i)
		{
			ThreadLocal * data = nodes_[i];
			if (data == NULL)
				continue;

			data->Clear();
		}
	}

	ThreadLocalLogger& ThreadLocalLogger::Singleton()
	{

		static ThreadLocalLogger __thread_local_logger__;
		return __thread_local_logger__;	
	}

	void * TlsGetData(int key)
	{
		return ::TlsGetValue((DWORD)key);
	}
}


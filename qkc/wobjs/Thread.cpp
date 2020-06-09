
#include "wobjs/Thread.h"
#include "wobjs/ObjMgr.h"
#include "wobjs/ErrorMap.h"
#include "wobjs/Mutex.h"

#include <errno.h>
#include <string.h>

namespace qkc {

	ThreadInfo::ThreadInfo()
	{
		thread_id_ = 0;
		errno_ = 0;
		random_number_ = 0;
		owner_ = NULL;
	}

	ThreadInfo::~ThreadInfo()
	{
		//
	}

	void ThreadInfo::Assign(const ThreadInfo& info)
	{
		thread_id_ = info.thread_id_;
		errno_ = info.errno_;
		random_number_ = info.random_number_;
		owner_ = info.owner_;
	}

	ThreadInfo& ThreadInfo::operator=(const ThreadInfo& info)
	{
		Assign(info);
		return (*this);
	}


	DWORD WINAPI WinThreadFunction(LPVOID lpParam);
	static ThreadLocal __default_thread_info_tls__;

	Thread::Thread()
	{
		worker_ = NULL;
		self_ = this;
		terminated_ = false;
		inited_ = false;
		finaled_ = false;
		exit_code_ = 0;
	}

	Thread::~Thread()
	{
		Final();
	}

	bool Thread::Init()
	{
		Mutex::ScopedLock locker(locker_);
		if (inited_ == true)
			return true;

		DWORD tid = 0;
		HANDLE handle = ::CreateThread(NULL, 0, WinThreadFunction, this, 0, &tid);
		if (handle == NULL || handle == INVALID_HANDLE_VALUE)
			return false;

		cond_.Wait(locker_);

		worker_ = handle;
		inited_ = true;

		if (ObjMgr::Singleton().Add(handle, this) < 0)
		{
			Terminate();
			return false;
		}

		return true;
	}
	
	void Thread::Final()
	{
		Mutex::ScopedLock locker(locker_);

		if (inited_ == false || finaled_ == true)
			return;

		finaled_ = true;

		//关闭系统线程
		if (worker_ != NULL)
		{
			Terminate();
			HANDLE wthr = worker_;
			worker_ = NULL;
			::WaitForSingleObject(wthr , INFINITE);
			::CloseHandle(wthr);
		}

		//清理线程信息
		ThreadInfo * info = Thread::DefaultInfo();
		if (info != NULL)
		{
			__default_thread_info_tls__.SetData(NULL);
		}	
	}

	void Thread::Run()
	{
		//
	}

	void Thread::Wait()
	{
		if (worker_ == NULL || worker_ == INVALID_HANDLE_VALUE)
			return;

		DWORD ret = ::WaitForSingleObject(worker_ , INFINITE) ;
		if (ret == WAIT_OBJECT_0)
		{
				DWORD exit_code = 0;
				if (::GetExitCodeThread(worker_, &exit_code) == TRUE)
					exit_code_ = (int)exit_code;
		}
	}

	void Thread::Terminate()
	{
		terminated_ = true;
	}

	void Thread::SetInfo()
	{
		Object::OType(Object::kThread);
	}

	ThreadInfo * Thread::DefaultInfo()
	{
		return (ThreadInfo *)__default_thread_info_tls__.GetData();
	}

	DWORD WINAPI WinThreadFunction(LPVOID lpParam)
	{
		Thread * thr = (Thread *)lpParam;
		ThreadInfo & info = thr->Info();
		info.Owner(thr);
		info.ThreadID(::GetCurrentThreadId());
		__default_thread_info_tls__.SetData(&info);
		thr->Run();
		ThreadLocalLogger::Singleton().ThreadClear();
		return 0;
	}
}

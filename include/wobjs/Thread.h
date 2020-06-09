
#ifndef __QUARK_WOBJS_THREAD_H
#define __QUARK_WOBJS_THREAD_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"
#include "wobjs/Condition.h"
#include "wobjs/Mutex.h"
#include "wobjs/ThreadLocal.h"

namespace qkc {
	class Thread;

	class QKCAPI ThreadInfo {
	public :
		ThreadInfo();
		virtual ~ThreadInfo();

		void Assign(const ThreadInfo& info);
		ThreadInfo& operator=(const ThreadInfo& info);

		inline int ErrNo() const { return errno_; }
		inline int RandomNumber() const { return random_number_; }
		inline const Thread * Owner() const { return owner_; }
		inline void Owner(Thread* thr) { owner_ = thr; }
		inline DWORD ThreadID() const { return thread_id_; }
		inline void ThreadID(DWORD tid) { thread_id_ = tid; }

	private:
		DWORD thread_id_;	//操作系统分配的线程号
		int errno_;
		int random_number_;
		Thread * owner_;
	};


	class QKCAPI Thread : public Object{
	public:
		Thread();
		~Thread();

		bool Init();
		void Final();

		virtual void Run();
		void Wait();

		inline bool Terminated() const { return terminated_; }
		void Terminate();
		inline bool Finaled() const { return finaled_; }
		inline ThreadInfo& Info() { return info_; }
		inline const ThreadInfo& Info() const { return info_; }
		inline void Info(const ThreadInfo& info) { info_ = info; }
		inline int ExitCode() const { return exit_code_; }

		static ThreadInfo * DefaultInfo();

	protected:
		virtual void SetInfo();
	private:
		HANDLE worker_;		//系统分配的工作线程
		ThreadInfo info_;
		Thread * self_;
		bool terminated_;
		bool inited_;
		bool finaled_;
		Condition cond_;
		Mutex locker_;
		int exit_code_;
	};

}

#endif  /** __QUARK_WOBJS_THREAD_H */

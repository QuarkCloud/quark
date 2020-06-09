
#ifndef __QUARK_WOBJS_THREAD_LOCAL_H
#define __QUARK_WOBJS_THREAD_LOCAL_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"
#include "wobjs/Condition.h"
#include "wobjs/Mutex.h"

namespace qkc {

	class QKCAPI ThreadLocal {
	public:
		typedef void(*DataFreeFunc)(void * data);
		ThreadLocal(DataFreeFunc f = NULL);
		virtual ~ThreadLocal();

		bool SetData(const void * data);
		void * GetData();
		void Clear();

		inline DWORD Key() const { return tls_key_; }
		inline bool Validate() const { return (tls_key_ == TLS_OUT_OF_INDEXES); }
		inline int Index() const { return index_; }
	private:
		DWORD tls_key_;
		DataFreeFunc func_;
		int index_;
	};

	class QKCAPI ThreadLocalLogger {
	public:
		ThreadLocalLogger();
		virtual ~ThreadLocalLogger();

		int Add(ThreadLocal * data);

		void DelByKey(DWORD key , ThreadLocal*& data);
		void DelByIndex(int index, ThreadLocal*& data);

		ThreadLocal * GetByKey(DWORD key);
		ThreadLocal * GetByIndex(int index);

		void ThreadClear();

		static ThreadLocalLogger& Singleton();
		static const int kMaxTLSKeySize = 64;
	private:
		ThreadLocal * nodes_[kMaxTLSKeySize];
		Mutex guard_;
		int index_;
	};

	QKCAPI void * TlsGetData(int key);
}

#endif  /** __QUARK_WOBJS_THREAD_LOCAL_H */

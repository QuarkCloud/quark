
#ifndef __QUARK_WOBJS_IOCP_MGR_H
#define __QUARK_WOBJS_IOCP_MGR_H 1

#include <quark_compile.h>
#include <stdint.h>
#include "wobjs/MemMgr.h"
#include "wobjs/ObjMgr.h"
#include "wobjs/RWLocker.h"
#include "wobjs/FileSystem.h"
#include "wobjs/AddrMgr.h"
#include "wobjs/AtomicCounter.h"

namespace qkc {

	class QKCAPI IOCPItem : public FileCallback {
	public:
		IOCPItem();
		virtual ~IOCPItem();

		inline HANDLE Handle() { return handle_; }
		inline void Events(uint32_t events) { events_ = events; }
		inline uint32_t Events() { return events_ ; }
	private:
		int fd_;
		HANDLE handle_;
		uint32_t events_;
		void * data_;
	};

	class QKCAPI IOCPMgr : public Object {
	public:
		IOCPMgr();
		virtual ~IOCPMgr();

		bool Init(DWORD backlog);
		void Final();

		bool Add(IOCPItem * item , uint32_t events);
		//仅仅从管理器中删除，并没有接触绑定。
		bool Del(IOCPItem * item);
		bool Modify(IOCPItem * item, uint32_t events);

		//timeout的单位为毫秒
		int Wait(IOCPItem **items, int max_size, int timeout);

	private:
		HANDLE iocp_;
		mutable RWLocker guard_;
		AddrMgr items_;
		AtomicCounter thread_count_;
	};
}

#endif  /** __QUARK_WOBJS_IOCP_MGR_H */

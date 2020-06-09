
#ifndef __QUARK_WOBJS_IOCP_MGR_H
#define __QUARK_WOBJS_IOCP_MGR_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/epoll.h>
#include <winsock2.h>

#include "wobjs/MemMgr.h"
#include "wobjs/Mutex.h"
#include "wobjs/RList.h"

namespace qkc {

	/**
		2018-08-29
		�����ܵ���֧���첽��д���������ܵ���socketû�б��ʵ����������iocp�в�֧�ֹܵ�������
		2019-03-29
		Ϊ��ʵ��Linux�µĹܵ���ʹ�������ܵ���ģ�������ܵ���ͬʱ֧���첽�������ܹ���IOCP֧�֡�
		֮����Ҫ֧�ֹܵ�������Ϊlibuv��Ҫ֧�ֹܵ���Ϊ�źŴ�������
	*/
	class IOCPItem;
	class IOCPMgr;
	class QKCAPI IOCPOvlp {
	public:
		IOCPOvlp();
		~IOCPOvlp();

		bool Init();
		void Final();

		bool Lock();
		bool Unlock();

		static const int kVoid		= 0;
		static const int kInput		= 1;
		static const int kOutput	= 2;

		inline OVERLAPPED * Ovlp() { return &ovlp_; }
		inline int Status() const { return status_; }
		inline void Status(int status) { status_ = status; }
		inline int Type() const { return type_; }
		inline LONG Counter() const { return counter_; }
		inline const IOCPItem * Owner() const { return owner_; }
		inline IOCPItem * Owner() { return owner_; }
		inline void Owner(IOCPItem * owner) { owner_ = owner; }
	private:
		OVERLAPPED ovlp_;
		int status_;
		int type_;
		volatile LONG counter_;
		IOCPItem	*owner_;
	};

	class QKCAPI IOCPItem : public RLNode {
	public:
		IOCPItem();
		virtual ~IOCPItem();

		static const int kTypeUnknown	= 0;
		static const int kTypeSocket	= 1;
		static const int kTypeFile		= 2;
		static const int kTypePipe		= 3;

		static const int kEvtOpen	= 1;
		static const int kEvtClose	= 2;
		static const int kEvtRead	= 3;
		static const int kEvtWrite	= 4;

		typedef int(*CallBack_t)(IOCPItem * item, int evt, int result);

		inline void CallBack(CallBack_t cb) { callback_ = cb; }

	private:
		int fd_;
		int type_;
		struct epoll_event data_;
		uint32_t mask_;
		IOCPMgr * owner_;
		CallBack_t callback_;
	};

	class QKCAPI IOCPMgr : public Object {
	public:
		IOCPMgr();
		~IOCPMgr();

		int Init();
		void Final();

		bool Add(int fd, struct epoll_event * ev);
		bool Modify(int fd, struct epoll_event * ev);
		bool Del(int fd, struct epoll_event * ev);

		int Wait(int timeout);
		int Retrieve(struct epoll_event * evs, int max_count);
	protected:
		void SetInfo();
	private:
		void FreeItems();
		int epfd_;
		bool finaled_;
		HANDLE iocp_;
		Mutex	guard_;
		RList readies_;					//�Ѿ�������
		int ready_counter_;
		volatile LONG thread_counter_;	//������߳���
	};


}

#endif  /** __QUARK_WOBJS_IOCP_MGR_H */

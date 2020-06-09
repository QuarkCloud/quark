
#ifndef __QUARK_WOBJS_INOTIFY_MGR_H
#define __QUARK_WOBJS_INOTIFY_MGR_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <windows.h>

#include "wobjs/RBTree.h"
#include "wobjs/MemMgr.h"
#include "wobjs/Mutex.h"

namespace qkc {

	/**
	FindFirstChangeNotification���ܹ���ʾ����仯�������ReadDirectoryChangesW
	2018-05-29
	����ʵ��ԭ�ͣ��ȴ������������ơ�����inotify��Ӧ��������

	2019-11-23
	�޸�ΪC++ģ�ͣ�������������ΪC++���RBTree��
	*/
	class INotifyItem ;
	class INotifyMgr;

	class QKCAPI INotifyOvlp {
	public:
		INotifyOvlp();
		~INotifyOvlp();

		bool Init();
		void Final();
		void Owner(INotifyItem * owner);

		inline OVERLAPPED * Ovlp() { return &ovlp_; }
		inline INotifyItem * Owner() { return owner_; }
		inline char * Buffer() { return buffer_; }
		inline const char * Buffer() const { return buffer_; }
		inline size_t BufSize() const { return bufsize_; }
		inline void BufSize(size_t size) { bufsize_ = size; }

		static const size_t kOvlpBufferSize = 1024;
	private:
		OVERLAPPED ovlp_;
		bool inited_;
		INotifyItem * owner_;
		char * buffer_;
		size_t bufsize_;
		char cache_[kOvlpBufferSize];
	};

	/**
	2019-11-23
	֮���Խ�Ovlp��Item���룬����ΪItem�����Ǳ�ȻҪ����Ovlp��ֻ�е������������¼��������ʱ������Ҫ��
	*/
	class QKCAPI INotifyItem : public RBNode {
	public:
		INotifyItem();
		virtual ~INotifyItem();

		bool Init(const char * name , int mask);
		void Final();

		inline int Key() const { return (data_ == NULL)? 0 : data_->wd ;}
		inline void Key(int key) { if (data_ != NULL) data_->wd = key;}

		inline HANDLE Handle() { return handle_; }
		inline void Handle(HANDLE handle) { handle_ = handle; }
		inline uint32_t Occur() const { return occur_; }
		inline INotifyMgr * Owner() { return owner_; }
		inline void Owner(INotifyMgr * owner) { owner_ = owner; }
		inline INotifyOvlp * Ovlp() { return ovlp_; }
		inline void Ovlp(INotifyOvlp * ovlp) { ovlp_ = ovlp; }
		inline struct inotify_event * Data() { return data_; }

	private:
		bool OpenNotifyFile(const char * name);

		HANDLE handle_;
		uint32_t occur_;	//�Ѿ��������¼�
		INotifyMgr * owner_;
		INotifyOvlp * ovlp_;
		struct inotify_event * data_;
	};

	/**
		�ȼ���std::map<int , INotifyItem *>
	*/
	class QKCAPI INotifyItems : public RBTree {
	public:
		INotifyItems();
		~INotifyItems();

		bool Insert(INotifyItem * item);
		bool Find(int wd, INotifyItem *& item);
		bool Delete(int wd, INotifyItem *& item);

		virtual int Compare(const RBNode * src, const RBNode * dst) const;
	private:
		virtual int CompareNode(const INotifyItem * src, const INotifyItem * dst) const;
	};

	class QKCAPI INotifyMgr : public Object {
	public:
		INotifyMgr();
		~INotifyMgr();

		int Init();
		void Final();

		int Add(const char * name, uint32_t mask);
		bool Del(int wd);
		INotifyItem * Find(int wd);

		inline bool Finaled() const { return finaled_; }

	protected:
		void SetInfo();		
	private:
		HANDLE iocp_;
		int handle_;
		int count_;
		int last_id_;
		bool finaled_;
		Mutex guard_;
		INotifyItems items_;
	};


}

#endif  /** __QUARK_WOBJS_INOTIFY_MGR_H */

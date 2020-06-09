
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
	FindFirstChangeNotification不能够提示具体变化情况，用ReadDirectoryChangesW
	2018-05-29
	初步实现原型，等待后续继续完善。根据inotify的应用来调整

	2019-11-23
	修改为C++模型，将搜索容器改为C++版的RBTree。
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
	之所以将Ovlp和Item分离，是因为Item并不是必然要关联Ovlp。只有当关联到其他事件管理机制时，才需要。
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
		uint32_t occur_;	//已经触发的事件
		INotifyMgr * owner_;
		INotifyOvlp * ovlp_;
		struct inotify_event * data_;
	};

	/**
		等价于std::map<int , INotifyItem *>
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

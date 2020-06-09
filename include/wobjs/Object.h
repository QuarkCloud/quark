
#ifndef __QUARK_WOBJS_OBJECT_H
#define __QUARK_WOBJS_OBJECT_H 1

#include <quark_compile.h>
#include <stdint.h>

namespace qkc {
	class QKCAPI Object {
	public:
		Object();
		virtual ~Object();

		inline int OID() const { return id_; }
		inline int OType() const { return type_; }
		void OType(int type);
		inline int OSubType() const { return subtype_; }
		inline void OSubType(int subtype) { subtype_ = subtype; }
		inline const char * OName() const { return name_; }
		inline const int OIndex() const { return index_; }

		inline bool IsProcess() const { return type_ == kProcess; }
		inline bool IsThread() const { return type_ == kThread; }
		inline bool IsFile() const { return type_ == kFile; }
		inline bool IsModule() const { return type_ == kModule; }

		inline bool IsMutex() const { return type_ == kMutex; }
		inline bool IsSemaphore() const { return type_ == kSemaphore; }
		inline bool IsEvent() const { return type_ == kEvent; }
		inline bool IsSocket() const { return type_ == kSocket; }

		inline bool IsShareMemory() const { return type_ == kShareMemory; }
		inline bool IsFileWatcher() const { return type_ == kFileWatcher; }
		inline bool IsIOCP() const { return type_ == kIOCP; }
		inline bool IsRWLocker() const { return type_ == kRWLocker; }

		inline bool IsSpinLocker() const { return type_ == kSpinLocker; }
		inline bool IsCondition() const { return type_ == kCondition; }
		inline bool IsPipe() const { return type_ == kPipe; }
		inline bool IsPipeAlias() const { return type_ == kPipeAlias; }
		inline bool IsPThreadMutex() const { return type_ == kPthreadMutex; }
		inline bool IsPThreadRWLock() const { return type_ == kPthreadRWLock; }
		inline bool IsPThreadCond() const { return type_ == kPthreadCond; }

		static int AllocID();
		static bool IsValidType(int type);

		static const int kVoid = 0;
		static const int kOther	= 1;
		static const int kProcess = 2;
		static const int kThread = 3;
		static const int kFile = 4;
		static const int kModule = 5;
		static const int kMutex	= 6;
		static const int kSemaphore = 7;
		static const int kEvent	= 8;
		static const int kSocket= 9;
		static const int kShareMemory = 10;
		static const int kFileWatcher = 11;
		static const int kIOCP = 12;
		static const int kRWLocker = 13;
		static const int kSpinLocker = 14;
		static const int kCondition = 15;

		static const int kPipe = 20;
		static const int kPipeAlias = 21;

		static const int kPthreadMutex = 22;		//主要为pthread_mutex使用，解决pthread_cond和SRWLock的兼容问题
		static const int kPthreadRWLock = 23;	
		static const int kPthreadCond = 24;

		static const int kMaxType = 32;

		static const char * kEmptyName;
		static const char * Type2Name(int type);

		friend class ObjMgr;
	protected:
		virtual void SetInfo();
	private:
		int id_;
		int type_;
		int subtype_;	//由子类定义。
		int index_;
		uintptr_t wdata_;
		const char * name_;
	};

	class QKCAPI SafeObj : public Object {
	public:
		SafeObj();
		virtual ~SafeObj();

		long Refer()const;
		long Defer() const;

	private:
		mutable volatile long refcounter_;
	};

	class QKCAPI SafeObjRef {
	public:
		SafeObjRef();
		SafeObjRef(const SafeObjRef& ref);
		SafeObjRef(const SafeObj * obj);
		virtual ~SafeObjRef();

		SafeObjRef& operator=(const SafeObj * obj);
		void Assign(const SafeObj * obj);
		void Clear();

		inline SafeObj * Object() { return object_; }
		inline const SafeObj * Object() const { return object_; }
		inline bool Valid() const { return (object_ != NULL); }
		inline SafeObj * operator->() { return object_; }
		inline const SafeObj * operator->() const { return object_; }
	protected:
		mutable SafeObj * object_;
		void CheckToFree(const SafeObj * obj);
	};

	template<class T>
	class SafeObjAccess : private SafeObjRef {
	public:
		SafeObjAccess() { object_ = NULL; }
		SafeObjAccess(const SafeObjAccess& access) { object_ = NULL; Assign(Object()); }
		SafeObjAccess(const T * obj) { object_ = NULL; Assign(obj);}
		virtual ~SafeObjAccess() {}

		inline T * Object() { return (T*)object_; }
		inline const T * Object() const { return (const T *)object_; }
		inline bool Valid() const { return (object_ != NULL); }
		inline T * operator->() { return (T *)object_; }
		inline const T * operator->() const { return (const T *)object_; }
	};
}

#endif  /** __QUARK_WOBJS_OBJECT_H */

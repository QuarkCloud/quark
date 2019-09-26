
#ifndef __QUARK_WOBJS_OBJECT_H
#define __QUARK_WOBJS_OBJECT_H 1

#include <quark_compile.h>

namespace qkc {
	class QKCAPI Object {
	public:
		Object();
		virtual ~Object();

		inline int OID() const { return oid_; }
		inline void OID(int oid) { oid_ = oid; }
		inline int OType() const { return type_; }
		inline void OType(int type) { type_ = type; }
		inline const char * OName() const { return name_; }
		inline void OName(const char * name) { name_ = name; }

		static int AllocID();

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
		static const int kRWLock = 13;
		static const int kPipe = 14;
		static const int kPipeAlias = 15;
	private:
		int oid_;
		int type_;
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

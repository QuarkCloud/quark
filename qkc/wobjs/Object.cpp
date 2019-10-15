
#include "wobjs/Object.h"
#include <windows.h>

namespace qkc {
	Object::Object()
	{
		oid_ = Object::AllocID();
		type_ = Object::kVoid;
		name_ = NULL;

		SetInfo();
	}

	Object::~Object()
	{
		//
	}

	void Object::SetInfo()
	{
	
	}

	int Object::AllocID()
	{
		static volatile LONG __internal_object_id__ = 0;

		return (int)::InterlockedIncrement(&__internal_object_id__);	
	}

	const char * Object::kEmptyName = "";

	const char * Object::Type2Name(int type)
	{
		static const char * __object_type_names__[Object::kMaxType] = {
			"Void" , "Other" , "Process" , "Thread" , "File" , "Module" , "Mutex" , "Semaphore" ,       /** 0 ~ 7  */
			"Event" , "Socket" , "ShareMemory" , "FileWatcher" , "IOCP" , "RWLock" , "SpinLock" , "Condition" ,  /** 8 ~ 15 */
			"" ,"" , "" , "" , "Pipe" , "PipeAlias" , "" , "" ,		/** 16 ~ 23 */
			"" ,"" , "" , "" , "" , "" , "" , "" 		/** 24 ~ 31 */
		}; 

		if (type < 0 || type >= Object::kMaxType)
			return Object::kEmptyName;
		else
			return __object_type_names__[type];	
	}

	SafeObj::SafeObj()
	{
		refcounter_ = 0;
	}

	SafeObj::~SafeObj()
	{
		//
	}

	long SafeObj::Refer()const
	{
		return ::InterlockedIncrement(&refcounter_);
	}

	long SafeObj::Defer() const
	{
		return ::InterlockedDecrement(&refcounter_);
	}

	SafeObjRef::SafeObjRef()
	{
		object_ = NULL;
	}
	SafeObjRef::SafeObjRef(const SafeObjRef& ref)
	{
		Assign(ref.Object());
	}

	SafeObjRef::SafeObjRef(const SafeObj * obj)
	{
		Assign(obj);
	}

	SafeObjRef::~SafeObjRef()
	{
		CheckToFree(object_);
	}

	SafeObjRef& SafeObjRef::operator=(const SafeObj * obj)
	{
		SafeObj * old_obj = object_;
		object_ = NULL;
		CheckToFree(old_obj);
		Assign(obj);
		return (*this);
	}

	void SafeObjRef::Assign(const SafeObj * obj)
	{
		if (obj != NULL)
			obj->Refer();

		const SafeObj * old = NULL;
		do{
			 old = object_;
		} while (::InterlockedCompareExchangePointer((PVOID volatile *)&object_, (void *)obj, (void *)old) != old);

		CheckToFree(old);
	}

	void SafeObjRef::Clear()
	{
		Assign(NULL);
	}

	void SafeObjRef::CheckToFree(const SafeObj * obj)
	{
		if (obj == NULL)
			return;
		long counter = obj->Defer();
		if (counter == 0)
			delete obj;
	}

}
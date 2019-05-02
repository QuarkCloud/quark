
#ifndef __QUARK_PIPE_BASE_H
#define __QUARK_PIPE_BASE_H 1

#include <windows.h>

class PipeBase {
public:
	PipeBase();
	virtual ~PipeBase();

	long Refer() ;
	long Refer() const;

	long Derefer();
	long Derefer() const;

	inline const int Type() const { return type_; }
	inline void Type(int type) {type_ = type; }
	inline HANDLE Handle() { return handle_; }
	inline void Handle(HANDLE handle) {handle_ = handle; }
	inline bool Valid() const { return (handle_ != INVALID_HANDLE_VALUE && handle_ != NULL); }

	static const int kPipeUnknown	= 0;
	static const int kPipeConnection= 1;
	static const int kPipeServer	= 2;
	static const int kPipeClient	= 3;
	static const int kPipeReader	= 4;
	static const int kPipeWriter	= 5;

private:
	HANDLE handle_;
	int type_;
	mutable volatile long ref_counter_ ;
};

template<class T>
class PipeAccess {
public:
	PipeAccess() { object_ = NULL; }
	PipeAccess(T * obj) { object_ = obj; }
	virtual ~PipeAccess()
	{
		T * obj = object_;
		object_ = NULL;
		CheckFreeObject(obj);
	}

	inline PipeAccess& operator=(T * obj) 
	{
		T * old = object_;
		if(obj != NULL)
			obj->Refer();
		object_ = obj;
		CheckFreeObject(old);
		return (*this);
	}

	inline T * Object() { return object_; }
	inline const T * Object() const { return object_; }
	inline bool Valid() const { return (object_ != NULL); }
	inline T * operator->() { return object_; }
	inline const T * operator->() const { return object_; }
private:
	T * object_;
	void CheckFreeObject(T * obj) 
	{
		if (obj == NULL)
			return;
		long counter = obj->Derefer();
		if (counter == 0)
			delete obj;
	}
};

class PipeOvlp : public  OVERLAPPED{
public:
	PipeOvlp();
	virtual ~PipeOvlp();

	inline PipeBase * Owner() { return owner_; }
	inline void Owner(PipeBase * owner) {owner_ = owner; }
private:
	PipeBase * owner_;
};

#endif // ! __QUARK_PIPE_BASE_H

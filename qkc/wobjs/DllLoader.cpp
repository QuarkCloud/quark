
#include "wobjs/DllLoader.h"
#include "wobjs/GlobalLocker.h"

namespace qkc {
	DllLoader::DllLoader()
	{
		handle_ = NULL;
		OType(Object::kModule);
	}

	DllLoader::~DllLoader()
	{
		Final();
		if (handle_ != NULL && handle_ != INVALID_HANDLE_VALUE)
		{
			Close();
		}
	}

	bool DllLoader::Init()
	{
		return false;
	}

	void DllLoader::Final()
	{
		//
	}

	bool DllLoader::Open(const char * name)
	{
		handle_ = ::LoadLibrary(name);
		return (handle_ != NULL);
	}

	void DllLoader::Close()
	{
		if (handle_ != NULL)
		{
			FreeLibrary(handle_);
			handle_ = NULL;
		}
	}

	FARPROC DllLoader::GetAddress(const char * name)
	{
		return ::GetProcAddress(handle_, name);
	}

	bool DllLoader::ConfirmLoad()
	{
		if (loaded_ == false)
		{
			guard_.Lock();
			if (loaded_ == false)
				loaded_ = Init();
			guard_.Unlock();
		}

		return loaded_;
	}

}

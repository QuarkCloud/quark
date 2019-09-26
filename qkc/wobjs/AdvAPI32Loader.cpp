
#include "wobjs/AdvAPI32Loader.h"
#include "wobjs/GlobalLocker.h"

namespace qkc {

	AdvAPI32Loader::AdvAPI32Loader()
	{
		lpfnGetUserName_ = NULL;
	}
	AdvAPI32Loader::~AdvAPI32Loader()
	{
		//
	}

	bool AdvAPI32Loader::Init()
	{
		bool result = false;
		if ((result = Open(AdvAPI32Loader::kDllName)) == true)
		{
			lpfnGetUserName_ = (LPFN_GETUSERNAMEW)this->GetAddress("GetUserNameW");
			lpfnOpenProcessToken_ = (LPFN_OPENPROCESSTOKEN)this->GetAddress("OpenProcessToken");
			Loaded(true);
		}
		return result;
	}

	void AdvAPI32Loader::Final()
	{
		Close();
	}

	int AdvAPI32Loader::GetUserName(char * name, int size)
	{
		if (ConfirmLoad() == false)
			return -1;

		int result = -1;
		if (lpfnGetUserName_ != NULL)
		{
			WCHAR username[256];
			DWORD username_size = 256;
			if (lpfnGetUserName_(username, &username_size) == TRUE)
			{
				result = ::WideCharToMultiByte(CP_UTF8, 0, username, username_size, name, size, NULL, NULL);
				if (result >= 0)
					name[result] = '\0';
			}
		}

		return result;
	}

	bool AdvAPI32Loader::GetProcessToken(HANDLE& token)
	{
		if (ConfirmLoad() == false || lpfnOpenProcessToken_ == NULL)
			return false;

		return (lpfnOpenProcessToken_(::GetCurrentProcess(), TOKEN_READ, &token) == TRUE);
	}

	const char * AdvAPI32Loader::kDllName = "advapi32.dll";
	AdvAPI32Loader AdvAPI32Loader::Singleton;
}


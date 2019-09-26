
#include "wobjs/UserEnvLoader.h"
#include "wobjs/AdvAPI32Loader.h"

namespace qkc {

	UserEnvLoader::UserEnvLoader()
	{
		lpfnGetUserProfileDirectorW_ = NULL;
	}
	UserEnvLoader::~UserEnvLoader()
	{
		//
	}

	bool UserEnvLoader::Init()
	{
		bool result = false;
		if ((result = Open(UserEnvLoader::kDllName)) == true)
		{
			lpfnGetUserProfileDirectorW_ = (LPFN_GETUSERPROFILEDIRECTORW)this->GetAddress("GetUserProfileDirectoryW");
			Loaded(true);
		}
		return result;
	}

	void UserEnvLoader::Final()
	{
		Close();
	}

	int UserEnvLoader::GetUserDirector(char * dir, int size)
	{
		if (ConfirmLoad() == false)
			return -1;

		int result = -1;
		if (lpfnGetUserProfileDirectorW_ != NULL)
		{

			HANDLE token = NULL;
			if(AdvAPI32Loader::Singleton.GetProcessToken(token) == false)
				return -1;

			WCHAR profile[256];
			DWORD profile_size = 256;
			BOOL is_got = lpfnGetUserProfileDirectorW_(::GetCurrentProcess(), profile, &profile_size);
			::CloseHandle(token);
			if (is_got == FALSE)
				return -1;

			result = ::WideCharToMultiByte(CP_UTF8, 0, profile, profile_size, dir, size, NULL, NULL);
			if (result >= 0)
				dir[result] = '\0';
		}

		return result;
	}

	const char * UserEnvLoader::kDllName = "userenv.dll";
	UserEnvLoader UserEnvLoader::Singleton;
}



#ifndef __QUARK_WOBJS_USER_ENV_LOADER_H
#define __QUARK_WOBJS_USER_ENV_LOADER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/DllLoader.h"

namespace qkc {

	class QKCAPI UserEnvLoader : public DllLoader {
	public:
		UserEnvLoader();
		virtual ~UserEnvLoader();

		virtual bool Init();
		virtual void Final();

		typedef BOOL(WINAPI *LPFN_GETUSERPROFILEDIRECTORW)(HANDLE token, LPWSTR profile, LPDWORD size);
		int GetUserDirector(char * dir, int size);

		static const char * kDllName;
		static UserEnvLoader Singleton;

	private:
		LPFN_GETUSERPROFILEDIRECTORW lpfnGetUserProfileDirectorW_;
	};

}

#endif  /** __QUARK_WOBJS_USER_ENV_LOADER_H */

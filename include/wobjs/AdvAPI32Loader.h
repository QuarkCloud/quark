
#ifndef __QUARK_WOBJS_ADVAPI32_LOADER_H
#define __QUARK_WOBJS_ADVAPI32_LOADER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/DllLoader.h"

namespace qkc {

	class QKCAPI AdvAPI32Loader : public DllLoader {
	public:
		AdvAPI32Loader();
		virtual ~AdvAPI32Loader();

		virtual bool Init();
		virtual void Final();

		typedef BOOL(WINAPI*LPFN_GETUSERNAMEW)(LPWSTR buffer, LPDWORD size);
		typedef BOOL(WINAPI*LPFN_OPENPROCESSTOKEN)(HANDLE ProcessHandle, DWORD DesiredAccess, PHANDLE TokenHandle);

		int GetUserName(char * name , int size);
		bool GetProcessToken(HANDLE& token);

		static const char * kDllName;
		static AdvAPI32Loader Singleton;

	private:
		LPFN_GETUSERNAMEW lpfnGetUserName_;
		LPFN_OPENPROCESSTOKEN lpfnOpenProcessToken_;
	};

}

#endif  /** __QUARK_WOBJS_ADVAPI32_LOADER_H */

/**
#include <wintf/wthr.h>
#include <windows.h>
#include "../internal/file_system_dev_null.h"
#include "../internal/file_system_proc.h"

2019-12-26
��ʱ�ȿ��ã��Ⱥ�����ʵ�����ˣ���ͬһ����ȥ�����еĵ����;�̬���������ڿ���ʵ����˳��
*/
#include <windows.h>
#include "wobjs/DllInit.h"


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	qkc::SetThisDllHandle((uintptr_t)hinstDll);

	bool result = true;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		//file_system_dev_null_init();
		//file_system_proc_init();
		//result = wthr_process_attach();
		break;
	case DLL_THREAD_ATTACH:
		//result = wthr_thread_attach();
		break;

	case DLL_THREAD_DETACH:
		//result = wthr_thread_detach();
		break;

	case DLL_PROCESS_DETACH:
		//result = wthr_process_detach();
		//file_system_proc_final();
		//file_system_dev_null_final();
		break;
	}

	return (result);
}

namespace qkc {

	static uintptr_t __this_dll_handle__ = 0;

	uintptr_t GetThisDllHandle()
	{
		return __this_dll_handle__;
	}

	void SetThisDllHandle(uintptr_t handle)
	{
		__this_dll_handle__ = handle;
	}

}
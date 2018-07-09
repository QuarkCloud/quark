
#include <wintf/wthr.h>
#include <windows.h>
#include "../internal/file_system_dev_null.h"


BOOL WINAPI DllMain (HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    dll_handle_set((uintptr_t)hinstDll) ;

    bool result = true ;
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        file_system_dev_null_init() ;
        result = wthr_process_attach();
        break;
    case DLL_THREAD_ATTACH:
        result = wthr_thread_attach();
        break;

    case DLL_THREAD_DETACH:
        result = wthr_thread_detach();
        break;

    case DLL_PROCESS_DETACH:
        result = wthr_process_detach();
        file_system_dev_null_final() ;
        break;
    }

    return (result);
}	
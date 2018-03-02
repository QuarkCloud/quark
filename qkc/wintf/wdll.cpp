
#include <wintf/wthr.h>
#include <windows.h>


BOOL WINAPI DllMain (HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    bool result = true ;
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
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
        break;
    }

    return (result);
}	
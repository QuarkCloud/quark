
#include <wait.h>
#include <errno.h>
#include <windows.h>
#include <wintf/wcrt.h>

pid_t wait(__WAIT_STATUS stat_loc)
{
    return 0 ;
}

pid_t waitpid(pid_t pid , int * stat_loc, int options)
{
    /**
        getpid的返回值是GetProcessId，但是_cwait的参数process是HANDLE，这两者之间需要转换
    */
    HANDLE handle = ::OpenProcess(0 , TRUE , pid) ;
    if(handle == INVALID_HANDLE_VALUE)
    {
        errno = ECHILD ;
        return -1 ;
    }

    intptr_t ret = ::_cwait(stat_loc , (intptr_t)handle , options) ;
    if(ret == (intptr_t)handle)
        return pid ;

    return -2 ;
}


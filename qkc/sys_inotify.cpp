
#include <sys/inotify.h>
#include <windows.h>
#include <wintf/wobj.h>

//后续再实现
int inotify_init (void)
{
	HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
	return wobj_set(WOBJ_NOTF , handle , NULL) ;
}

int inotify_add_watch (int fd, const char *name, uint32_t mask)
{
    //::FindFirstChangeNotification(

    return 0 ;
}

int inotify_rm_watch (int fd, uint32_t wd)
{
    return 0 ;
}

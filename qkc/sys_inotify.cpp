
#include <sys/inotify.h>
#include <windows.h>
#include <wintf/wobj.h>

int inotify_init (void)
{
	HANDLE handle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
	return wobj_set(WOBJ_IOCP , handle , NULL) ;
}

int inotify_add_watch (int fd, const char *name, uint32_t mask)
{
    wobj_t * iobj = wobj_get(fd) ;
    if(iobj == NULL || iobj->type != WOBJ_IOCP || iobj->handle == NULL)
        return -1 ;

    HANDLE iocp = iobj->handle ;

    /**
        FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001
        FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002
        FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004
        FILE_NOTIFY_CHANGE_SIZE         0x00000008
        FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010
        FILE_NOTIFY_CHANGE_SECURITY     0x00000100
    */
    DWORD filter = FILE_NOTIFY_CHANGE_FILE_NAME |  FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES ;
    filter |= FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY ;

    HANDLE hnotify = ::FindFirstChangeNotificationA(name , TRUE , filter) ;
    int widx = wobj_set(WOBJ_NOTF , hnotify , NULL) ;

    ::CreateIoCompletionPort(hnotify , iocp , 0 , 0) ;
    return 0 ;
}

int inotify_rm_watch (int fd, uint32_t wd)
{
    wobj_t * obj = wobj_get(wd) ;
    if(obj == NULL || obj->type != WOBJ_NOTF || obj->handle == NULL)
        return -1 ;
    HANDLE handle = obj->handle ;
    wobj_del(wd) ;

    ::FindCloseChangeNotification(handle) ;
    return 0 ;
}

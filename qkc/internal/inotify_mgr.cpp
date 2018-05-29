
#include "inotify_mgr.h"
#include "wintf/wobj.h"
#include <errno.h>

inotify_mgr_t *  inotify_mgr_new()
{
    size_t size = sizeof(inotify_mgr_t) ;
    inotify_mgr_t * mgr = (inotify_mgr_t *)::malloc(size) ;
    if(mgr == NULL)
    {
        errno = ENOMEM ;
        return NULL ;
    }
    ::memset(mgr , 0 , size) ;

    if(inotify_mgr_init(mgr) == true)
        return mgr ;
    ::free(mgr) ;
    return NULL ;
}

int  inotify_mgr_free(inotify_mgr_t * mgr)
{
    inotify_mgr_final(mgr) ;
    ::free(mgr) ;
    return 0 ;
}


int inotify_compare(const rb_node_t * src , const rb_node_t * dst)
{
    const inotify_item_t * sitem = (const inotify_item_t *)src ;
    const inotify_item_t * ditem = (const inotify_item_t *)dst ;

    return (sitem->data.wd - ditem->data.wd) ;
}

bool inotify_mgr_init(inotify_mgr_t * mgr)
{
    if(mgr == NULL)
    {
        errno = EINVAL ;
        return false ;
    }
    ::memset(mgr , 0 , sizeof(mgr)) ;
    mgr->iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    mgr->locker = ::CreateMutexA(NULL , FALSE , NULL) ;
    mgr->items.key_compare = inotify_compare ;

    int oid = ::wobj_set(WOBJ_NOTF , mgr->iocp , mgr) ;
    if(oid == INVALID_WOBJ_ID)
        return false ;

    mgr->ifd = oid ;
    return true ;}

bool inotify_mgr_final(inotify_mgr_t * mgr)
{
    wobj_del(mgr->ifd) ;

    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    rb_node_t * node = NULL ;
    while((node = rb_first(&mgr->items)) != NULL)
    {
        inotify_item_t * item = (inotify_item_t *)node;

        HANDLE handle = item->handle ;
        if(handle != NULL)
            ::FindCloseChangeNotification(handle) ;
        rb_erase(&mgr->items , node) ;

        ::free(node) ;
    }

    ::CloseHandle(mgr->iocp) ;
    ::ReleaseMutex(mgr->locker) ;
    ::CloseHandle(mgr->locker) ;

    return true ;
}

bool inotify_items_free(rb_tree_t * items) 
{
    rb_node_t * node = NULL ;
    while((node = rb_first(items)) != NULL)
    {
        inotify_item_free((inotify_item_t *)node) ;
        rb_erase(items , node) ;

        ::free(node) ;
    }
    return true ;
}

bool inotify_item_free(inotify_item_t * item)
{
    HANDLE handle = item->handle ;
    if(handle == NULL)
        return false ;

    item->handle = NULL ;
    ::CloseHandle(handle) ;

    inotify_ovlp_t * ovlp = item->olvp ;
    item->olvp = NULL ;
    if(ovlp != NULL)
    {
        ovlp->item = NULL ;
        ::free(ovlp) ;
    }
    return true ;
}

size_t inotify_item_size() 
{
    static size_t __inotify_item_size = 0 ;
    if(__inotify_item_size == 0)
    {
        inotify_item_t dummy ;
        __inotify_item_size = (size_t)((char *)&dummy - dummy.data.name) ;
    }
    return __inotify_item_size ;
}

int inotify_mgr_add(inotify_mgr_t * mgr , const char * name , uint32_t mask)
{
    if(mgr == NULL || name == NULL || mask == 0)
    {
        errno = EINVAL ;
        return -1 ;
    }

    size_t item_size = inotify_item_size() ;
    size_t name_size = 0 ;
    if(name != NULL)
        name_size = ::strlen(name) ;

    size_t size = item_size + name_size + 1;
    inotify_item_t * item = (inotify_item_t * )::malloc(size) ;
    if(item == NULL)
    {
        errno = ENOMEM ;
        return -1 ;
    }
    ::memset(item , 0 , size) ;

    item->data.mask = mask ;
    ::memcpy(item->data.name , name , name_size) ;
    item->data.len = name_size + 1 ;

    HANDLE inotify_handle = ::CreateFileA(name , GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE , NULL , OPEN_EXISTING , FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED , NULL) ;
    item->handle = inotify_handle ;

    inotify_ovlp_t * ovlp = (inotify_ovlp_t *)::malloc(sizeof(inotify_ovlp_t)) ;
    ::memset(ovlp , 0 , sizeof(inotify_ovlp_t)) ;
    ovlp->item = item ;
    item->olvp = ovlp ;

    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
    {
        ::free(item) ;
        return -1 ;
    }
    int wd = ++mgr->last_id ;
    item->data.wd = wd ;
    rb_insert(&mgr->items , &item->node) ;

    ::CreateIoCompletionPort(inotify_handle , mgr->iocp , (ULONG_PTR)wd , 0) ;

    DWORD filter = inotify_from_linux(mask) ;
    DWORD bytes_returned = 0 ;
    ovlp->buffer = ovlp->cache ;
    ::ReadDirectoryChangesW(inotify_handle , ovlp->buffer , kOvlpBufferSize , FALSE , filter ,
        &bytes_returned , (LPOVERLAPPED)ovlp , NULL) ;

    ::ReleaseSemaphore(mgr->locker , 1 , NULL) ;
    return wd ;
}

bool inotify_mgr_del(inotify_mgr_t * mgr , int wd)
{
    if(mgr == NULL)
    {
        errno = EINVAL ;
        return false ;
    }

    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
        return false ;

    bool result = false ;
    inotify_item_t kval , *item = NULL;
    ::memset(&kval , 0 , sizeof(inotify_item_t)) ;
    kval.data.wd = wd ;
    item = (inotify_item_t *)rb_find(&mgr->items , &kval.node) ;
    if(item == NULL)
        errno = ENOENT ;
    else
    {
        rb_erase(&mgr->items , &item->node) ;
        inotify_item_free(item) ;
        ::free(item) ;
        mgr->count-- ;

        result = true ;
    }

    ::ReleaseMutex(mgr->locker) ;
    return result ;
}

DWORD inotify_from_linux(uint32_t mask)
{
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

    return filter ;
}

uint32_t inotify_to_linux(DWORD action)
{
/**
    FILE_ACTION_ADDED               The file was added to the directory.
    FILE_ACTION_REMOVED             The file was removed from the directory.
    FILE_ACTION_MODIFIED            The file was modified. This can be a change in the time stamp or attributes.
    FILE_ACTION_RENAMED_OLD_NAME    The file was renamed and this is the old name.
    FILE_ACTION_RENAMED_NEW_NAME    The file was renamed and this is the new name.
*/
    uint32_t mask = 0 ;
    if(action == FILE_ACTION_ADDED)
        mask = IN_CREATE ;
    else if(action == FILE_ACTION_REMOVED)
        mask = IN_DELETE ;
    else if(action == FILE_ACTION_MODIFIED)
        mask = IN_MODIFY | IN_ATTRIB;
    //(action == FILE_ACTION_RENAMED_OLD_NAME || action == FILE_ACTION_RENAMED_NEW_NAME)

    return mask ;
}

int inotify_read(inotify_mgr_t * mgr , char * buffer , int bufsize) 
{  
    if(::WaitForSingleObject(mgr->locker , INFINITE) != WAIT_OBJECT_0)
        return -1 ;

    DWORD bytes_returned = 0 ;
    ULONG_PTR key = 0 ;
    LPOVERLAPPED povlp = NULL ;

    int result_size = 0 , expect_size = 0 ;
    struct inotify_event * ievents = (struct inotify_event *) buffer;

    while(expect_size < bufsize)
    {
        if(::GetQueuedCompletionStatus(mgr->iocp , &bytes_returned , &key , &povlp , 0) == TRUE)
        {
            DWORD parse_size = 0 , next_entry_offset = 0;
            inotify_ovlp_t * ovlp = (inotify_ovlp_t *)povlp ;
            FILE_NOTIFY_INFORMATION * info = (FILE_NOTIFY_INFORMATION *)ovlp->buffer ;
            do{
                next_entry_offset = info->NextEntryOffset ;
                uint32_t mask = inotify_to_linux(info->Action) ;
                if(mask > 0)
                {
                    char file_name[1024] ;
                    int nsize = ::wide_to_char(info->FileName , info->FileNameLength , file_name , sizeof(file_name)) ;
                    if(nsize > 0)
                    {
                        int ievent_size = (int)inotify_item_size() + nsize + 1 ;
                        expect_size += ievent_size ;
                        if(expect_size >= bufsize)
                            break ;

                        ::memset(ievents , 0 , ievent_size) ;
                        ievents->wd = (int)key ;
                        ievents->mask = mask ;

                        ::memcpy(ievents->name , file_name , nsize) ;
                        ievents->name[nsize] = '\0' ;
                        ++nsize ;
                        ievents->len = nsize ;

                        result_size += ievent_size ;
                    }
                }

                parse_size += next_entry_offset ;
                info = (FILE_NOTIFY_INFORMATION *)(ovlp->buffer + parse_size) ;
                if(next_entry_offset == 0)
                {
                    DWORD info_size = (DWORD)((char *)info->FileName - (char *)info) ;
                    info_size += info->FileNameLength ;
                    parse_size += info_size ;
                }
            
            }while(next_entry_offset != 0) ;

            if(next_entry_offset != 0)
            {
                //没有处理完
                ovlp->buffer += parse_size ;
                bytes_returned -= parse_size ;

                ::PostQueuedCompletionStatus(mgr->iocp , bytes_returned , key , (LPOVERLAPPED)ovlp) ;
                break ;
            }

            inotify_item_t * item = ovlp->item ;
            ovlp->buffer = ovlp->cache ;
            ::ReadDirectoryChangesW(item->handle , ovlp->buffer , kOvlpBufferSize , FALSE , inotify_from_linux(item->data.mask) , 
                NULL , (LPOVERLAPPED)ovlp , NULL) ;
        }
        else
        {
            break ;
        }
    }
    return 0 ;
}

int wide_to_char(const wchar_t * wstr , int bytes , char * str , int len) 
{
    size_t wsize = ::wcsnlen(wstr , bytes) ;

    int nbytes = ::WideCharToMultiByte(0 , 0 , wstr , wsize , NULL , 0 , NULL , NULL) ;

    if(nbytes > len)
        return -1 ;

    nbytes = ::WideCharToMultiByte(0 , 0 , wstr , wsize , str , len , NULL , NULL) ;
    return nbytes ;
}


#include <quark_compile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include "wobjs/CrtLinker.h"

int stat (const char * file, struct stat * buf)
{
    struct _stat64 s64 ;
    int ret = _stat64(file , &s64);
    if(ret != 0)
        return ret ;

    HANDLE h = ::CreateFileA(file , GENERIC_READ , 0 , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL) ;
    if(h != INVALID_HANDLE_VALUE)
    {

        BY_HANDLE_FILE_INFORMATION fi ;
        ::memset(&fi , 0 , sizeof(fi)) ;
        if(::GetFileInformationByHandle(h , &fi) == TRUE)
        {
            DWORD low = fi.nFileIndexLow ;
            DWORD high = fi.nFileIndexHigh ;
            high = (high >> 16) << 24 ;
            buf->st_ino = (high | low) ;
        }   
        ::CloseHandle(h) ;
    }

    buf->st_dev = s64.st_dev ;

    buf->st_nlink = s64.st_nlink ;
    buf->st_mode = s64.st_mode ;
    buf->st_uid = s64.st_uid ;
    buf->st_gid = s64.st_gid ;
    buf->st_rdev = s64.st_rdev ;
    buf->st_size = s64.st_size ;
    buf->st_atime = s64.st_atime ;
    buf->st_mtime = s64.st_mtime ;
    buf->st_ctime = s64.st_ctime ;

    return 0 ;
}

int lstat (const char * path, struct stat * statbuf)
{
    return stat(path , statbuf) ;
}

int fstat (int fd , struct stat *buf)
{
    struct _stat64 s64 ;
    int ret = _fstat64(fd , &s64);
    if(ret != 0)
        return ret ;

    HANDLE h = (HANDLE)::_get_osfhandle(fd) ;
    if(h != INVALID_HANDLE_VALUE)
    {

        BY_HANDLE_FILE_INFORMATION fi ;
        ::memset(&fi , 0 , sizeof(fi)) ;
        if(::GetFileInformationByHandle(h , &fi) == TRUE)
        {
            DWORD low = fi.nFileIndexLow ;
            DWORD high = fi.nFileIndexHigh ;
            high = (high >> 16) << 24 ;
            buf->st_ino = (high | low) ;
        }   
    }

    buf->st_dev = s64.st_dev ;
    //buf->st_ino = s64.st_ino ;
    buf->st_nlink = s64.st_nlink ;
    buf->st_mode = s64.st_mode ;
    buf->st_uid = s64.st_uid ;
    buf->st_gid = s64.st_gid ;
    buf->st_rdev = s64.st_rdev ;
    buf->st_size = s64.st_size ;
    buf->st_atime = s64.st_atime ;
    buf->st_mtime = s64.st_mtime ;
    buf->st_ctime = s64.st_ctime ;

    return 0 ;
}

int chmod (const char *file, mode_t mode)
{
    return ::_chmod(file , mode) ;
}

int lchmod (const char *file, mode_t mode) 
{
    return ::_chmod(file , mode) ;
}

int fchmod (int fd , mode_t mode)
{
    HANDLE fhandle = (HANDLE)::_get_osfhandle(fd) ;
    if(fhandle == NULL || fhandle == INVALID_HANDLE_VALUE)
    {
        errno = EINVAL ;
        return -1 ;
    }

    FILE_BASIC_INFO info ;
    if(::GetFileInformationByHandleEx(fhandle , FileBasicInfo , &info , sizeof(FILE_BASIC_INFO)) == FALSE)
    {
        _dosmaperr(GetLastError());
        return -1 ;
    }

    DWORD attr = info.FileAttributes ;
    if(mode & S_IWRITE)
        attr &= ~FILE_ATTRIBUTE_READONLY;
    else if(mode & S_IREAD)
        attr |= FILE_ATTRIBUTE_READONLY ;
    else
    {
        errno = ENOSYS ;
        return -1 ;
    }

    FILE_BASIC_INFO base ;
    base.CreationTime = info.CreationTime ;
    base.LastAccessTime = info.LastAccessTime ;
    base.LastWriteTime = info.LastWriteTime ;
    base.ChangeTime = info.ChangeTime ;
    base.FileAttributes = attr ;
    if(::SetFileInformationByHandle(fhandle , FileBasicInfo , &base , sizeof(base)) == FALSE)
    {
        _dosmaperr(GetLastError());
        return -1 ;
    }

    return 0 ;
}

int mkdir (const char *path, mode_t mode)
{
    return ::_mkdir(path) ;
}




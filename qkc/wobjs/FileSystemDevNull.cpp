
#include <windows.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "wobjs/FileSystemDevNull.h"

namespace qkc {

	const char * FileSystemDevNull::kName = "/dev/null";

	FileSystemDevNull::FileSystemDevNull()
	{
	}

	FileSystemDevNull::~FileSystemDevNull()
	{
	
	}

	int FileSystemDevNull::Create(const char * file, mode_t mode)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemDevNull::Open(const char * path, int oflag, ...)
	{
		errno = ENOSYS;
		return -1;
	}

	void FileSystemDevNull::Close(int fd)
	{
		//
	}

	int FileSystemDevNull::FCntl(int fd, int cmd, ...)
	{
		errno = ENOSYS;
		return -1;
	}

	ssize_t FileSystemDevNull::Write(int fd, const void * buf, size_t bytes)
	{
		errno = ENOSYS;
		return -1;
	}

	ssize_t FileSystemDevNull::Read(int fd, void * buf, size_t bytes)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemDevNull::Dup(int fd)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemDevNull::Dup2(int fd1, int fd2)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemDevNull::LSeek(int fd, off_t offset, int whence)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemDevNull::FStat(int fd, struct stat * stat)
	{
		errno = ENOSYS;
		return -1;
	}

	FileSystemRegister<FileSystemDevNull> __FileSystemDevNullInstance__;
}

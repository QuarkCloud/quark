
#ifndef __QUARK_WOBJS_FILE_SYSTEM_DEV_NULL_H
#define __QUARK_WOBJS_FILE_SYSTEM_DEV_NULL_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "wobjs/FileSystem.h"

namespace qkc {

	class QKCAPI FileSystemDevNull : public FileSystem {
	public:
		FileSystemDevNull();
		virtual ~FileSystemDevNull();

		virtual int Create(const char * file, mode_t mode);
		virtual int Open(const char * path, int oflag, ...);
		virtual void Close(int fd);
		virtual int FCntl(int fd, int cmd, ...);

		virtual ssize_t Write(int fd, const void * buf, size_t bytes);
		virtual ssize_t Read(int fd, void * buf, size_t bytes);
		virtual int Dup(int fd);
		virtual int Dup2(int fd1, int fd2);
		virtual int LSeek(int fd, off_t offset, int whence);
		virtual int FStat(int fd, struct stat * stat);

		static const char * kName; 
	};

}

#endif  /** __QUARK_WOBJS_FILE_SYSTEM_DEV_NULL_H */

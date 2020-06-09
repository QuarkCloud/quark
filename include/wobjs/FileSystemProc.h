
#ifndef __QUARK_WOBJS_FILE_SYSTEM_PROC_H
#define __QUARK_WOBJS_FILE_SYSTEM_PROC_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "wobjs/FileSystem.h"
#include "wobjs/Object.h"

namespace qkc {

	class QKCAPI FileProcInfo : public FileObj {
	public:
		FileProcInfo();
		~FileProcInfo();

		void Path(const char * path);
		inline const char * Path() const { return path_; }

		void SetInfo();
	private:
		char path_[MAX_PATH];
	};

	class QKCAPI FileSystemProc : public FileSystem {
	public:
		FileSystemProc();
		virtual ~FileSystemProc();

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

	private :
		FileProcInfo * Get(int fd);

		int PrintIntr(char *  buf, int size);
		int PrintCPUInfo(char * buf, int size);
		size_t ReadPIDStat(pid_t pid, void * buf, size_t size);

		size_t ReadStat(void * buf, size_t size);
		int ParseIdentifier(const char * identifier, int& family, int& model, int& stepping);
		size_t ReadCPUInfo(void * buf, size_t size);
	};

}

#endif  /** __QUARK_WOBJS_FILE_SYSTEM_PROC_H */

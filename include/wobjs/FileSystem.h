
#ifndef __QUARK_WOBJS_FILE_SYSTEM_H
#define __QUARK_WOBJS_FILE_SYSTEM_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "wobjs/RBTree.h"
#include "wobjs/Object.h"

namespace qkc {

	class FileObj;
	class FileSystem;

	class QKCAPI FileCallback {
	public:
		FileCallback();
		virtual ~FileCallback();

		virtual int Create(FileObj * file);
		virtual int Open(FileObj * file);
		virtual void Close(FileObj * file);

		virtual ssize_t Write(FileObj * file, const void * buf, size_t bytes);
		virtual ssize_t Read(FileObj * file, void * buf, size_t bytes);

		inline FileObj * Owner() { return owner_; }
		inline void Owner(FileObj * file) { owner_ = file; }
	private:
		FileObj * owner_;
	};

	class QKCAPI FileObj : public Object {
	public:
		FileObj();
		virtual ~FileObj();

		inline FileCallback * Callback() { return callback_; }
		inline void Callback(FileCallback * callback) { callback_ = callback; }

		static const int kDevNull	= 1;
		static const int kProc		= 2;
	protected:
		void SetInfo();
		FileCallback *callback_;
	};

	/**
		FileSystem的作用在于挂接到具体的目录中，针对该所有目录下所有对象的操作，都有该类完成。
		外部通过fd索引到FileObj，再由FileObj指向FileSystem。
	*/
	class QKCAPI FileSystem : public RBNode {
	public:
		FileSystem();
		virtual ~FileSystem();

		virtual int Create(const char * file, mode_t mode);
		virtual int Open(const char * path, int oflag, ...);
		virtual void Close(FileObj * file);
		virtual int FCntl(FileObj * file, int cmd, ...);

		virtual ssize_t Write(FileObj * file, const void * buf, size_t bytes);
		virtual ssize_t Read(FileObj * file, void * buf, size_t bytes);
		virtual int Dup(FileObj * file);
		virtual int Dup2(FileObj * file1, FileObj * file2);
		virtual int LSeek(FileObj * file, off_t offset, int whence);
		virtual int FStat(FileObj * file, struct stat * stat);

		static int Mount(const char * name, FileSystem * fs);
		static int Unmount(const char * name);
		static FileSystem * Find(const char * name);

		inline const char * Name() const { return name_; }
		void Name(const char * name);
	private:
		char name_[MAX_PATH];
	};
	
	class QKCAPI FileSystemMgr : private RBTree{
	public:
		FileSystemMgr();
		~FileSystemMgr();

		int Insert(const char * name, FileSystem * fs);
		int Delete(const char * name , FileSystem *&node);
		FileSystem * Find(const char * name);

		static FileSystemMgr& Singleton();

	protected:
		int CompareFileSystem(const FileSystem * src, const FileSystem * dst) const;
		virtual int Compare(const RBNode * src, const RBNode * dst) const;
	};

	template<typename T> 
	class FileSystemRegister {
	public:
		FileSystemRegister()
		{
			fs_ = new T();
			FileSystem::Mount(T::kName, fs_);
		}
		~FileSystemRegister()
		{
			if (fs_ != NULL)
			{
				FileSystem::Unmount(fs_->Name());
				fs_ = NULL;
			}
		}
	private:
		T * fs_;
	};
}

#endif  /** __QUARK_WOBJS_FILE_SYSTEM_H */

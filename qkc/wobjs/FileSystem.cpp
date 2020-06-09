
#include "wobjs/FileSystem.h"

namespace qkc {


	FileCallback::FileCallback()
	{
		owner_ = NULL;
	}

	FileCallback::~FileCallback()
	{
		//
	}

	int FileCallback::Create(FileObj * file)
	{
		return -1;
	}
	int FileCallback::Open(FileObj * file)
	{
		return -1;
	}
	void FileCallback::Close(FileObj * file)
	{
		//
	}

	ssize_t FileCallback::Write(FileObj * file, const void * buf, size_t bytes)
	{
		return 0;
	}
	ssize_t FileCallback::Read(FileObj * file, void * buf, size_t bytes)
	{
		return 0;
	}


	FileObj::FileObj()
	{
		//
	}

	FileObj::~FileObj()
	{
		//
	}

	void FileObj::SetInfo()
	{
		OType(Object::kFile);
	}



	FileSystem::FileSystem()
	{
		name_[0] = '\0';
	}

	FileSystem::~FileSystem()
	{
		//
	}

	int FileSystem::Create(const char * file, mode_t mode)
	{
		return 0;
	}

	int FileSystem::Open(const char * path, int oflag, ...)
	{
		return 0;
	}

	void FileSystem::Close(FileObj * file)
	{
		//
	}

	int FileSystem::FCntl(FileObj * file, int cmd, ...)
	{
		return 0;
	}

	ssize_t FileSystem::Write(FileObj * file, const void * buf, size_t bytes)
	{
		return 0;
	}

	ssize_t FileSystem::Read(FileObj * file, void * buf, size_t bytes)
	{
		return 0;
	}

	int FileSystem::Dup(FileObj * file)
	{
		return 0;
	}

	int FileSystem::Dup2(FileObj * file1, FileObj * file2)
	{
		return 0;
	}

	int FileSystem::LSeek(FileObj * file, off_t offset, int whence)
	{
		return 0;
	}

	int FileSystem::FStat(FileObj * file, struct stat * stat)
	{
		return 0;
	}

	void FileSystem::Name(const char * name)
	{
		name_[0] = '\0';
		if (name == NULL)
			return;
		size_t nsize = ::strlen(name);
		if (nsize >= MAX_PATH)
			nsize = MAX_PATH - 1;
		::memcpy(name_, name, nsize);
		name_[nsize] = '\0';
	}

	int FileSystem::Mount(const char * name, FileSystem * fs)
	{
		return FileSystemMgr::Singleton().Insert(name , fs);
	}

	int FileSystem::Unmount(const char * name)
	{
		FileSystem * fs = NULL;
		int result = FileSystemMgr::Singleton().Delete(name , fs);
		if (result == 0 && fs != NULL)
			delete fs;
		return result;
	}

	FileSystem * FileSystem::Find(const char * name)
	{
		return FileSystemMgr::Singleton().Find(name);
	}


	FileSystemMgr::FileSystemMgr()
	{
		//
	}

	FileSystemMgr::~FileSystemMgr()
	{
		Clear();
	}

	int FileSystemMgr::Insert(const char * name, FileSystem * fs)
	{
		if (fs == NULL || name == NULL)
			return -1;

		fs->Name(name);

		RBNode * parent = NULL , **link = &root_;
		while ((*link) != NULL)
		{
			RBNode * parent = *link;
			FileSystem * dst = (FileSystem *)parent;
			int diff = ::strcmp(fs->Name(), dst->Name());
			if (diff < 0)
				*link = parent->Left;
			else if (diff > 0)
				*link = parent->Right;
			else
			{
				//已经存在了，只能返回错误。
				return -1;
			}
		}

		fs->Link(parent, link);
		InternalInsert(fs, false, NULL);
		return 0;
	}

	int FileSystemMgr::Delete(const char * name, FileSystem *&node)
	{
		RBNode *cur = Root();
		while (cur != NULL)
		{
			FileSystem * fs = (FileSystem *)cur;
			int diff = ::strcmp(name, fs->Name());
			if (diff == 0)
				break;

			if (diff < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		if (cur == NULL)
			return false;

		node = (FileSystem *)cur;

		RBNode * rebalance = InternalErase(cur, NULL);
		if (rebalance != NULL)
			InternalEraseColor(rebalance);
		return true;
	}

	FileSystem * FileSystemMgr::Find(const char * name)
	{
		RBNode * cur = Root();
		while (cur != NULL)
		{
			FileSystem * fs = (FileSystem *)cur;
			int diff = ::strcmp(name, fs->Name());
			if (diff == 0)
				return fs;
			if (diff < 0)
				cur = cur->Left;
			else
				cur = cur->Right;
		}

		return NULL;
	}

	int FileSystemMgr::CompareFileSystem(const FileSystem * src, const FileSystem * dst) const
	{
		if (src == NULL)
		{
			return (dst == NULL) ? 0 : -1;
		}
		else if (dst == NULL)
			return 1;

		return ::strcmp(src->Name(), dst->Name());
	}

	int FileSystemMgr::Compare(const RBNode * src, const RBNode * dst) const
	{
		return CompareFileSystem((const FileSystem *)src, (const FileSystem *)dst);	
	}

	FileSystemMgr& FileSystemMgr::Singleton()
	{
		static FileSystemMgr __file_system_mgr_singleton__;
		return __file_system_mgr_singleton__;
	}

}
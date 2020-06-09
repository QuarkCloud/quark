
#include "wobjs/FileSystemProc.h"
#include "wobjs/SysInfo.h"
#include "wobjs/ErrorMap.h"
#include "wobjs/StrUtil.h"
#include "wobjs/ProcessMemoryInfo.h"
#include "wobjs/ObjMgr.h"

#include <errno.h>

namespace qkc {

	FileProcInfo::FileProcInfo()
	{
		path_[0] = '\0';
		OSubType(FileObj::kProc);
	}

	FileProcInfo::~FileProcInfo()
	{
		//
	}

	void FileProcInfo::Path(const char * path)
	{
		if (path == NULL)
			path_[0] = '\0';
		else
		{
			size_t len = ::strlen(path);
			if (len >= MAX_PATH)
				len = MAX_PATH - 1;
			::memcpy(path_, path, len);
			path_[len] = '\0';
		}
	}

	FileSystemProc::FileSystemProc()
	{
		//
	}

	FileSystemProc::~FileSystemProc()
	{
		//
	}

	int FileSystemProc::Create(const char * file, mode_t mode)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemProc::Open(const char * path, int oflag, ...)
	{
		FileProcInfo * file = new FileProcInfo();
		file->Path(path);
		int index = ObjMgr::Singleton().Add(file, file);
		if (index < 0)
			delete file;
		return index;
	}

	FileProcInfo * FileSystemProc::Get(int fd)
	{
		Object * obj = ObjMgr::Singleton().Get(fd);
		if (obj == NULL || obj->IsFile() == false || obj->OSubType() != FileObj::kProc)
			return NULL;
		return (FileProcInfo *)obj;
	}

	void FileSystemProc::Close(int fd)
	{
		FileProcInfo * info = Get(fd);
		if (info == NULL)
			return;
		Object * obj = NULL;
		ObjMgr::Singleton().Delete(fd, obj);
		delete info;
	}

	int FileSystemProc::FCntl(int fd, int cmd, ...)
	{
		errno = ENOSYS;
		return -1;
	}

	ssize_t FileSystemProc::Write(int fd, const void * buf, size_t bytes)
	{
		errno = ENOSYS;
		return -1;
	}

	ssize_t FileSystemProc::Read(int fd, void * buf, size_t bytes)
	{
		FileProcInfo * info = Get(fd);
		if (info == NULL)
			return -1;

		StrSeg segs[64];
		size_t count = StrSplit(info->Path(), '/', segs, 64);
		if (count >= 2)
		{
			if (StrNCmp(segs[1].Start, segs[1].Size, "stat") == 0)
				return ReadStat(buf, bytes);
			else if (StrNCmp(segs[1].Start, segs[1].Size, "cpuinfo") == 0)
				return ReadCPUInfo(buf, bytes);
			else if (StrNCmp(segs[1].Start, segs[1].Size, "self") == 0 && count >= 3)
			{
				if (StrNCmp(segs[2].Start, segs[2].Size, "stat") == 0)
					return ReadPIDStat(0, buf, bytes);
			}
		}

		errno = ENOSYS;
		return -1;
	}

	int FileSystemProc::Dup(int fd)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemProc::Dup2(int fd1, int fd2)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemProc::LSeek(int fd, off_t offset, int whence)
	{
		errno = ENOSYS;
		return -1;
	}

	int FileSystemProc::FStat(int fd, struct stat * stat)
	{
		errno = ENOSYS;
		return -1;
	}

	const char * FileSystemProc::kName = "/proc";

	int FileSystemProc::PrintIntr(char *  buf, int size)
	{
		int isize = ::sprintf(buf, "intr ");
		for (int idx = 0; idx < 240; ++idx)
		{
			int zsize = ::sprintf(buf + isize, "0 ");
			isize += zsize;
		}
		buf[isize] = '\n';
		return isize;
	}

	int FileSystemProc::PrintCPUInfo(char * buf, int size)
	{
		int num_processors = (int)qkc::SysInfo::Singleton().NumberOfProcessors();//(int)::GetNumberOfProcessors() ;

		DWORD sppi_size = num_processors * sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * 4;
		SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * sppi = (SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *)::malloc(sppi_size);
		if (sppi == NULL)
		{
			errno = ENOMEM;
			return 0;
		}

		ULONG result_size = 0;
		NTSTATUS status = NtQuerySystemInformation(SystemPerformanceInformation, sppi, sppi_size, &result_size);
		if (status != STATUS_SUCCESS)
		{
			errno = oserr_map(::GetLastError());
			return 0;
		}

		char * str = buf;
		int isize = 0;

		for (int cpu = -1; cpu < num_processors; ++cpu)
		{
			if (cpu < 0)
			{
				isize = ::sprintf(str, "cpu 0 0 0 0 0 0 0 0\n");
				continue;
			}

			uint64_t user_time = 0, nice_time = 0, system_time = 0;
			uint64_t idle_time = 0, iowait_time = 0, irq_time = 0, softirq_time = 0;

			SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION& pi = sppi[cpu];
			user_time = pi.UserTime.QuadPart / 10000;
			system_time = (pi.KernelTime.QuadPart - pi.IdleTime.QuadPart) / 10000;
			idle_time = pi.IdleTime.QuadPart / 10000;
			irq_time = pi.InterruptTime.QuadPart / 10000;

			int csize = ::sprintf(str + isize, "cpu%d %llu %llu %llu %llu %llu %llu %llu 0\n",
				cpu, user_time, nice_time, system_time, idle_time,
				iowait_time, irq_time, softirq_time);

			isize += csize;
		}

		return isize;
	}

	size_t FileSystemProc::ReadPIDStat(pid_t pid, void * buf, size_t size)
	{
		if (pid != 0)
			return 0;

		ProcessMemoryInfo info;
		if (info.Load() == false)
			return 0;

		size_t work_size = info.WorkingSetSize;
		size_t page_size = SysInfo::Singleton().PageSize();
		size_t rss = work_size / page_size;

		return ::sprintf((char *)buf, "0 () 0 0 0 0 0 0 0 0 " "0 0 0 0 0 0 0 0 0 0 "
			"0 0 0 %u 0 0 0 0 0 0 " "0 0 0 0 0 0 0 0 0 0 " "0", rss);
	}

	size_t FileSystemProc::ReadStat(void * buf, size_t size)
	{
		int total_size = (int)size;
		char * str = (char *)buf;
		int slen = PrintCPUInfo(str, total_size);

		slen += PrintIntr(str + slen, total_size - slen);

		slen += ::sprintf(str + slen, "ctxt 0\n");
		slen += ::sprintf(str + slen, "btime 0\n");
		slen += ::sprintf(str + slen, "processes 0\n");
		slen += ::sprintf(str + slen, "procs_running 0\n");
		slen += ::sprintf(str + slen, "procs_blocked 0\n");

		return slen;
	}

	int FileSystemProc::ParseIdentifier(const char * identifier, int& family, int& model, int& stepping)
	{
		char tmpstr[256];
		int tsize = 0, stage = 0; //family=1 value=2 ; model=3 value=4 ; stepping=5 value=6
		const char * pchar = identifier;
		while (true)
		{
			char ch = *pchar;
			if (ch == ' ' || ch == '\0')
			{
				tmpstr[tsize] = '\0';
				if (tsize != 0)
				{
					if ((stage & 1) == 1)
					{
						//求值
						int value = ::atoi(tmpstr);
						if (stage == 1)
							family = value;
						else if (stage == 3)
							model = value;
						else if (stage == 5)
							stepping = value;

						stage++;
					}
					else if (StrNCmp(tmpstr, tsize, "Family") == 0)
						stage = 1;
					else if (StrNCmp(tmpstr, tsize, "Model") == 0)
						stage = 3;
					else if (StrNCmp(tmpstr, tsize, "Stepping") == 0)
						stage = 5;
				}

				tsize = 0;
				if (ch == '\0')
					break;
			}
			++pchar;
		}

		return (int)(pchar - identifier);
	}

	size_t FileSystemProc::ReadCPUInfo(void * buf, size_t size)
	{
		char * str = (char *)buf;
		int isize = (int)size, offset = 0;
		int num_processors = qkc::SysInfo::Singleton().NumberOfProcessors();//(int)::GetNumberOfProcessors() ;
		for (int idx = 0; idx < num_processors; ++idx)
		{
			CHAR key_name[128];
			HKEY processor_key;
			DWORD cpu_speed;
			DWORD cpu_speed_size = sizeof(cpu_speed);
			CHAR cpu_brand[256];
			DWORD cpu_brand_size = sizeof(cpu_brand);
			CHAR vendor_id[256];
			DWORD vendor_id_size = sizeof(vendor_id);
			CHAR identifier[256];
			DWORD identifier_size = sizeof(identifier);

			sprintf(key_name, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d", idx);
			DWORD r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name, 0, KEY_QUERY_VALUE, &processor_key);
			if (r != ERROR_SUCCESS)
				break;

			if (RegQueryValueEx(processor_key, "~MHz", NULL, NULL, (BYTE*)&cpu_speed, &cpu_speed_size) != ERROR_SUCCESS)
			{
				RegCloseKey(processor_key);
				break;
			}
			if (RegQueryValueEx(processor_key, "ProcessorNameString", NULL, NULL, (BYTE*)&cpu_brand, &cpu_brand_size) != ERROR_SUCCESS)
			{
				RegCloseKey(processor_key);
				break;
			}
			if (RegQueryValueEx(processor_key, "VendorIdentifier", NULL, NULL, (BYTE*)&vendor_id, &vendor_id_size) != ERROR_SUCCESS)
			{
				RegCloseKey(processor_key);
				break;
			}

			if (RegQueryValueEx(processor_key, "Identifier", NULL, NULL, (BYTE*)&identifier, &identifier_size) != ERROR_SUCCESS)
			{
				RegCloseKey(processor_key);
				break;
			}
			RegCloseKey(processor_key);

			/**
				2018-08-07
				根据注册表的信息，填写cpuinfo
			*/
			int family = 0, model = 0, stepping = 0;
			ParseIdentifier(identifier, family, model, stepping);

			offset += ::sprintf(str + offset, "processor\t: %d\n", idx);
			offset += ::sprintf(str + offset, "vendor_id\t: %s\n", vendor_id);
			offset += ::sprintf(str + offset, "cpu family\t: %d\n", family);
			offset += ::sprintf(str + offset, "model\t\t: %d\n", model);
			offset += ::sprintf(str + offset, "model name\t: %s\n", cpu_brand);
			offset += ::sprintf(str + offset, "stepping\t: %d\n", stepping);
			offset += ::sprintf(str + offset, "cpu MHz\t\t: %u\n", cpu_speed);
			offset += ::sprintf(str + offset, "cache size\t: \n");
			offset += ::sprintf(str + offset, "physical id\t: \n");
			offset += ::sprintf(str + offset, "siblings\t: \n");
			offset += ::sprintf(str + offset, "core id\t: \n");
			offset += ::sprintf(str + offset, "cpu cores\t: \n");
			offset += ::sprintf(str + offset, "apicid\t: \n");
			offset += ::sprintf(str + offset, "fpu\t: \n");
			offset += ::sprintf(str + offset, "fpu_exception\t: \n");
			offset += ::sprintf(str + offset, "cpuid level\t: \n");
			offset += ::sprintf(str + offset, "wp\t: \n");
			offset += ::sprintf(str + offset, "flags\t: \n");
			offset += ::sprintf(str + offset, "bogomips\t: \n");
			offset += ::sprintf(str + offset, "clflush size\t: \n");
			offset += ::sprintf(str + offset, "cache_alignmen\t: \n");
			offset += ::sprintf(str + offset, "address sizes\t: \n");
			offset += ::sprintf(str + offset, "power management\t: \n");
			offset += ::sprintf(str + offset, "\n");
		}

		return offset;
	}

	FileSystemRegister<FileSystemProc> __FileSystemProcInstance__;
}

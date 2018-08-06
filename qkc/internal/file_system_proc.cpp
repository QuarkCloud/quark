
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <winnt.h>

#include "wintf/wobj.h"
#include "wintf/wps.h"
#include "wintf/wcap.h"
#include "wintf/werr.h"
#include "file_system.h"
#include "file_system_proc.h"
#include "string_util.h"


static file_system_t  file_system_proc ;

typedef struct{
    char * path ;
} fs_proc_info_t ;

static size_t file_system_proc_pid_stat_read(pid_t pid , void * buf , size_t size) ;
static size_t file_system_proc_stat_read(void * buf , size_t size) ;
static size_t file_system_proc_cpuinfo_read(void * buf , size_t size) ;

int file_system_proc_creat(const char * file , mode_t mode)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_open(const char * path , int oflag , ...)
{
    size_t isize = sizeof(fs_proc_info_t) ;
    fs_proc_info_t * info = (fs_proc_info_t *)::malloc(isize) ;
    info->path = ::strdup(path) ;

    return ::wobj_set(WOBJ_FILE , info , &file_system_proc) ;
}

void file_system_proc_close(int fd)
{
    wobj_t * obj = ::wobj_get(fd) ;
    if(obj == NULL || obj->type != WOBJ_FILE)
        return ;

    if(obj->handle != NULL)
    {
        fs_proc_info_t * info = (fs_proc_info_t *)obj->handle ;
        obj->handle = NULL ;

        if(info->path != NULL)
            ::free(info->path) ;

        ::free(info) ;
    }

    wobj_del(fd) ;
}

int file_system_proc_fcntl(int fd , int cmd , ...)
{
    errno = ENOSYS ;
    return -1 ;
}

ssize_t file_system_proc_write(int fd , const void * buf , size_t bytes)
{
    errno = ENOSYS ;
    return -1 ;
}

ssize_t file_system_proc_read(int fd , void * buf , size_t bytes)
{
    wobj_t * obj = ::wobj_get(fd) ;
    if(obj == NULL)
    {
        errno = EINVAL ;
        return -1 ;
    }

    if(obj->handle == NULL || obj->addition == NULL)
        return -1 ;

    fs_proc_info_t * info = (fs_proc_info_t *)obj->handle ;

    str_seg_t segs[64] ;
    size_t count = str_split(info->path , '/' , segs , 64) ;
    if(count >= 2)
    {
        if(::str_ncmp(segs[1].start , segs[1].size , "stat") == 0)
            return file_system_proc_stat_read(buf , bytes) ;
        else if(::str_ncmp(segs[1].start , segs[1].size , "cpuinfo") == 0)
            return file_system_proc_cpuinfo_read(buf , bytes) ;
        else if(::str_ncmp(segs[1].start , segs[1].size , "self") == 0 && count >= 3)
        {
            if(::str_ncmp(segs[2].start , segs[2].size , "stat") == 0)
                return file_system_proc_pid_stat_read(0 , buf , bytes) ;
        }
    }

    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_dup(int fd)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_dup2(int fd1 , int fd2)
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_lseek(int fd , off_t offset , int whence) 
{
    errno = ENOSYS ;
    return -1 ;
}

int file_system_proc_fstat(int fd , struct stat * stat)
{
    errno = ENOSYS ;
    return -1 ;
}

void file_system_proc_init()
{
    file_system_proc.creat = file_system_proc_creat ;
    file_system_proc.open =  file_system_proc_open ;
    file_system_proc.close =  file_system_proc_close ;
    file_system_proc.fcntl =  file_system_proc_fcntl ;
    file_system_proc.write =  file_system_proc_write ;
    file_system_proc.read =  file_system_proc_read ;

    file_system_proc.dup =  file_system_proc_dup ;
    file_system_proc.dup2 =  file_system_proc_dup2 ;
    file_system_proc.lseek =  file_system_proc_lseek ;
    file_system_proc.fstat =  file_system_proc_fstat ;
    file_system_proc.data =  NULL;

    file_system_mount(file_system_proc_name , &file_system_proc) ;
}

void file_system_proc_final()
{
    file_system_unmount(file_system_proc_name) ;
}

static size_t file_system_proc_pid_stat_read(pid_t pid , void * buf , size_t size) 
{
    if(pid != 0)
        return 0 ;

    process_memory_info_t info ;
    if(get_process_memory_info(pid , &info) == false)
        return 0 ;

    size_t work_size = info.WorkingSetSize ;
    size_t page_size = GetPageSize() ;
    size_t rss = work_size / page_size ;

    return ::sprintf((char *)buf , "0 () 0 0 0 0 0 0 0 0 " "0 0 0 0 0 0 0 0 0 0 "
        "0 0 0 %u 0 0 0 0 0 0 " "0 0 0 0 0 0 0 0 0 0 " "0" , rss) ;
}

static int print_cpu_info(char * buf , int size) 
{
    int num_processors = (int)::GetNumberOfProcessors() ;

    DWORD sppi_size = num_processors * sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * 4;
    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION * sppi = (SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *)::malloc(sppi_size) ;
    if(sppi == NULL)
    {
        errno = ENOMEM ;
        return 0 ;
    }

    ULONG result_size = 0 ;
    NTSTATUS status = NtQuerySystemInformation(SystemPerformanceInformation , sppi , sppi_size , &result_size) ;
    if(status != STATUS_SUCCESS)
    {
        errno = oserr_map(::GetLastError()) ;
        return 0 ;
    }

    char * str = buf ;
    int isize = 0 ;

    for(int cpu = -1 ; cpu < num_processors ; ++cpu)
    {
        if(cpu < 0)
        {
            isize = ::sprintf(str , "cpu 0 0 0 0 0 0 0 0\n") ;
            continue ;
        }

        uint64_t user_time = 0 , nice_time = 0 , system_time = 0 ;
        uint64_t idle_time = 0 , iowait_time = 0 , irq_time = 0 , softirq_time = 0;

        SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION& pi = sppi[cpu] ;
        user_time = pi.UserTime.QuadPart / 10000 ;
        system_time = (pi.KernelTime.QuadPart - pi.IdleTime.QuadPart)/10000 ;
        idle_time = pi.IdleTime.QuadPart / 10000 ;
        irq_time = pi.InterruptTime.QuadPart / 10000 ;

        int csize = ::sprintf(str + isize , "cpu%d %llu %llu %llu %llu %llu %llu %llu 0\n" ,
            cpu , user_time , nice_time , system_time , idle_time , 
            iowait_time , irq_time , softirq_time) ;

        isize += csize ;
    }

    return isize ;
}

static int print_intr(char *  buf , int size)
{
    int isize = ::sprintf(buf , "intr ") ;
    for(int idx = 0 ; idx < 240 ; ++idx)
    {
        int zsize = ::sprintf(buf + isize , "0 ") ;
        isize += zsize ;
    }
    buf[isize] = '\n' ;
    return isize ;
}

static size_t file_system_proc_stat_read(void * buf , size_t size) 
{
    int total_size = (int)size ;
    char * str = (char *)buf ;
    int slen = print_cpu_info(str , total_size) ;

    slen += print_intr(str + slen , total_size - slen) ;

    slen += ::sprintf(str + slen , "ctxt 0\n") ;
    slen += ::sprintf(str + slen , "btime 0\n") ;
    slen += ::sprintf(str + slen , "processes 0\n") ;
    slen += ::sprintf(str + slen , "procs_running 0\n") ;
    slen += ::sprintf(str + slen , "procs_blocked 0\n") ;

    return slen ;
}

static size_t file_system_proc_cpuinfo_read(void * buf , size_t size) 
{
    int num_processors = (int)::GetNumberOfProcessors() ;
    for(int idx = 0 ; idx < num_processors ; ++idx)
    {
        CHAR key_name[128];
        HKEY processor_key;
        DWORD cpu_speed;
        DWORD cpu_speed_size = sizeof(cpu_speed);
        WCHAR cpu_brand[256];
        DWORD cpu_brand_size = sizeof(cpu_brand);
        size_t len;

        len = sprintf(key_name , "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",idx);
        DWORD r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_name,0,KEY_QUERY_VALUE,&processor_key);
        if (r != ERROR_SUCCESS)
            break ;

        if (RegQueryValueEx(processor_key,"~MHz",NULL,NULL,(BYTE*) &cpu_speed,&cpu_speed_size) != ERROR_SUCCESS)
        {
          RegCloseKey(processor_key);
          break ;
        }

        if (RegQueryValueEx(processor_key,"ProcessorNameString",NULL,NULL,(BYTE*) &cpu_brand,&cpu_brand_size) != ERROR_SUCCESS)
        {
          RegCloseKey(processor_key);
          break ;
        }

        RegCloseKey(processor_key);
    }

    return 0 ;
}


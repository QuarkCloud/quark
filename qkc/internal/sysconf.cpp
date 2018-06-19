

#include "sysconf.h"
#include <windows.h>

int __get_pagesize() 
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}

int __get_num_of_processor() 
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors ;
}

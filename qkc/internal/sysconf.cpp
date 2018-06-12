

#include "sysconf.h"
#include <windows.h>

int __get_pagesize() 
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}

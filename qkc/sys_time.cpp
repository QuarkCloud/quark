
#include <sys/time.h>
#include <stdint.h>
#include <windows.h>
#include "wobjs/DateTime.h"


//¾«¶ÈÎª100ÄÉÃë
static inline uint64_t hr_nano_time() {return qkc::GetWinHrTime() ;}

int gettimeofday (struct timeval * tv , void * tz)
{
    uint64_t nano = hr_nano_time() ;
    tv->tv_sec = (time_t)(nano / 10000000) ;
    tv->tv_usec = (suseconds_t)((nano % 10000000) / 10) ;
    return 0 ;
}

int utimes (const char * file, const struct timeval tvp[2])
{
    return 0 ;
}



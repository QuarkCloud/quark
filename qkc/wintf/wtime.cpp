#include <wintf/wtime.h>

uint64_t GetWinHrTime()
{
    FILETIME ft ;
    ::GetSystemTimeAsFileTime(&ft) ;

	/*
		2011-03-08
		参考BOOST的microsec_time_clock.hpp	将FILETIME转化为毫秒

		shift is difference between 1970-Jan-01 & 1601-Jan-01
		in 100-nanosecond intervals 
	*/
	const uint64_t shift = 116444736000000000ULL; // (27111902 << 32) + 3577643008
	uint64_t caster = ft.dwHighDateTime  ;
	caster = (caster << 32 ) + ft.dwLowDateTime - shift ;

    return caster ;
}

uint64_t ElapseToMSec(const struct timespec * ts) 
{
    uint64_t now = GetWinHrTime() / 10000 ;
    uint64_t to = ts->tv_sec ;
    to = to * 1000 + ts->tv_nsec / 1000000 ;

    if(to <= now)
        return 0 ;
    else
        return (to - now) ;
}

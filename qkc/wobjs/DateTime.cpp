
#include <windows.h>
#include "wobjs/DateTime.h"
#include "wobjs/CrtLinker.h"
#include "wobjs/StrUtil.h"
#include <string.h>

namespace qkc {

	uint64_t GetWinHrTime()
	{
		FILETIME ft;
		::GetSystemTimeAsFileTime(&ft);

		/*
			2011-03-08
			参考BOOST的microsec_time_clock.hpp	将FILETIME转化为毫秒

			shift is difference between 1970-Jan-01 & 1601-Jan-01
			in 100-nanosecond intervals
		*/
		const uint64_t shift = 116444736000000000ULL; // (27111902 << 32) + 3577643008
		uint64_t caster = ft.dwHighDateTime;
		caster = (caster << 32) + ft.dwLowDateTime - shift;

		return caster;
	}


	int64_t NowTime()
	{
		return (int64_t)_time64(NULL);
	}

	int64_t NowUTime()
	{
		uint64_t hrt = GetWinHrTime();
		return (int64_t)(hrt / 10);
	}

	int NowNanoTime(struct timespec * ts)
	{
		if (ts == NULL)
			return -1;

		uint64_t hrt = GetWinHrTime();

		ts->tv_sec = (int64_t)(hrt / 10000000);
		ts->tv_nsec = (int)((hrt % 10000000) * 1000);

		return 0;	
	}

	int TimeZone()
	{
		return (int)_timezone;
	}

	uint64_t PerformanceCounter()
	{
		LARGE_INTEGER li;
		if (::QueryPerformanceCounter(&li) == FALSE)
			return 0;
		uint64_t counter = li.QuadPart;
		return counter;
	}

	uint64_t PerformanceFrequency()
	{
		static uint64_t __performance_frequency__ = 1;
		if (__performance_frequency__ == 1)
		{
			LARGE_INTEGER li;
			if (::QueryPerformanceFrequency(&li) == FALSE)
				return 1;
			__performance_frequency__ = li.QuadPart;
		}		
		return __performance_frequency__;
	}


	Date::Date(bool calc_now)
	{
		Reset();
		if (calc_now == true)
			Assign(NowTime());
	}

	Date::Date(int64_t ts)
	{
		Reset();
		Assign(ts);
	}

	Date::Date(int year, int month, int day)
	{
		Reset();
		Assign(year, month, day);
	}

	Date::~Date()
	{
		//
	}

	void Date::Reset()
	{
		time_ = 0;
		year_ = month_ = day_ = 0;
		yday_ = wday_ = 0;
	}


	bool Date::Assign(int64_t ts)
	{
		struct tm stm;
		if (_localtime64_s(&stm, &ts) != 0)
			return false;

		year_ = stm.tm_year + 1900;
		month_ = stm.tm_mon + 1;
		day_ = stm.tm_mday;
		yday_ = stm.tm_yday + 1;
		wday_ = stm.tm_wday;
		if (wday_ == 0)
			wday_ = 7;

		//清除时分秒，确保是0点开始
		stm.tm_hour = stm.tm_min = stm.tm_sec = 0;
		__time64_t t64 = _mktime64(&stm);
		if (t64 < 0)
			return false;
		time_ = (int64_t)t64;
		return true;
	}

	bool Date::Assign(const char * str)
	{
		int year = 0, month = 0, day = 0;
		if (StrToDate(str, year, month, day) < 0)
			return false;

		return Assign(year, month, day);
	}

	bool Date::Assign(int year, int month, int day)
	{
		struct tm stm;
		::memset(&stm, 0, sizeof(stm));

		stm.tm_year = year - 1900;
		stm.tm_mon = month - 1;
		stm.tm_mday = day;

		__time64_t t64 = ::_mktime64(&stm);
		if (t64 < 0)
			return false;

		if (::_localtime64_s(&stm, &t64) != 0)
			return false;

		year_ = stm.tm_year + 1900;
		month_ = stm.tm_mon + 1;
		day_ = stm.tm_mday;
		yday_ = stm.tm_yday + 1;
		wday_ = stm.tm_wday;
		if (wday_ == 0)
			wday_ = 7;

		time_ = (int64_t)t64;
		return true;
	}

	bool Date::NextDays(int days)
	{
		int64_t ts = days * 86400 + time_;
		return Assign(ts);
	}

	bool Date::NextMonths(int months)
	{
		int new_month = month_ + months;
		int year = (new_month - 1) / 12;
		year_ += year;
		month_ = new_month - 12 * year;
		return true;
	}

	bool Date::NextYears(int years)
	{
		year_ += years;
		return true;
	}

	int Date::ToString(char * str, int size) const
	{
		return ::snprintf(str, size, "%04d%02d%02d", year_, month_, day_);
	}

	const Date& Date::Today()
	{
		static Date __today_date__;

		int64_t ts = NowTime();
		if (ts - __today_date__.Time() > 86400)
		{
			__today_date__.Assign(ts);
		}

		return __today_date__;
	}

	STime::STime(bool calc_now)
	{
		Reset();
		if (calc_now == true)
			Assign(NowTime());
	}
	STime::STime(int64_t ts)
	{
		Reset();
		Assign(ts);
	}

	STime::STime(int hour, int minute, int second)
	{
		Reset();
		Assign(hour, minute, second);
	}

	STime::~STime()
	{
		//
	}

	void STime::Reset()
	{
		hour_ = minute_ = second_ = 0;
	}

	bool STime::Assign(int64_t ts)
	{
		int64_t lt = (ts + TimeZone() * 3600 ) % 86400;
		hour_ = (int)(lt / 3600);
		minute_ = (int)((lt - hour_ * 3600) / 60);
		second_ = (int)(lt - hour_ * 3600 - minute_ * 60);
		return true;
	}

	bool STime::Assign(const char * str)
	{
		int hour = 0, minute = 0, second = 0, usec = 0;
		if (StrToTime(str, hour, minute, second, usec) < 0)
			return false;

		return Assign(hour, minute, second);
	}

	bool STime::Assign(int hour, int minute, int second)
	{
		if (hour < 0 || minute < 0 || second < 0)
			return false;

		if (hour >= 24 || minute >= 60 || second >= 60)
			return false;
		hour_ = hour;
		minute_ = minute;
		second_ = second;
		time_ = hour * 3600 + minute_ * 60 + second_;
		return true;
	}

	int STime::ToString(char * str, int size) const
	{
		return ::snprintf(str, size, "%0d:%02d:%02d", hour_, minute_, second_);
 	}


	UTime::UTime(bool calc_now)
	{
		Reset();
		if (calc_now == true)
			Assign(NowUTime());
	}

	UTime::UTime(int64_t ut)
	{
		Reset();
		Assign(ut);
	}

	UTime::UTime(int hour, int minute, int second, int usec)
	{
		Reset();
		Assign(hour, minute, second, usec);	
	}

	UTime::~UTime()
	{
		//
	}
	void UTime::Reset()
	{
		time_ = hour_ = minute_ = second_ = usec_ = 0;
	}

	bool UTime::Assign(int64_t ut)
	{
		int64_t us = ut % 86400000000LL;

		int64_t st = us / 1000000;
		usec_ = (int)(us % 1000000);
		hour_ = (int)(st / 3600);
		minute_ = (int)((st - hour_ * 3600) / 60);
		second_ = (int)(st - (hour_ * 3600 + minute_ * 60));

		int64_t ts = hour_ * 3600 + minute_ * 60 + second_;
		time_ = (int)(ts * 1000000 + usec_);
		return true;	
	}

	bool UTime::Assign(const char * str)
	{
		if (StrToTime(str, hour_, minute_, second_, usec_) < 0)
			return false;
		int64_t ts = hour_ * 3600 + minute_ * 60 + second_;
		time_ = ts * 1000000 + usec_;
		return true;
	}

	bool UTime::Assign(int hour, int minute, int second, int usec)
	{
		hour_ = hour;
		minute_ = minute;
		second_ = second;

		int64_t ts = hour_ * 3600 + minute_ * 60 + second_;
		time_ = ts * 1000000 + usec_;
		return true;	
	}

	int UTime::ToString(char * str, int size) const
	{
		return ::snprintf(str, size, "%02d:%02d:%02d.%06d", hour_, minute_, second_, usec_);
	}

	ElapseTime::ElapseTime(bool auto_start)
	{
		stop_time_ = 0;

		if (auto_start == false)
			start_time_ = 0;
		else
			start_time_ = GetWinHrTime();
	}

	ElapseTime::~ElapseTime()
	{
		//
	}

	void ElapseTime::Start()
	{
		start_time_ = GetWinHrTime();
	}

	void ElapseTime::Stop()
	{
		stop_time_ = GetWinHrTime();
	}

	uint64_t ElapseTime::Time() const
	{
		if (start_time_ == 0)
			return 0;

		if (stop_time_ == 0)
			stop_time_ = GetWinHrTime();

		if (stop_time_ <= start_time_)
			return 0;

		return (stop_time_ - start_time_);
	}

	ElapseCounter::ElapseCounter(bool auto_start)
	{
		start_counter_ = stop_counter_ = 0;
		if (auto_start == true)
			Start();
	}

	ElapseCounter::~ElapseCounter()
	{
		//
	}

	void ElapseCounter::Start()
	{
		start_counter_ = PerformanceCounter();
	}

	void ElapseCounter::Stop()
	{
		stop_counter_ = PerformanceCounter();
	}

	uint64_t ElapseCounter::Counter() const
	{
		if (start_counter_ == 0)
			return 0;
		if (stop_counter_ == 0)
			stop_counter_ = PerformanceCounter();

		if (stop_counter_ < start_counter_)
			return 0;

		return (stop_counter_ - start_counter_);
	}

	int64_t ElapseCounter::Time() const
	{
		uint64_t counter = Counter();
		if (counter == 0)
			return 0;

		double et = (double)counter;
		et = (et / PerformanceFrequency()) * 1000000;
		return (int64_t)et;
	}

	uint64_t ElapseToMSec(const struct timespec * ts)
	{
		uint64_t now = GetWinHrTime() / 10000;
		uint64_t to = ts->tv_sec;
		to = to * 1000 + ts->tv_nsec / 1000000;

		if (to <= now)
			return 0;
		else
			return (to - now);
	}

}


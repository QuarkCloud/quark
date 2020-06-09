
#ifndef __QUARK_WOBJS_DATE_TIME_H
#define __QUARK_WOBJS_DATE_TIME_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <time.h>

namespace qkc {
	//100���뾫��
	QKCAPI uint64_t GetWinHrTime();

	QKCAPI uint64_t ElapseToMSec(const struct timespec * ts);

	//����ʱ�������
	QKCAPI int64_t NowTime();
	//΢�뾫��
	QKCAPI int64_t NowUTime();
	//100���뾫��
	QKCAPI int NowNanoTime(struct timespec * ts);

	QKCAPI int TimeZone();

	QKCAPI uint64_t PerformanceCounter();
	QKCAPI uint64_t PerformanceFrequency();

	//����msvcrt�ĺ���ʵ�֣������Լ���д
	class QKCAPI Date {
	public:
		Date(bool calc_now = false);
		Date(int64_t ts);
		Date(int year, int month, int day);
		~Date();

		void Reset();

		inline int Year() const { return year_; }
		inline int Month() const { return month_; }
		inline int Day() const { return day_; }
		inline int YDay() const { return yday_; }
		inline int WDay() const { return wday_; }
		inline int64_t Time() const { return time_; }
		
		bool Assign(int64_t ts);
		bool Assign(const char * str);
		bool Assign(int year, int month, int day);

		bool NextDays(int days = 1);
		bool NextMonths(int months = 1);
		bool NextYears(int years = 1);

		int ToString(char * str, int size) const;

		static const Date& Today();	
	private:
		int64_t time_;		//����ʱ�������
		int year_;
		int month_;
		int day_;
		int yday_;		//1-356��1��1��Ϊ1��struct tm��Ϊ0
		int wday_;		//1-7������Ϊ7��struct tm������Ϊ0
	};

	//�뼶����
	class QKCAPI STime {
	public:
		STime(bool calc_now = false);
		STime(int64_t ts);
		STime(int hour, int minute, int second);
		~STime();

		void Reset();

		inline int Hour() const { return hour_; }
		inline int Minute() const { return minute_; }
		inline int Second() const { return second_; }
		inline int Time() const { return time_; }

		bool Assign(int64_t ts);
		bool Assign(const char * str);
		bool Assign(int hour, int minute, int second);

		int ToString(char * str, int size) const;

	private:
		int time_;
		int hour_;
		int minute_;
		int second_;
	};

	class QKCAPI UTime {
	public:
		UTime(bool calc_now = false);
		UTime(int64_t ut);
		UTime(int hour, int minute, int second, int usec);
		~UTime();
		void Reset();

		inline int Hour() const { return hour_; }
		inline int Minute() const { return minute_; }
		inline int Second() const { return second_; }
		inline int USec() const { return usec_; }
		inline int64_t Time() const { return time_; }

		bool Assign(int64_t ut);
		bool Assign(const char * str);
		bool Assign(int hour, int minute, int second, int usec);

		int ToString(char * str, int size) const;

	private:
		int64_t time_;
		int hour_;
		int minute_;
		int second_;
		int usec_;
	};

	//ֱ��ʹ��GetWinHrTime������100����ľ���
	class QKCAPI ElapseTime {
	public:
		ElapseTime(bool auto_start = false);
		~ElapseTime();

		void Start();
		void Stop();

		uint64_t Time() const;

	private:
		uint64_t start_time_;
		mutable uint64_t stop_time_;
	};

	class QKCAPI ElapseCounter {
	public:
		ElapseCounter(bool auto_start = false);
		~ElapseCounter();

		void Start();
		void Stop();

		uint64_t Counter() const;
		int64_t Time() const;	//����΢��

	private:
		uint64_t start_counter_;
		mutable uint64_t stop_counter_;
	};

}

#endif  /** __QUARK_WOBJS_DATE_TIME_H */

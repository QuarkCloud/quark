
#ifndef __QUARK_WOBJS_STR_UTIL_H
#define __QUARK_WOBJS_STR_UTIL_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <string.h>

namespace qkc {

	typedef struct  __st_str_seg
	{
		const char * Start;
		size_t Size;
	}StrSeg;

	QKCAPI size_t StrSplit(const char * str, const char  seperator, StrSeg * segs, size_t count);

	QKCAPI int StrNCmp(const char * src, size_t nsize, const char * dst);

	QKCAPI bool StrToInt32(const char * str, int32_t& value);
	QKCAPI bool StrToInt64(const char * str, int64_t& value);

	//֧��2019-1-1����"-" �� "/" �ָ���ʽ����20190101�޷ָ���ʽ
	QKCAPI int StrToDate(const char * str, int& year, int& month, int& day);
	//֧��12:23:59.123456����12:23:59.123����12:23:59�޺���ģʽ
	QKCAPI int StrToTime(const char * str, int& hour, int& minute, int& second, int& usec);
	//�ں��������ָ�ʽ���Կո�ֿ�
	QKCAPI int StrToDateTime(const char * str, int&year, int& month, int& day, int& hour, int& minute, int& second, int& usec);

}

#endif  /** __QUARK_WOBJS_STR_UTIL_H */

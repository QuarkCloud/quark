
#include <stdio.h>
#include <stdarg.h>
#include "wintf/wcrt.h"

#ifndef _FILE_DEFINED
#define _FILE_DEFINED
struct _iobuf
{
    void* _Placeholder;
};
#endif

__BEGIN_DECLS
QUARK_LINKAGE FILE *  __cdecl __acrt_iob_func(unsigned idx);
__END_DECLS

QKCAPI FILE *  __qkc_iob_func__(unsigned idx)
{
    return __acrt_iob_func(idx);
}


int _vsprintf_l(char*  const buffer, char const* const format, _locale_t  const locale, va_list arglist);
int _vsnprintf_l(char* const buffer, size_t const buffer_count, char const* const format,
    _locale_t const locale, va_list arglist);
int _vfprintf_l(FILE* const stream, char const* const format, _locale_t const locale, va_list arglist);
int _vfscanf_l(FILE* const stream, char const* const format, _locale_t const locale, va_list arglist);
int _vsscanf_l(char const* const buffer, char const* const format, _locale_t const locale, va_list arglist);

int sprintf(char * s, const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);

    result = _vsprintf_l(s , format, NULL, arglist);

    _crt_va_end(arglist);
    return result;
}

int printf(const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);
    result = _vfprintf_l(stdout, format, NULL, arglist);
    _crt_va_end(arglist);
    return result;
}

int _vsprintf_l(char*  const buffer,char const* const format,_locale_t  const locale,va_list arglist)
{
    return _vsnprintf_l(buffer, (size_t)-1, format , locale, arglist);
}

int _vsnprintf_l(char* const buffer,size_t const buffer_count,char const* const format,
    _locale_t const locale,va_list arglist)
{
    int const result = __stdio_common_vsprintf(0 , buffer, buffer_count, format, locale, arglist);

    return result < 0 ? -1 : result;
}

int _vfprintf_l(FILE* const stream, char const* const format,_locale_t const locale,va_list arglist)
{
    return __stdio_common_vfprintf(0, stream, format, locale, arglist);
}

int snprintf(char * s, size_t maxlen, const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);
    result = vsnprintf(s, maxlen, format, arglist);
    _crt_va_end(arglist);
    return result;
}

int vsnprintf(char* const buffer, size_t const buffer_count , char const* const format, va_list arglist)
{
    int const result = __stdio_common_vsprintf(0, buffer, buffer_count, format, NULL, arglist);

    return result < 0 ? -1 : result;
}

int fprintf(FILE * stream, const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);
    result = _vfprintf_l(stream, format, NULL, arglist);
    _crt_va_end(arglist);
    return result;
}

int fscanf(FILE * stream, const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);
    result = _vfscanf_l(stream, format, NULL, arglist);
    _crt_va_end(arglist);
    return result;
}

int sscanf(const char * s, const char * format, ...)
{
    int result;
    va_list arglist;
    _crt_va_start(arglist, format);
    result = _vsscanf_l(s, format, NULL, arglist);
    _crt_va_end(arglist);
    return result;

}

int _vfscanf_l(FILE* const stream,char const* const format,_locale_t const locale,va_list arglist)
{
    return __stdio_common_vfscanf(0, stream, format, locale, arglist);
}

int _vsscanf_l(char const* const buffer,char const* const format,_locale_t const locale,va_list arglist)
{
    return __stdio_common_vsscanf(0, buffer, (size_t)-1, format, locale, arglist);
}





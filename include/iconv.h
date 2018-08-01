
#ifndef __QKC_ICONV_H
#define __QKC_ICONV_H 1

#include <quark_compile.h>

__BEGIN_DECLS

typedef void *iconv_t;
/**
    ��Ч��CodePage����ʹ��iconv --list������ֵ֧�������б�
    linux�µĴ��� :     GB2312[GBK]  GB18030   UTF-8[UTF8]   UTF-16[UTF16]    UTF-32[UTF32]
    windows�µĴ��� ��   936         54936      65001        1200[LE]          12000[LE]
*/
QKCAPI iconv_t iconv_open (const char * to , const char * from);

QKCAPI size_t iconv (iconv_t cd , char ** inbuf , size_t * inleft,     char ** outbuf, size_t * outleft);

QKCAPI int iconv_close (iconv_t cd);

__END_DECLS

#endif /* __QKC_ICONV_H */

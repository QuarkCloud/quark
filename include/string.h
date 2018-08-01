
#ifndef __QKC_STRING_H
#define __QKC_STRING_H 1

#include <quark_compile.h>

__BEGIN_DECLS

QUARK_LINKAGE void *memcpy (void * dest, const void * src, size_t n);
QUARK_LINKAGE void *memmove (void *dest, const void *src, size_t n) ;
QUARK_LINKAGE void *memset (void *s, int c, size_t n) ;
QUARK_LINKAGE int memcmp (const void *s1, const void *s2, size_t n) ;
QUARK_LINKAGE void *memchr (const void *s, int c, size_t n);
QUARK_LINKAGE char *strcpy (char * dest, const char * src);
QUARK_LINKAGE char *strncpy (char * dest,   const char * src, size_t n) ;

QUARK_LINKAGE char *strcat (char * dest, const char * src) ;
QUARK_LINKAGE char *strncat (char * dest, const char * src, size_t n) ;

QUARK_LINKAGE int strcmp (const char *s1, const char *s2) ;
QUARK_LINKAGE int strncmp (const char *s1, const char *s2, size_t n) ;
QUARK_LINKAGE char *strdup (const char *s) ;
QUARK_LINKAGE char *strchr (const char *s, int c) ;

QUARK_LINKAGE char *strrchr (const char *s, int c);

QUARK_LINKAGE char *strstr (const char *haystack, const char *needle) ;
QUARK_LINKAGE char *strtok (char * s, const char * delim) ;

QKCAPI char *strtok_r (char * s, const char * delim,  char ** save_ptr);

QUARK_LINKAGE size_t strlen (const char *s);
QUARK_LINKAGE char *strerror (int errnum) ;

QUARK_LINKAGE size_t wcslen(const wchar_t * s);
QUARK_LINKAGE size_t wcsnlen(const wchar_t * s, size_t max_count);

QKCAPI char *strerror_r (int errnum , char * buf, size_t buflen);

__END_DECLS

#endif /**__QKC_STRING_H*/


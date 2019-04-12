
#ifndef __QUARK_PIPE_UTIL_H
#define __QUARK_PIPE_UTIL_H 1

#include <windows.h>


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef QUARK_LINKAGE
#define QUARK_LINKAGE  __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif


QUARK_LINKAGE void *malloc(size_t size);
QUARK_LINKAGE void *realloc(void * ptr, size_t size);
QUARK_LINKAGE void * calloc(size_t count, size_t size);
QUARK_LINKAGE void free(void * ptr);


QUARK_LINKAGE void *memcpy(void * dest, const void * src, size_t n);
QUARK_LINKAGE void *memmove(void *dest, const void *src, size_t n);
QUARK_LINKAGE void *memset(void *s, int c, size_t n);
QUARK_LINKAGE int memcmp(const void *s1, const void *s2, size_t n);
QUARK_LINKAGE void *memchr(const void *s, int c, size_t n);
QUARK_LINKAGE char *strcpy(char * dest, const char * src);
QUARK_LINKAGE char *strncpy(char * dest, const char * src, size_t n);

QUARK_LINKAGE char *strcat(char * dest, const char * src);
QUARK_LINKAGE char *strncat(char * dest, const char * src, size_t n);

QUARK_LINKAGE int strcmp(const char *s1, const char *s2);
QUARK_LINKAGE int strncmp(const char *s1, const char *s2, size_t n);
QUARK_LINKAGE char *strdup(const char *s);
QUARK_LINKAGE char *strchr(const char *s, int c);

QUARK_LINKAGE char *strrchr(const char *s, int c);

QUARK_LINKAGE char *strstr(const char *haystack, const char *needle);
QUARK_LINKAGE char *strtok(char * s, const char * delim);

QUARK_LINKAGE size_t strlen(const char *s);


#ifdef __cplusplus
}
#endif


#endif /**__QUARK_PIPE_UTIL_H*/


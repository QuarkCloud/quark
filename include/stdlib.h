
#ifndef __QKC_STDLIB_H
#define __QKC_STDLIB_H 1

#include <quark_compile.h>
#include <alloca.h>

__BEGIN_DECLS

QUARK_LINKAGE extern char ** _environ ;

#define environ _environ

#define	RAND_MAX	2147483647
#define	EXIT_FAILURE	1	/* Failing exit status.  */
#define	EXIT_SUCCESS	0	/* Successful exit status.  */

QUARK_LINKAGE double atof (const char * nptr) ;
QUARK_LINKAGE int atoi (const char * nptr) ;
QUARK_LINKAGE long int atol (const char *nptr) ;

QUARK_LINKAGE double strtod (const char * nptr,     char ** endptr);
QUARK_LINKAGE float strtof (const char * nptr,  char ** endptr) ;
QUARK_LINKAGE long double strtold (const char * nptr, char ** endptr);
QUARK_LINKAGE long int strtol (const char * nptr, char ** endptr, int base) ;
QUARK_LINKAGE unsigned long int strtoul (const char * nptr, char ** endptr, int base) ;
QUARK_LINKAGE long long int strtoll (const char * nptr, char ** endptr, int base) ;

QUARK_LINKAGE int rand (void)  ;
QUARK_LINKAGE void srand (unsigned int  seed) ;
QKCAPI int rand_r (unsigned int * seed) ;

QUARK_LINKAGE void *malloc (size_t size) ;
QUARK_LINKAGE void *realloc (void * ptr, size_t size)  ;
QUARK_LINKAGE void * calloc(size_t count , size_t size);
QUARK_LINKAGE void free (void * ptr) ;

QUARK_LINKAGE void abort (void) ;
QUARK_LINKAGE int on_exit (void (* func) (int status, void * arg), void * arg) ;
QUARK_LINKAGE void exit (int status ) ;
QUARK_LINKAGE int system (const char * command) ;

QUARK_LINKAGE char * getenv(const char * name);
QKCAPI char * secure_getenv (const char *name) ;

QKCAPI int putenv(char *str);
QKCAPI int setenv(const char *envname, const char *envval, int overwrite) ;

//最后六个字符必须是XXXXXX
QKCAPI char *mkdtemp (char * template_str) ;

QKCAPI char *realpath (const char * name, char *resolved) ;

QUARK_LINKAGE void qsort(void * base, size_t count , size_t size , int (* compare)(const void *, const void *));

__END_DECLS

#endif /**__QKC_STDLIB_H*/


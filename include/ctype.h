
#ifndef __QKC_CTYPE_H
#define __QKC_CTYPE_H 1

#include <quark_compile.h>

__BEGIN_DECLS

QUARK_LINKAGE int isalnum (int c) ;
QUARK_LINKAGE int isalpha (int c) ;
QUARK_LINKAGE int iscntrl (int c) ;
QUARK_LINKAGE int isdigit (int c) ;
QUARK_LINKAGE int islower (int c) ;

QUARK_LINKAGE int isgraph (int c) ;
QUARK_LINKAGE int isprint (int c) ;
QUARK_LINKAGE int ispunct (int c) ;
QUARK_LINKAGE int isspace (int c) ;
QUARK_LINKAGE int isupper (int c) ;

QUARK_LINKAGE int isxdigit (int c) ;
QKCAPI int isblank (int c) ;


QUARK_LINKAGE int tolower (int c) ;
QUARK_LINKAGE int toupper (int c) ;

QKCAPI int isctype (int c , int mask);

__END_DECLS

#endif /** __QKC_CTYPE_H */

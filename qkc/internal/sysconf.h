
#ifndef __QKC_INTERNAL_SYSCONF_H
#define __QKC_INTERNAL_SYSCONF_H 1

#include <quark_compile.h>
#include <bits/confname.h>

__BEGIN_DECLS

//_SC_PAGESIZE
int __get_pagesize() ;

//_SC_NPROCESSORS_ONLN
int __get_num_of_processor() ;

__END_DECLS

#endif /** __QKC_INTERNAL_SYSCONF_H */

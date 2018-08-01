
#ifndef __QKC_GETOPT_H
#define __QKC_GETOPT_H 1

#include <quark_compile.h>

__BEGIN_DECLS

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

QKCAPI int getopt (int argc , char *const *argv, const char *shortopts) ;

__END_DECLS


#endif /* __QKC_GETOPT_H */

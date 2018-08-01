#ifndef __QKC_SYS_SENDFILE_H
#define __QKC_SYS_SENDFILE_H 1

#include <quark_compile.h>
#include <sys/types.h>

__BEGIN_DECLS

QKCAPI ssize_t sendfile(int out_fd , int in_fd , off_t * offset , size_t count) ;

QKCAPI ssize_t sendfile64(int out_fd , int in_fd , off64_t * offset , size_t count) ;

__END_DECLS


#endif	/** __QKC_SYS_SENDFILE_H */

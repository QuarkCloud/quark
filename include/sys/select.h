
#ifndef __QKC_SYS_SELECT_H
#define __QKC_SYS_SELECT_H	1

#include <quark_compile.h>
#include <bits/select.h>

__BEGIN_DECLS

typedef long int __fd_mask;

#define __FD_SETSIZE            1024
#define __NFDBITS	    (8 * (int) sizeof (__fd_mask))
#define	__FD_ELT(d)	    ((d) / __NFDBITS)
#define	__FD_MASK(d)	((__fd_mask) 1 << ((d) % __NFDBITS))

typedef struct
{
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
} fd_set;

#define	FD_SETSIZE		__FD_SETSIZE

typedef __fd_mask fd_mask;
# define NFDBITS		__NFDBITS


#define	FD_SET(fd, fdsetp)	__FD_SET (fd, fdsetp)
#define	FD_CLR(fd, fdsetp)	__FD_CLR (fd, fdsetp)
#define	FD_ISSET(fd, fdsetp)	__FD_ISSET (fd, fdsetp)
#define	FD_ZERO(fdsetp)		__FD_ZERO (fdsetp)


QKCAPI int select (int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval *timeout);

__END_DECLS

#endif /** __QKC_SYS_SELECT_H */

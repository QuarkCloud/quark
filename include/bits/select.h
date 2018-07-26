
#ifndef __QKC_BITS_SELECT_H
#define __QKC_BITS_SELECT_H 1

#include <quark_compile.h>
#include <bits/wordsize.h>

__BEGIN_DECLS


# define __FD_ZERO(set)  \
  do {									      \
    unsigned int __i;							      \
    fd_set *__arr = (set);						      \
    for (__i = 0; __i < sizeof (fd_set) / sizeof (__fd_mask); ++__i)	      \
      __FDS_BITS (__arr)[__i] = 0;					      \
  } while (0)


#define __FD_SET(d, set)        ((void) (__FDS_BITS (set)[__FD_ELT (d)] |= __FD_MASK (d)))
#define __FD_CLR(d, set)        ((void) (__FDS_BITS (set)[__FD_ELT (d)] &= ~__FD_MASK (d)))
#define __FD_ISSET(d, set)      ((__FDS_BITS (set)[__FD_ELT (d)] & __FD_MASK (d)) != 0)

__END_DECLS

#endif /** __QKC_BITS_SELECT_H */

#ifndef __QKC_SYS_UIO_H
#define __QKC_SYS_UIO_H 1

#include <sys/types.h>

#define UIO_MAXIOV	1024

/* Structure for scatter/gather I/O.  */
struct iovec
{
    void *iov_base;	/* Pointer to data.  */
    size_t iov_len;	/* Length of data.  */
};

#ifdef __cplusplus
extern "C" {
#endif

//Œ¥ µœ÷
QKCAPI ssize_t readv(int fd , const struct iovec * iovec , int count);

QKCAPI ssize_t writev(int fd , const struct iovec * iovec , int count);

QKCAPI ssize_t preadv (int fd, const struct iovec * iovec, int count, off_t offset) ;

QKCAPI ssize_t pwritev (int fd, const struct iovec * iovec, int count, off_t offset) ;

#ifdef __cplusplus
}
#endif


#endif  /**__QKC_SYS_UIO_H */

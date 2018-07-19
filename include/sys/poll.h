
#ifndef __QKC_SYS_POLL_H
#define __QKC_SYS_POLL_H 1

#include <quark_compile.h>


#ifdef __cplusplus
extern "C" {
#endif

#define POLLIN      1
#define POLLOUT     2

typedef unsigned long int nfds_t;

struct pollfd
{
    int fd;			/* File descriptor to poll.  */
    short int events;		/* Types of events poller cares about.  */
    short int revents;		/* Types of events that actually occurred.  */
};


QKCAPI int poll (struct pollfd * fds, nfds_t nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif	/** __QKC_SYS_POLL_H */

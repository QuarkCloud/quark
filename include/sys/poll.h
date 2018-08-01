
#ifndef __QKC_SYS_POLL_H
#define __QKC_SYS_POLL_H 1

#include <quark_compile.h>

__BEGIN_DECLS

#define POLLIN          0x001           /* There is data to read.  */
#define POLLPRI         0x002           /* There is urgent data to read.  */
#define POLLOUT         0x004           /* Writing now will not block.  */

#define POLLRDNORM      0x040           /* Normal data may be read.  */
#define POLLRDBAND      0x080           /* Priority data may be read.  */
#define POLLWRNORM      0x100           /* Writing now will not block.  */
#define POLLWRBAND      0x200           /* Priority data may be written.  */
#define POLLMSG         0x400
#define POLLREMOVE      0x1000
#define POLLRDHUP       0x2000
#define POLLERR         0x008           /* Error condition.  */
#define POLLHUP         0x010           /* Hung up.  */
#define POLLNVAL        0x020           /* Invalid polling request.  */


typedef unsigned long int nfds_t;

struct pollfd
{
    int fd;			/* File descriptor to poll.  */
    short int events;		/* Types of events poller cares about.  */
    short int revents;		/* Types of events that actually occurred.  */
};


QKCAPI int poll (struct pollfd * fds, nfds_t nfds, int timeout);

__END_DECLS

#endif	/** __QKC_SYS_POLL_H */

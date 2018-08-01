
#ifndef __QKC_NET_IF_H
#define __QKC_NET_IF_H 1

#include <quark_compile.h>
#include <stdint.h>

__BEGIN_DECLS

enum
  {
    IFF_UP = 0x1,		/* Interface is up.  */
# define IFF_UP	IFF_UP
    IFF_BROADCAST = 0x2,	/* Broadcast address valid.  */
# define IFF_BROADCAST	IFF_BROADCAST
    IFF_DEBUG = 0x4,		/* Turn on debugging.  */
# define IFF_DEBUG	IFF_DEBUG
    IFF_LOOPBACK = 0x8,		/* Is a loopback net.  */
# define IFF_LOOPBACK	IFF_LOOPBACK
    IFF_POINTOPOINT = 0x10,	/* Interface is point-to-point link.  */
# define IFF_POINTOPOINT IFF_POINTOPOINT
    IFF_NOTRAILERS = 0x20,	/* Avoid use of trailers.  */
# define IFF_NOTRAILERS	IFF_NOTRAILERS
    IFF_RUNNING = 0x40,		/* Resources allocated.  */
# define IFF_RUNNING	IFF_RUNNING
    IFF_NOARP = 0x80,		/* No address resolution protocol.  */
# define IFF_NOARP	IFF_NOARP
    IFF_PROMISC = 0x100,	/* Receive all packets.  */
# define IFF_PROMISC	IFF_PROMISC

    /* Not supported */
    IFF_ALLMULTI = 0x200,	/* Receive all multicast packets.  */
# define IFF_ALLMULTI	IFF_ALLMULTI

    IFF_MASTER = 0x400,		/* Master of a load balancer.  */
# define IFF_MASTER	IFF_MASTER
    IFF_SLAVE = 0x800,		/* Slave of a load balancer.  */
# define IFF_SLAVE	IFF_SLAVE

    IFF_MULTICAST = 0x1000,	/* Supports multicast.  */
# define IFF_MULTICAST	IFF_MULTICAST

    IFF_PORTSEL = 0x2000,	/* Can set media type.  */
# define IFF_PORTSEL	IFF_PORTSEL
    IFF_AUTOMEDIA = 0x4000,	/* Auto media select active.  */
# define IFF_AUTOMEDIA	IFF_AUTOMEDIA
    IFF_DYNAMIC = 0x8000	/* Dialup device with changing addresses.  */
# define IFF_DYNAMIC	IFF_DYNAMIC
  };

QKCAPI unsigned int if_nametoindex (const char * ifname) ;
QKCAPI char *if_indextoname (unsigned int ifindex, char *ifname) ;

struct if_nameindex
{
    unsigned int if_index;
    char *if_name;
};

QKCAPI struct if_nameindex *if_nameindex (void) ;

QKCAPI void if_freenameindex (struct if_nameindex * ptr) ;

__END_DECLS

#endif	/** __QKC_NET_IF_H */

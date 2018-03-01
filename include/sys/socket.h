#ifndef __QKC_SYS_SOCKET_H
#define __QKC_SYS_SOCKET_H 1

#include <quark_compile.h>

#include <stdint.h>
#include <sys/types.h>

/* Types of sockets.  */
enum __socket_type
{
    SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based byte streams.  */
#define SOCK_STREAM SOCK_STREAM
    SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams  of fixed maximum length.  */
#define SOCK_DGRAM SOCK_DGRAM
    SOCK_RAW = 3,			/* Raw protocol interface.  */
#define SOCK_RAW SOCK_RAW
    SOCK_RDM = 4,			/* Reliably-delivered messages.  */
#define SOCK_RDM SOCK_RDM
    SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,    datagrams of fixed maximum length.  */
#define SOCK_SEQPACKET SOCK_SEQPACKET
    SOCK_PACKET = 10		/* Linux specific way of getting packets at the dev level.  For writing rarp and
                                other similar things on the user level. */
#define SOCK_PACKET SOCK_PACKET
};

/* Protocol families.  */
#define	PF_UNSPEC	0	/* Unspecified.  */
#define	PF_LOCAL	1	/* Local to host (pipes and file-domain).  */
#define	PF_UNIX		PF_LOCAL /* Old BSD name for PF_LOCAL.  */
#define	PF_FILE		PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define	PF_INET		2	/* IP protocol family.  */
#define	PF_AX25		3	/* Amateur Radio AX.25.  */
#define	PF_IPX		4	/* Novell Internet Protocol.  */
#define	PF_APPLETALK	5	/* Appletalk DDP.  */
#define	PF_NETROM	6	/* Amateur radio NetROM.  */
#define	PF_BRIDGE	7	/* Multiprotocol bridge.  */
#define	PF_ATMPVC	8	/* ATM PVCs.  */
#define	PF_X25		9	/* Reserved for X.25 project.  */
#define	PF_INET6	10	/* IP version 6.  */
#define	PF_ROSE		11	/* Amateur Radio X.25 PLP.  */
#define	PF_DECnet	12	/* Reserved for DECnet project.  */
#define	PF_NETBEUI	13	/* Reserved for 802.2LLC project.  */
#define	PF_SECURITY	14	/* Security callback pseudo AF.  */
#define	PF_KEY		15	/* PF_KEY key management API.  */
#define	PF_NETLINK	16
#define	PF_ROUTE	PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define	PF_PACKET	17	/* Packet family.  */
#define	PF_ASH		18	/* Ash.  */
#define	PF_ECONET	19	/* Acorn Econet.  */
#define	PF_ATMSVC	20	/* ATM SVCs.  */
#define	PF_SNA		22	/* Linux SNA Project */
#define	PF_IRDA		23	/* IRDA sockets.  */
#define	PF_PPPOX	24	/* PPPoX sockets.  */
#define	PF_WANPIPE	25	/* Wanpipe API sockets.  */
#define	PF_BLUETOOTH	31	/* Bluetooth sockets.  */
#define	PF_MAX		32	/* For now..  */

/* Address families.  */
#define	AF_UNSPEC	PF_UNSPEC
#define	AF_LOCAL	PF_LOCAL
#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
#define	AF_INET		PF_INET
#define	AF_AX25		PF_AX25
#define	AF_IPX		PF_IPX
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_NETROM	PF_NETROM
#define	AF_BRIDGE	PF_BRIDGE
#define	AF_ATMPVC	PF_ATMPVC
#define	AF_X25		PF_X25
#define	AF_INET6	PF_INET6
#define	AF_ROSE		PF_ROSE
#define	AF_DECnet	PF_DECnet
#define	AF_NETBEUI	PF_NETBEUI
#define	AF_SECURITY	PF_SECURITY
#define	AF_KEY		PF_KEY
#define	AF_NETLINK	PF_NETLINK
#define	AF_ROUTE	PF_ROUTE
#define	AF_PACKET	PF_PACKET
#define	AF_ASH		PF_ASH
#define	AF_ECONET	PF_ECONET
#define	AF_ATMSVC	PF_ATMSVC
#define	AF_SNA		PF_SNA
#define	AF_IRDA		PF_IRDA
#define	AF_PPPOX	PF_PPPOX
#define	AF_WANPIPE	PF_WANPIPE
#define	AF_BLUETOOTH	PF_BLUETOOTH
#define	AF_MAX		PF_MAX

/* Socket level values.  Others are defined in the appropriate headers.

   XXX These definitions also should go into the appropriate headers as
   far as they are available.  */
#define SOL_RAW		255
#define SOL_DECNET      261
#define SOL_X25         262
#define SOL_PACKET	263
#define SOL_ATM		264	/* ATM layer (cell level).  */
#define SOL_AAL		265	/* ATM Adaption Layer (packet level).  */
#define SOL_IRDA	266

/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN	128

typedef unsigned short int sa_family_t;
struct sockaddr
{
    sa_family_t sa_family ;
    char sa_data[14];		/* Address data.  */
};

/* The following constants should be used for the second parameter of
 `shutdown'.*/
enum
{
    SHUT_RD = 0 , 		/* No more receptions.*/
#define SHUT_RD		SHUT_RD
    SHUT_WR , 		/* No more transmissions.*/
#define SHUT_WR		SHUT_WR
    SHUT_RDWR		/* No more receptions or transmissions.*/
#define SHUT_RDWR	SHUT_RDWR
};


/* Bits in the FLAGS argument to `send', `recv', et al.  */
enum
{
    MSG_OOB		= 0x01,	/* Process out-of-band data.  */
#define MSG_OOB		MSG_OOB
    MSG_PEEK		= 0x02,	/* Peek at incoming messages.  */
#define MSG_PEEK	MSG_PEEK
    MSG_DONTROUTE	= 0x04,	/* Don't use local routing.  */
#define MSG_DONTROUTE	MSG_DONTROUTE
#ifdef __USE_GNU
    /* DECnet uses a different name.  */
    MSG_TRYHARD		= MSG_DONTROUTE,
# define MSG_TRYHARD	MSG_DONTROUTE
#endif
    MSG_CTRUNC		= 0x08,	/* Control data lost before delivery.  */
#define MSG_CTRUNC	MSG_CTRUNC
    MSG_PROXY		= 0x10,	/* Supply or ask second address.  */
#define MSG_PROXY	MSG_PROXY
    MSG_TRUNC		= 0x20,
#define	MSG_TRUNC	MSG_TRUNC
    MSG_DONTWAIT	= 0x40, /* Nonblocking IO.  */
#define	MSG_DONTWAIT	MSG_DONTWAIT
    MSG_EOR		= 0x80, /* End of record.  */
#define	MSG_EOR		MSG_EOR
    MSG_WAITALL		= 0x100, /* Wait for a full request.  */
#define	MSG_WAITALL	MSG_WAITALL
    MSG_FIN		= 0x200,
#define	MSG_FIN		MSG_FIN
    MSG_SYN		= 0x400,
#define	MSG_SYN		MSG_SYN
    MSG_CONFIRM		= 0x800, /* Confirm path validity.  */
#define	MSG_CONFIRM	MSG_CONFIRM
    MSG_RST		= 0x1000,
#define	MSG_RST		MSG_RST
    MSG_ERRQUEUE	= 0x2000, /* Fetch message from error queue.  */
#define	MSG_ERRQUEUE	MSG_ERRQUEUE
    MSG_NOSIGNAL	= 0x4000, /* Do not generate SIGPIPE.  */
#define	MSG_NOSIGNAL	MSG_NOSIGNAL
    MSG_MORE		= 0x8000  /* Sender will send more.  */
#define	MSG_MORE	MSG_MORE
};


#ifdef __cplusplus
extern "C" {
#endif

//Œ¥ µœ÷

QKCAPI int socket(int domain , int type , int protocol) ;

QKCAPI int bind(int fd , const struct sockaddr * addr , socklen_t len);

QKCAPI int getsockname(int fd , struct sockaddr * addr , socklen_t * len) ;

QKCAPI int connect(int fd , const struct sockaddr * addr , socklen_t len) ;

QKCAPI int getpeername(int fd , struct sockaddr * addr , socklen_t * len) ;

QKCAPI ssize_t send(int fd , const void * buf , size_t n , int flags);

QKCAPI ssize_t recv(int fd , void *buf , size_t n , int flags);

QKCAPI ssize_t sendto(int fd , const void *buf , size_t n , int flags , const struct sockaddr * addr , socklen_t addr_len);

QKCAPI ssize_t recvfrom(int fd , void * buf , size_t n , int flags , struct sockaddr * addr , socklen_t * addr_len);

QKCAPI ssize_t sendmsg(int fd , const struct msghdr * message , int flags);

QKCAPI ssize_t recvmsg(int fd , struct msghdr *message , int flags);

QKCAPI int getsockopt(int fd , int level , int optname , void * optval , socklen_t * optlen) ;

QKCAPI int setsockopt(int fd , int level , int optname , const void *optval , socklen_t optlen) ;

QKCAPI int listen(int fd , int n) ;

QKCAPI int accept(int fd , struct sockaddr * addr , socklen_t * addr_len);

QKCAPI int shutdown(int fd , int how) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_SOCKET_H */


#ifndef __QUARK_BITS_SOCKADDR_H
#define __QUARK_BITS_SOCKADDR_H	1


typedef unsigned short int sa_family_t;

#define	__SOCKADDR_COMMON(sa_prefix) sa_family_t sa_prefix##family

#define __SOCKADDR_COMMON_SIZE	(sizeof (unsigned short int))

#endif	/** __QUARK_BITS_SOCKADDR_H */

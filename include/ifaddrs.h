
#ifndef __QKC_IFADDRS_H
#define __QKC_IFADDRS_H	1

#include <quark_compile.h>
#include <sys/socket.h>

__BEGIN_DECLS

struct ifaddrs
{
  struct ifaddrs *ifa_next;

  char *ifa_name;		
  unsigned int ifa_flags;

  struct sockaddr *ifa_addr;
  struct sockaddr *ifa_netmask;
  union
  {
    struct sockaddr *ifu_broadaddr;
    struct sockaddr *ifu_dstaddr;
  } ifa_ifu;
# ifndef ifa_broadaddr
#  define ifa_broadaddr	ifa_ifu.ifu_broadaddr
# endif
# ifndef ifa_dstaddr
#  define ifa_dstaddr	ifa_ifu.ifu_dstaddr
# endif

  void *ifa_data;
};

QKCAPI int getifaddrs (struct ifaddrs **ifap) ;
QKCAPI void freeifaddrs (struct ifaddrs * ifa)  ;

__END_DECLS

#endif /** __QKC_IFADDRS_H */

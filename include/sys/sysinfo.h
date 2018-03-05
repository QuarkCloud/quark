
#ifndef __QKC_SYS_SYSINFO_H
#define __QKC_SYS_SYSINFO_H 1

#include <quark_compile.h>
#include <sys/types.h>

#ifdef	__cplusplus
extern "C" {
#endif

struct sysinfo {
  long uptime;                /* Seconds since boot */
  unsigned long loads[3];     /* 1, 5, and 15 minute load averages */
  unsigned long totalram;     /* Total usable main memory size */
  unsigned long freeram;      /* Available memory size */
  unsigned long sharedram;    /* Amount of shared memory */
  unsigned long bufferram;    /* Memory used by buffers */
  unsigned long totalswap;    /* Total swap space size */
  unsigned long freeswap;     /* swap space still available */
  unsigned short procs;       /* Number of current processes */
  unsigned long totalhigh;    /* Total high memory size */
  unsigned long freehigh;     /* Available high memory size */
  unsigned int mem_unit;      /* Memory unit size in bytes */
  char __f[10];               /* Pads structure to 64 bytes */
};

QKCAPI int sysinfo (struct sysinfo * info) ;
QKCAPI int get_nprocs_conf (void) ;
QKCAPI int get_nprocs (void) ;
QKCAPI long int get_phys_pages (void) ;
QKCAPI long int get_avphys_pages (void) ;

#ifdef	__cplusplus
}
#endif

#endif	/** __QKC_SYS_SYSINFO_H */

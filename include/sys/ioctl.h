#ifndef __QKC_SYS_IOCTL_H
#define __QKC_SYS_IOCTL_H 1

#include <quark_compile.h>


#ifdef __cplusplus
extern "C" {
#endif

#define FIONREAD	0x541B
#define FIONBIO		0x5421
#define FIONCLEX	0x5450
#define FIOCLEX		0x5451
#define FIOASYNC	0x5452

QKCAPI int ioctl(int fd , unsigned long request , ...);

#ifdef __cplusplus
}
#endif


#endif /** __QKC_SYS_IOCTL_H */

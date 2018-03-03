
#ifndef __QKC_WINTF_WOBJ_H
#define __QKC_WINTF_WOBJ_H 1

#include <quark_compile.h>

#ifdef	__cplusplus
extern "C" {
#endif

QUARK_LINKAGE intptr_t _get_osfhandle(int fd);
QUARK_LINKAGE int _open_osfhandle(intptr_t handle, int flags);
QUARK_LINKAGE int _open(const char * filename , int oflag , int mode = 0);
QUARK_LINKAGE int _unlink(const char * filename);

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WOBJ_H */

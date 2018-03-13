
#ifndef __QKC_WINTF_WTIME_H
#define __QKC_WINTF_WTIME_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>
#include <time.h>

#ifdef	__cplusplus
extern "C" {
#endif
//从1970年01月01日，精度100纳秒
QKCAPI uint64_t GetWinHrTime() ;

//精度毫秒
QKCAPI uint64_t ElapseToMSec(const struct timespec * ts) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WTIME_H */

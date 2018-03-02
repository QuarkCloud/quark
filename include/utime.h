
#ifndef __QKC_UTIME_H
#define __QKC_UTIME_H 1

#include <quark_compile.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct utimbuf
{
    time_t actime;		/* Access time.  */
    time_t modtime;		/* Modification time.  */
};

QKCAPI int utime (const char * file, const struct utimbuf * file_times);


#ifdef __cplusplus
}
#endif

#endif /** __QKC_UTIME_H */

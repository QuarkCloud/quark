
#ifndef __QKC_UTIME_H
#define __QKC_UTIME_H 1

#include <quark_compile.h>
#include <time.h>

__BEGIN_DECLS

struct utimbuf
{
    time_t actime;		/* Access time.  */
    time_t modtime;		/* Modification time.  */
};

QKCAPI int utime (const char * file, const struct utimbuf * file_times);

__END_DECLS

#endif /** __QKC_UTIME_H */

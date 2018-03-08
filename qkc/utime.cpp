
#include <utime.h>
#include <wintf/wcrt.h>

int utime (const char * file, const struct utimbuf * file_times)
{
    struct utimbuf  ftimes ;
    if(file_times == NULL)
    {
        ftimes.actime = 0 ;
        ftimes.modtime = 0 ;
    }
    else
    {
        ftimes.actime = file_times->actime ;
        ftimes.modtime = file_times->modtime ;
    }

    return ::_utime64(file , &ftimes) ;
}



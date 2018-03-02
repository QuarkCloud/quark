
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

QUARK_LINKAGE char *tmpnam_s (char *s);
QUARK_LINKAGE int _fcloseall (void);
QUARK_LINKAGE FILE *_fdopen (int fd , const char *modes);
QUARK_LINKAGE int _snprintf (char * s , size_t maxlen , const char * format , ...);
QUARK_LINKAGE int _vsnprintf (char * s , size_t maxlen , const char * format , va_list arg);
QUARK_LINKAGE int _getw (FILE *stream);
QUARK_LINKAGE int _putw (int w , FILE *stream);

QUARK_LINKAGE int _fseeki64 (FILE *stream , off_t off , int whence);
QUARK_LINKAGE off_t _ftelli64 (FILE *stream);
QUARK_LINKAGE int _fileno (FILE *stream);

QUARK_LINKAGE void _lock_file (FILE *stream);
QUARK_LINKAGE void _unlock_file (FILE *stream);


#ifdef __cplusplus
}
#endif

char *tmpnam_r (char *s)
{
    return tmpnam_s(s) ;
}

int fcloseall (void)
{
    return _fcloseall() ;
}

FILE *fdopen (int fd , const char *modes)
{
    return _fdopen(fd , modes) ;
}

int getw (FILE *stream)
{
    return _getw(stream) ;
}

int putw (int w , FILE *stream)
{
    return _putw(w , stream) ;
}

int fseeko (FILE *stream , off_t off , int whence)
{
    return _fseeki64(stream , off , whence) ;
}

off_t ftello (FILE *stream)
{
    return _ftelli64(stream) ;
}


void flockfile (FILE *stream)
{
    _lock_file(stream) ;
}

void funlockfile (FILE *stream)
{
    _unlock_file(stream) ;
}

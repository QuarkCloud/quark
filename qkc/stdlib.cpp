
#include <stdlib.h>
#include <errno.h>
#include <wintf/wcrt.h>

#ifdef __cplusplus
extern "C" {
#endif

QUARK_LINKAGE errno_t rand_s (unsigned int * value);
QUARK_LINKAGE errno_t getenv_s(size_t * return_size , char * dst_buf , size_t dst_size, const char * name);


QUARK_LINKAGE int _putenv(char *str);
QUARK_LINKAGE int _putenv_s(const char *envname, const char *envval) ;


#ifdef __cplusplus
}
#endif

int rand_r (unsigned int * seed)
{
    unsigned int value = 0 ;
    rand_s(&value) ;
    return (int)value ;
}

char * secure_getenv (const char *name) 
{
    //TO-DO
    return NULL ;
}

int putenv(char *str)
{
    return _putenv(str) ;
}

int setenv(const char *envname, const char *envval, int overwrite)
{
    return _putenv_s(envname , envval) ;
}


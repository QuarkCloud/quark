
#include <stdlib.h>
#include <errno.h>
#include <wintf/wcrt.h>

#ifdef __cplusplus
extern "C" {
#endif

QUARK_LINKAGE errno_t rand_s (unsigned int * value);
QUARK_LINKAGE errno_t getenv_s(size_t * return_size , char * dst_buf , size_t dst_size, const char * name);

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

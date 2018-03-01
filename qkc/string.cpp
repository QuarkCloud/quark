
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

QUARK_LINKAGE char *  strtok_s(char * str , const char * delim,  char ** context);

#ifdef __cplusplus
}
#endif

char *strtok_r (char * s, const char * delim,  char ** save_ptr)
{
    return strtok_s(s , delim , save_ptr) ;
}


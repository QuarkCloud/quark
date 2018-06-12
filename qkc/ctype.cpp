
#include <ctype.h>


#ifdef __cplusplus
extern "C" {
#endif

QUARK_LINKAGE int _isctype (int c , int mask);

#ifdef __cplusplus
}
#endif

int isblank (int c) 
{
    if(c == ' ' || c == '\t')
        return  1;
    else
        return 0 ;
}


int isctype (int c , int mask)
{
    return _isctype(c , mask) ;
}


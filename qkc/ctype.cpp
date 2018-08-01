
#include <ctype.h>


__BEGIN_DECLS

QUARK_LINKAGE int _isctype (int c , int mask);

__END_DECLS

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


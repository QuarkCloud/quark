
#include <ctype.h>
#include "wobjs/CrtLinker.h"


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


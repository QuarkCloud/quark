
#include <string.h>
#include <wintf/wcrt.h>

char *strtok_r (char * s, const char * delim,  char ** save_ptr)
{
    return strtok_s(s , delim , save_ptr) ;
}


#include <stdint.h>
#include <sys/ipc.h>
#include <windows.h>

key_t ftok (const char *pathname , int proj_id)
{
    ATOM atom = ::GlobalAddAtom(pathname) ;
    if(atom == 0)
        return (key_t)-1 ;

    key_t key = atom ;
    key = (key << 16) + (uint8_t)proj_id ;
    return key ;
}


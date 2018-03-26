#include <stdint.h>
#include <string.h>
#include <sys/ipc.h>
#include "inner/hash.h"

key_t ftok (const char *pathname , int proj_id)
{
    uint32_t hash_value = ::hash_time33(pathname , ::strlen(pathname) , 0) ;
    key_t key = (hash_value & ((uint32_t)(-1) << 16)) + (uint8_t)proj_id ;
    return key ;
}


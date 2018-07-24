
#include <grp.h>

void setgrent (void)
{
    //
}

struct group *getgrgid (gid_t gid)
{
    return NULL ;
}

struct group *getgrnam (const char *name)
{
    return NULL ;
}

int setgroups (size_t n, const gid_t *groups)
{
    return -1 ;
}

int initgroups (const char *user, gid_t group)
{
    return 0 ;
}



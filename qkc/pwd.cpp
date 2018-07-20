
#include <pwd.h>


void setpwent (void)
{
    //
}

void endpwent (void)
{
    //
}

struct passwd *getpwent (void)
{
    return NULL ;
}

struct passwd *getpwuid (uid_t uid)
{
    return NULL ;
}

struct passwd *getpwnam (const char * name)
{
    return NULL ;
}

int getpw (uid_t uid, char *buffer)
{
    return 0 ;
}

int getpwuid_r(uid_t uid,struct passwd * resultbuf,
       char * buffer, size_t buflen,struct passwd **_result)
{
    return 0 ;
}


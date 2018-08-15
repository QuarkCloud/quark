
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <windows.h>
#include <userenv.h>
#include <advapi32.h>


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
    ::memset(resultbuf , 0 , sizeof(struct passwd)) ;
    char str[1024] ;
    int slen = 0 ;
    char * pchar = buffer ;
    int offset = 0 ;

    slen = _imp_get_user_directory(str , sizeof(str)) ;
    if(slen > 0)
    {
        ::memcpy(pchar + offset, str , slen) ;
        pchar[slen + offset] = '\0' ;
        resultbuf->pw_dir = pchar + offset;
        offset += (slen + 1);
    }

    slen = _imp_get_username(str , sizeof(str)) ;
    if(slen > 0)
    {
        ::memcpy(pchar + offset, str , slen) ;
        pchar[slen + offset] = '\0' ;
        resultbuf->pw_name = pchar + offset;
        offset += (slen + 1);
    }

    pchar[offset] = '\0' ;
    resultbuf->pw_passwd = pchar + offset ;
    ++offset ;

    resultbuf->pw_uid = -1 ;
    resultbuf->pw_gid = -1 ;

    pchar[offset] = '\0' ;
    resultbuf->pw_gecos = pchar + offset ;
    ++offset ;

    pchar[offset] = '\0' ;
    resultbuf->pw_shell = pchar + offset ;
    ++offset ;

    if(offset > 0)
    {
        if(_result != NULL)
            *_result = resultbuf ;
        return 0 ;
    }
    else
        return -1 ;
}



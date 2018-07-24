
#ifndef	__QKC_GRP_H
#define	__QKC_GRP_H	1

#include <quark_compile.h>
#include <sys/types.h>

__BEGIN_DECLS


struct group
{
    char *gr_name;		/* Group name.	*/
    char *gr_passwd;		/* Password.	*/
    gid_t gr_gid;		/* Group ID.	*/
    char **gr_mem;		/* Member list.	*/
};

QKCAPI void setgrent (void);

QKCAPI struct group *getgrgid (gid_t gid);

QKCAPI struct group *getgrnam (const char *name);

QKCAPI int setgroups (size_t n, const gid_t *groups) ;

QKCAPI int initgroups (const char *user, gid_t group);

__END_DECLS

#endif /** __QKC_GRP_H  */


#ifndef	__QKC_PWD_H
#define	__QKC_PWD_H	1

#include <quark_compile.h>
#include <sys/types.h>

__BEGIN_DECLS


struct passwd
{
  char *pw_name;		/* Username.  */
  char *pw_passwd;		/* Password.  */
  uid_t pw_uid;		    /* User ID.  */
  gid_t pw_gid;		    /* Group ID.  */
  char *pw_gecos;		/* Real name.  */
  char *pw_dir;			/* Home directory.  */
  char *pw_shell;		/* Shell program.  */
};


QKCAPI void setpwent (void);

QKCAPI void endpwent (void);

QKCAPI struct passwd *getpwent (void);

QKCAPI struct passwd *getpwuid (uid_t uid);

QKCAPI struct passwd *getpwnam (const char * name);

QKCAPI int getpw (uid_t uid, char *buffer);

QKCAPI int getpwuid_r(uid_t uid,struct passwd * resultbuf,
       char * buffer, size_t buflen,struct passwd **_result);


__END_DECLS

#endif  /** __QKC_PWD_H */
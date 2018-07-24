
#ifndef __QKC_BITS_WAITFLAGS_H
#define __QKC_BITS_WAITFLAGS_H 1

#include <quark_compile.h>

__BEGIN_DECLS


/* Bits in the third argument to `waitpid'.  */
#define	WNOHANG		1	/* Don't block waiting.  */
#define	WUNTRACED	2	/* Report status of stopped children.  */

/* Bits in the fourth argument to `waitid'.  */
#define WSTOPPED	2	/* Report stopped child (same as WUNTRACED). */
#define WEXITED		4	/* Report dead child.  */
#define WCONTINUED	8	/* Report continued child.  */
#define WNOWAIT		0x01000000 /* Don't reap, just poll status.  */

#define __WNOTHREAD     0x20000000 /* Don't wait on children of other threads
				      in this group */
#define __WALL		0x40000000 /* Wait for any child.  */
#define __WCLONE	0x80000000 /* Wait for cloned process.  */

__END_DECLS

#endif  /** __QKC_BITS_WAITFLAGS_H */

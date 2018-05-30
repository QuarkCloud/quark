
#ifndef __QKC_SYS_PARAM_H
#define __QKC_SYS_PARAM_H    1

#include <limits.h>
//#include <linux/limits.h>
//#include <linux/param.h>

/* BSD names for some <limits.h> values.  */

#define NBBY            CHAR_BIT
#ifndef NGROUPS
# define NGROUPS        NGROUPS_MAX
#endif
#define MAXSYMLINKS     20
#define CANBSIZ         MAX_CANON
#define NCARGS          ARG_MAX
#define MAXPATHLEN      PATH_MAX
/* The following is not really correct but it is a value we used for a
   long time and which seems to be usable.  People should not use NOFILE
   anyway.  */
#define NOFILE          256


#include <sys/types.h>

/* Bit map related macros.  */
#define setbit(a,i)     ((a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define clrbit(a,i)     ((a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define isset(a,i)      ((a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define isclr(a,i)      (((a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

/* Macros for counting and rounding.  */
#ifndef howmany
# define howmany(x, y)  (((x) + ((y) - 1)) / (y))
#endif
# define roundup(x, y)  ((((x) + ((y) - 1)) / (y)) * (y))

#define powerof2(x)     ((((x) - 1) & (x)) == 0)

/* Macros for min/max.  */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


/* Unit of `st_blocks'.  */
#define DEV_BSIZE       512

#endif  /* __QKC_SYS_PARAM_H */

                                                                                                                               
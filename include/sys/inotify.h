#ifndef __QKC_SYS_INOTIFY_H
#define __QKC_SYS_INOTIFY_H 1

#include <stdint.h>


/* Structure describing an inotify event.  */
struct inotify_event
{
  int wd;		/* Watch descriptor.  */
  uint32_t mask;	/* Watch mask.  */
  uint32_t cookie;	/* Cookie to synchronize two events.  */
  uint32_t len;		/* Length (including NULs) of name.  */
  char name ;	/* Name.  */
};


/* Supported events suitable for MASK parameter of INOTIFY_ADD_WATCH.  */
#define IN_ACCESS	 0x00000001	/* File was accessed.  */
#define IN_MODIFY	 0x00000002	/* File was modified.  */
#define IN_ATTRIB	 0x00000004	/* Metadata changed.  */
#define IN_CLOSE_WRITE	 0x00000008	/* Writtable file was closed.  */
#define IN_CLOSE_NOWRITE 0x00000010	/* Unwrittable file closed.  */
#define IN_CLOSE	 (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) /* Close.  */
#define IN_OPEN		 0x00000020	/* File was opened.  */
#define IN_MOVED_FROM	 0x00000040	/* File was moved from X.  */
#define IN_MOVED_TO      0x00000080	/* File was moved to Y.  */
#define IN_MOVE		 (IN_MOVED_FROM | IN_MOVED_TO) /* Moves.  */
#define IN_CREATE	 0x00000100	/* Subfile was created.  */
#define IN_DELETE	 0x00000200	/* Subfile was deleted.  */
#define IN_DELETE_SELF	 0x00000400	/* Self was deleted.  */
#define IN_MOVE_SELF	 0x00000800	/* Self was moved.  */

/* Events sent by the kernel.  */
#define IN_UNMOUNT	 0x00002000	/* Backing fs was unmounted.  */
#define IN_Q_OVERFLOW	 0x00004000	/* Event queued overflowed.  */
#define IN_IGNORED	 0x00008000	/* File was ignored.  */

/* Helper events.  */
#define IN_CLOSE	 (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)	/* Close.  */
#define IN_MOVE		 (IN_MOVED_FROM | IN_MOVED_TO)		/* Moves.  */

/* Special flags.  */
#define IN_ONLYDIR	 0x01000000	/* Only watch the path if it is a
					   directory.  */
#define IN_DONT_FOLLOW	 0x02000000	/* Do not follow a sym link.  */
#define IN_MASK_ADD	 0x20000000	/* Add to the mask of an already
					   existing watch.  */
#define IN_ISDIR	 0x40000000	/* Event occurred against dir.  */
#define IN_ONESHOT	 0x80000000	/* Only send event once.  */

/* All events which a program can wait on.  */
#define IN_ALL_EVENTS	 (IN_ACCESS | IN_MODIFY | IN_ATTRIB | IN_CLOSE_WRITE  \
			  | IN_CLOSE_NOWRITE | IN_OPEN | IN_MOVED_FROM	      \
			  | IN_MOVED_TO | IN_CREATE | IN_DELETE		      \
			  | IN_DELETE_SELF | IN_MOVE_SELF)


#ifdef __cplusplus
extern "C" {
#endif

QKCAPI int inotify_init (void) ;

QKCAPI int inotify_add_watch (int fd, const char *name, uint32_t mask) ;

QKCAPI int inotify_rm_watch (int fd, uint32_t wd) ;

#ifdef __cplusplus
}
#endif

#endif /** __QKC_SYS_INOTIFY_H */



#ifndef __QKC_TERMIOS_H
#define	__QKC_TERMIOS_H	1

#include <quark_compile.h>
#include <sys/types.h>
#include <bits/termios.h>

__BEGIN_DECLS

QKCAPI int tcgetattr (int fd, struct termios * termios_p) ;

QKCAPI int tcsetattr (int fd, int optional_actions, const struct termios * termios_p) ;

QKCAPI void cfmakeraw (struct termios *termios_p) ;

QKCAPI int tcsendbreak (int fd, int duration) ;

QKCAPI int tcdrain (int fd);

QKCAPI int tcflush (int fd, int queue_selector) ;

QKCAPI int tcflow (int fd, int action) ;

QKCAPI pid_t tcgetsid (int fd) ;

__END_DECLS

#endif /** __QKC_TERMIOS_H */


#include <termios.h>


int tcgetattr (int fd, struct termios * termios_p)
{
    return -1 ;
}

int tcsetattr (int fd, int optional_actions, const struct termios * termios_p)
{
    return -1 ;
}

void cfmakeraw (struct termios *termios_p)
{
    //
}

int tcsendbreak (int fd, int duration)
{
    return -1 ;
}

int tcdrain (int fd)
{
    return -1 ;
}

int tcflush (int fd, int queue_selector)
{
    return -1 ;
}

int tcflow (int fd, int action)
{
    return -1 ;
}

pid_t tcgetsid (int fd)
{
    return -1 ;
}

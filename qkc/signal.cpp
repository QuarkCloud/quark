
#include <signal.h>

int kill (pid_t pid, int sig)
{
    return 0 ;
}

int sigemptyset (sigset_t * sigs)
{
    return 0 ;
}

int sigfillset (sigset_t *sigs)
{
    return 0 ;
}

int sigaddset (sigset_t *sigs, int signo)
{
    return 0 ;
}

int sigdelset (sigset_t *sigs, int signo)
{
    return 0 ;
}

int sigismember (const sigset_t *sigs, int signo)
{
    return 0 ;
}

int sigisemptyset (const sigset_t *sigs)
{
    return 0 ;
}

int sigandset (sigset_t *sigs, const sigset_t *left, const sigset_t * right)
{
    return 0 ;
}

int sigorset (sigset_t *sigs, const sigset_t *left , const sigset_t *right)
{
    return 0 ;
}


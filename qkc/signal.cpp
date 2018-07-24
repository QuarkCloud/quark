
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


int sigprocmask (int how, const sigset_t * set, sigset_t * oset)
{
    return -1 ;
}

int sigsuspend (const sigset_t *set)
{
    return -1 ;
}

int sigaction (int sig, const struct sigaction * act, struct sigaction * oact)
{
    return -1 ;
}

int sigpending (sigset_t *set)
{
    return -1 ;
}

int sigwait (const sigset_t *set, int * sig)
{
    return -1 ;
}

int sigwaitinfo (const sigset_t * set,siginfo_t * info)
{
    return -1 ;
}

int sigtimedwait (const sigset_t * set,siginfo_t * info , const struct timespec * timeout)
{
    return -1 ;
}

int sigqueue (pid_t pid, int sig, const union sigval val)
{
    return -1 ;
}


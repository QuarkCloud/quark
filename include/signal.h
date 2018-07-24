
#ifndef __QKC_SIGNAL_H
#define __QKC_SIGNAL_H 1

#include <quark_compile.h>
#include <sys/types.h>
#include <bits/siginfo.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* Fake signal functions.  */
#define SIG_ERR	((__sighandler_t) -1)		/* Error return.  */
#define SIG_DFL	((__sighandler_t) 0)		/* Default action.  */
#define SIG_IGN	((__sighandler_t) 1)		/* Ignore signal.  */
#define SIG_HOLD	((__sighandler_t) 2)	/* Add signal to hold mask.  */


/* Signals.  */
#define	SIGHUP		1	/* Hangup (POSIX).  */
#define	SIGINT		2	/* Interrupt (ANSI).  */
#define	SIGQUIT		3	/* Quit (POSIX).  */
#define	SIGILL		4	/* Illegal instruction (ANSI).  */
#define	SIGTRAP		5	/* Trace trap (POSIX).  */
#define	SIGABRT		6	/* Abort (ANSI).  */
#define	SIGIOT		6	/* IOT trap (4.2 BSD).  */
#define	SIGBUS		7	/* BUS error (4.2 BSD).  */
#define	SIGFPE		8	/* Floating-point exception (ANSI).  */
#define	SIGKILL		9	/* Kill, unblockable (POSIX).  */
#define	SIGUSR1		10	/* User-defined signal 1 (POSIX).  */
#define	SIGSEGV		11	/* Segmentation violation (ANSI).  */
#define	SIGUSR2		12	/* User-defined signal 2 (POSIX).  */
#define	SIGPIPE		13	/* Broken pipe (POSIX).  */
#define	SIGALRM		14	/* Alarm clock (POSIX).  */
#define	SIGTERM		15	/* Termination (ANSI).  */
#define	SIGSTKFLT	16	/* Stack fault.  */
#define	SIGCLD		SIGCHLD	/* Same as SIGCHLD (System V).  */
#define	SIGCHLD		17	/* Child status has changed (POSIX).  */
#define	SIGCONT		18	/* Continue (POSIX).  */
#define	SIGSTOP		19	/* Stop, unblockable (POSIX).  */
#define	SIGTSTP		20	/* Keyboard stop (POSIX).  */
#define	SIGTTIN		21	/* Background read from tty (POSIX).  */
#define	SIGTTOU		22	/* Background write to tty (POSIX).  */
#define	SIGURG		23	/* Urgent condition on socket (4.2 BSD).  */
#define	SIGXCPU		24	/* CPU limit exceeded (4.2 BSD).  */
#define	SIGXFSZ		25	/* File size limit exceeded (4.2 BSD).  */
#define	SIGVTALRM	26	/* Virtual alarm clock (4.2 BSD).  */
#define	SIGPROF		27	/* Profiling alarm clock (4.2 BSD).  */
#define	SIGWINCH	28	/* Window size change (4.3 BSD, Sun).  */
#define	SIGPOLL		SIGIO	/* Pollable event occurred (System V).  */
#define	SIGIO		29	/* I/O now possible (4.2 BSD).  */
#define	SIGPWR		30	/* Power failure restart (System V).  */
#define SIGSYS		31	/* Bad system call.  */
#define SIGUNUSED	31

#define	_NSIG		65	/* Biggest signal number + 1 (including real-time signals).  */

typedef void (*__sighandler_t) (int);

//typedef int __sig_atomic_t;

/* A `sigset_t' has a bit for each signal.  */
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;
typedef __sigset_t sigset_t ;


struct sigaction
{
    /* Signal handler.  */
    __sighandler_t sa_handler;

    /* Additional set of signals to be blocked.  */
    __sigset_t sa_mask;

    /* Special flags.  */
    int sa_flags;

    /* Restore handler.  */
    void (*sa_restorer) (void);
};

/* Bits in `sa_flags'.  */
#define SA_NOCLDSTOP  1          /* Don't send SIGCHLD when children stop.  */
#define SA_NOCLDWAIT  2          /* Don't create zombie on child death.  */
#define SA_SIGINFO    4          /* Invoke signal-catching function with
                                    three arguments instead of one.  */

/* Values for the HOW argument to `sigprocmask'.  */
#define SIG_BLOCK     0          /* Block signals.  */
#define SIG_UNBLOCK   1          /* Unblock signals.  */
#define SIG_SETMASK   2          /* Set the set of blocked signals.  */

QUARK_LINKAGE __sighandler_t signal (int sig, __sighandler_t handler) ;

QKCAPI int kill (pid_t pid, int sig) ;

QUARK_LINKAGE int raise (int sig) ;

QKCAPI int sigemptyset (sigset_t * sigs) ;

QKCAPI int sigfillset (sigset_t *sigs) ;

QKCAPI int sigaddset (sigset_t *sigs, int signo) ;

QKCAPI int sigdelset (sigset_t *sigs, int signo) ;

QKCAPI int sigismember (const sigset_t *sigs, int signo) ;

QKCAPI int sigisemptyset (const sigset_t *sigs) ;

QKCAPI int sigandset (sigset_t *sigs, const sigset_t *left, const sigset_t * right) ;

QKCAPI int sigorset (sigset_t *sigs, const sigset_t *left , const sigset_t *right) ;

QKCAPI int sigprocmask (int how, const sigset_t * set, sigset_t * oset) ;

QKCAPI int sigsuspend (const sigset_t *set) ;

QKCAPI int sigaction (int sig, const struct sigaction * act, struct sigaction * oact) ;

QKCAPI int sigpending (sigset_t *set) ;

QKCAPI int sigwait (const sigset_t *set, int * sig);

QKCAPI int sigwaitinfo (const sigset_t * set,siginfo_t * info) ;

QKCAPI int sigtimedwait (const sigset_t * set,siginfo_t * info , const struct timespec * timeout);

QKCAPI int sigqueue (pid_t pid, int sig, const union sigval val) ;

#ifdef	__cplusplus
}
#endif


#endif /**__QKC_SIGNAL_H*/


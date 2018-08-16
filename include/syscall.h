#ifndef __QKC_SYSCALL_H
#define __QKC_SYSCALL_H 1

#include <quark_compile.h>
#include <bits/wordsize.h>

#if __WORDSIZE == 32       
 /** i386 系统，是32位的*/

#define __NR_restart_syscall      0
#define __NR_exit		  1
#define __NR_fork		  2
#define __NR_read		  3
#define __NR_write		  4
#define __NR_open		  5
#define __NR_close		  6
#define __NR_waitpid		  7
#define __NR_creat		  8
#define __NR_link		  9
#define __NR_unlink		 10
#define __NR_execve		 11
#define __NR_chdir		 12
#define __NR_time		 13
#define __NR_mknod		 14
#define __NR_chmod		 15
#define __NR_lchown		 16
#define __NR_break		 17
#define __NR_oldstat		 18
#define __NR_lseek		 19
#define __NR_getpid		 20
#define __NR_mount		 21
#define __NR_umount		 22
#define __NR_setuid		 23
#define __NR_getuid		 24
#define __NR_stime		 25
#define __NR_ptrace		 26
#define __NR_alarm		 27
#define __NR_oldfstat		 28
#define __NR_pause		 29
#define __NR_utime		 30
#define __NR_stty		 31
#define __NR_gtty		 32
#define __NR_access		 33
#define __NR_nice		 34
#define __NR_ftime		 35
#define __NR_sync		 36
#define __NR_kill		 37
#define __NR_rename		 38
#define __NR_mkdir		 39
#define __NR_rmdir		 40
#define __NR_dup		 41
#define __NR_pipe		 42
#define __NR_times		 43
#define __NR_prof		 44
#define __NR_brk		 45
#define __NR_setgid		 46
#define __NR_getgid		 47
#define __NR_signal		 48
#define __NR_geteuid		 49
#define __NR_getegid		 50
#define __NR_acct		 51
#define __NR_umount2		 52
#define __NR_lock		 53
#define __NR_ioctl		 54
#define __NR_fcntl		 55
#define __NR_mpx		 56
#define __NR_setpgid		 57
#define __NR_ulimit		 58
#define __NR_oldolduname	 59
#define __NR_umask		 60
#define __NR_chroot		 61
#define __NR_ustat		 62
#define __NR_dup2		 63
#define __NR_getppid		 64
#define __NR_getpgrp		 65
#define __NR_setsid		 66
#define __NR_sigaction		 67
#define __NR_sgetmask		 68
#define __NR_ssetmask		 69
#define __NR_setreuid		 70
#define __NR_setregid		 71
#define __NR_sigsuspend		 72
#define __NR_sigpending		 73
#define __NR_sethostname	 74
#define __NR_setrlimit		 75
#define __NR_getrlimit		 76	/* Back compatible 2Gig limited rlimit */
#define __NR_getrusage		 77
#define __NR_gettimeofday	 78
#define __NR_settimeofday	 79
#define __NR_getgroups		 80
#define __NR_setgroups		 81
#define __NR_select		 82
#define __NR_symlink		 83
#define __NR_oldlstat		 84
#define __NR_readlink		 85
#define __NR_uselib		 86
#define __NR_swapon		 87
#define __NR_reboot		 88
#define __NR_readdir		 89
#define __NR_mmap		 90
#define __NR_munmap		 91
#define __NR_truncate		 92
#define __NR_ftruncate		 93
#define __NR_fchmod		 94
#define __NR_fchown		 95
#define __NR_getpriority	 96
#define __NR_setpriority	 97
#define __NR_profil		 98
#define __NR_statfs		 99
#define __NR_fstatfs		100
#define __NR_ioperm		101
#define __NR_socketcall		102
#define __NR_syslog		103
#define __NR_setitimer		104
#define __NR_getitimer		105
#define __NR_stat		106
#define __NR_lstat		107
#define __NR_fstat		108
#define __NR_olduname		109
#define __NR_iopl		110
#define __NR_vhangup		111
#define __NR_idle		112
#define __NR_vm86old		113
#define __NR_wait4		114
#define __NR_swapoff		115
#define __NR_sysinfo		116
#define __NR_ipc		117
#define __NR_fsync		118
#define __NR_sigreturn		119
#define __NR_clone		120
#define __NR_setdomainname	121
#define __NR_uname		122
#define __NR_modify_ldt		123
#define __NR_adjtimex		124
#define __NR_mprotect		125
#define __NR_sigprocmask	126
#define __NR_create_module	127
#define __NR_init_module	128
#define __NR_delete_module	129
#define __NR_get_kernel_syms	130
#define __NR_quotactl		131
#define __NR_getpgid		132
#define __NR_fchdir		133
#define __NR_bdflush		134
#define __NR_sysfs		135
#define __NR_personality	136
#define __NR_afs_syscall	137 /* Syscall for Andrew File System */
#define __NR_setfsuid		138
#define __NR_setfsgid		139
#define __NR__llseek		140
#define __NR_getdents		141
#define __NR__newselect		142
#define __NR_flock		143
#define __NR_msync		144
#define __NR_readv		145
#define __NR_writev		146
#define __NR_getsid		147
#define __NR_fdatasync		148
#define __NR__sysctl		149
#define __NR_mlock		150
#define __NR_munlock		151
#define __NR_mlockall		152
#define __NR_munlockall		153
#define __NR_sched_setparam		154
#define __NR_sched_getparam		155
#define __NR_sched_setscheduler		156
#define __NR_sched_getscheduler		157
#define __NR_sched_yield		158
#define __NR_sched_get_priority_max	159
#define __NR_sched_get_priority_min	160
#define __NR_sched_rr_get_interval	161
#define __NR_nanosleep		162
#define __NR_mremap		163
#define __NR_setresuid		164
#define __NR_getresuid		165
#define __NR_vm86		166
#define __NR_query_module	167
#define __NR_poll		168
#define __NR_nfsservctl		169
#define __NR_setresgid		170
#define __NR_getresgid		171
#define __NR_prctl              172
#define __NR_rt_sigreturn	173
#define __NR_rt_sigaction	174
#define __NR_rt_sigprocmask	175
#define __NR_rt_sigpending	176
#define __NR_rt_sigtimedwait	177
#define __NR_rt_sigqueueinfo	178
#define __NR_rt_sigsuspend	179
#define __NR_pread64		180
#define __NR_pwrite64		181
#define __NR_chown		182
#define __NR_getcwd		183
#define __NR_capget		184
#define __NR_capset		185
#define __NR_sigaltstack	186
#define __NR_sendfile		187
#define __NR_getpmsg		188	/* some people actually want streams */
#define __NR_putpmsg		189	/* some people actually want streams */
#define __NR_vfork		190
#define __NR_ugetrlimit		191	/* SuS compliant getrlimit */
#define __NR_mmap2		192
#define __NR_truncate64		193
#define __NR_ftruncate64	194
#define __NR_stat64		195
#define __NR_lstat64		196
#define __NR_fstat64		197
#define __NR_lchown32		198
#define __NR_getuid32		199
#define __NR_getgid32		200
#define __NR_geteuid32		201
#define __NR_getegid32		202
#define __NR_setreuid32		203
#define __NR_setregid32		204
#define __NR_getgroups32	205
#define __NR_setgroups32	206
#define __NR_fchown32		207
#define __NR_setresuid32	208
#define __NR_getresuid32	209
#define __NR_setresgid32	210
#define __NR_getresgid32	211
#define __NR_chown32		212
#define __NR_setuid32		213
#define __NR_setgid32		214
#define __NR_setfsuid32		215
#define __NR_setfsgid32		216
#define __NR_pivot_root		217
#define __NR_mincore		218
#define __NR_madvise		219
#define __NR_madvise1		219	/* delete when C lib stub is removed */
#define __NR_getdents64		220
#define __NR_fcntl64		221
/* 223 is unused */
#define __NR_gettid		224
#define __NR_readahead		225
#define __NR_setxattr		226
#define __NR_lsetxattr		227
#define __NR_fsetxattr		228
#define __NR_getxattr		229
#define __NR_lgetxattr		230
#define __NR_fgetxattr		231
#define __NR_listxattr		232
#define __NR_llistxattr		233
#define __NR_flistxattr		234
#define __NR_removexattr	235
#define __NR_lremovexattr	236
#define __NR_fremovexattr	237
#define __NR_tkill		238
#define __NR_sendfile64		239
#define __NR_futex		240
#define __NR_sched_setaffinity	241
#define __NR_sched_getaffinity	242
#define __NR_set_thread_area	243
#define __NR_get_thread_area	244
#define __NR_io_setup		245
#define __NR_io_destroy		246
#define __NR_io_getevents	247
#define __NR_io_submit		248
#define __NR_io_cancel		249
#define __NR_fadvise64		250
/* 251 is available for reuse (was briefly sys_set_zone_reclaim) */
#define __NR_exit_group		252
#define __NR_lookup_dcookie	253
#define __NR_epoll_create	254
#define __NR_epoll_ctl		255
#define __NR_epoll_wait		256
#define __NR_remap_file_pages	257
#define __NR_set_tid_address	258
#define __NR_timer_create	259
#define __NR_timer_settime	(__NR_timer_create+1)
#define __NR_timer_gettime	(__NR_timer_create+2)
#define __NR_timer_getoverrun	(__NR_timer_create+3)
#define __NR_timer_delete	(__NR_timer_create+4)
#define __NR_clock_settime	(__NR_timer_create+5)
#define __NR_clock_gettime	(__NR_timer_create+6)
#define __NR_clock_getres	(__NR_timer_create+7)
#define __NR_clock_nanosleep	(__NR_timer_create+8)
#define __NR_statfs64		268
#define __NR_fstatfs64		269
#define __NR_tgkill		270
#define __NR_utimes		271
#define __NR_fadvise64_64	272
#define __NR_vserver		273
#define __NR_mbind		274
#define __NR_get_mempolicy	275
#define __NR_set_mempolicy	276
#define __NR_mq_open 		277
#define __NR_mq_unlink		(__NR_mq_open+1)
#define __NR_mq_timedsend	(__NR_mq_open+2)
#define __NR_mq_timedreceive	(__NR_mq_open+3)
#define __NR_mq_notify		(__NR_mq_open+4)
#define __NR_mq_getsetattr	(__NR_mq_open+5)
#define __NR_kexec_load		283
#define __NR_waitid		284
/* #define __NR_sys_setaltroot	285 */
#define __NR_add_key		286
#define __NR_request_key	287
#define __NR_keyctl		288
#define __NR_ioprio_set		289
#define __NR_ioprio_get		290
#define __NR_inotify_init	291
#define __NR_inotify_add_watch	292
#define __NR_inotify_rm_watch	293
#define __NR_migrate_pages	294
#define __NR_openat		295
#define __NR_mkdirat		296
#define __NR_mknodat		297
#define __NR_fchownat		298
#define __NR_futimesat		299
#define __NR_fstatat64		300
#define __NR_unlinkat		301
#define __NR_renameat		302
#define __NR_linkat		303
#define __NR_symlinkat		304
#define __NR_readlinkat		305
#define __NR_fchmodat		306
#define __NR_faccessat		307
#define __NR_pselect6		308
#define __NR_ppoll		309
#define __NR_unshare		310
#define __NR_set_robust_list	311
#define __NR_get_robust_list	312
#define __NR_splice		313
#define __NR_sync_file_range	314
#define __NR_tee		315
#define __NR_vmsplice		316
#define __NR_move_pages		317
#define __NR_getcpu		318
/* #define __NR_epoll_pwait	319 */
/* #define __NR_utimensat	320 */
/* #define __NR_signalfd	321 */
/* #define __NR_timerfd_create	322 */
#define __NR_eventfd		323
#define __NR_fallocate		324
/* #define __NR_timerfd_settime	325 */
/* #define __NR_timerfd_gettime	326 */
/* #define __NR_signalfd4	327 */
/* #define __NR_eventfd2	328 */
/* #define __NR_epoll_create1	329 */
/* #define __NR_dup3		330 */
#define __NR_pipe2		331
/* #define __NR_inotify_init1	332 */
/* #define __NR_preadv		333 */
/* #define __NR_pwritev		334 */
/* #define __NR_rt_tgsigqueueinfo	335 */
/* #define __NR_perf_event_open	336 */
#define __NR_recvmmsg		337
#define __NR_prlimit64		338

#else
/** x86_64 系统，是64位的*/

#ifndef __SYSCALL
#define __SYSCALL(a,b) 
#endif

/* at least 8 syscall per cacheline */
#define __NR_read                                0
__SYSCALL(__NR_read, sys_read)
#define __NR_write                               1
__SYSCALL(__NR_write, sys_write)
#define __NR_open                                2
__SYSCALL(__NR_open, sys_open)
#define __NR_close                               3
__SYSCALL(__NR_close, sys_close)
#define __NR_stat                                4
__SYSCALL(__NR_stat, sys_newstat)
#define __NR_fstat                               5
__SYSCALL(__NR_fstat, sys_newfstat)
#define __NR_lstat                               6
__SYSCALL(__NR_lstat, sys_newlstat)
#define __NR_poll                                7
__SYSCALL(__NR_poll, sys_poll)

#define __NR_lseek                               8
__SYSCALL(__NR_lseek, sys_lseek)
#define __NR_mmap                                9
__SYSCALL(__NR_mmap, sys_mmap)
#define __NR_mprotect                           10
__SYSCALL(__NR_mprotect, sys_mprotect)
#define __NR_munmap                             11
__SYSCALL(__NR_munmap, sys_munmap)
#define __NR_brk                                12
__SYSCALL(__NR_brk, sys_brk)
#define __NR_rt_sigaction                       13
__SYSCALL(__NR_rt_sigaction, sys_rt_sigaction)
#define __NR_rt_sigprocmask                     14
__SYSCALL(__NR_rt_sigprocmask, sys_rt_sigprocmask)
#define __NR_rt_sigreturn                       15
__SYSCALL(__NR_rt_sigreturn, stub_rt_sigreturn)

#define __NR_ioctl                              16
__SYSCALL(__NR_ioctl, sys_ioctl)
#define __NR_pread64                            17
__SYSCALL(__NR_pread64, sys_pread64)
#define __NR_pwrite64                           18
__SYSCALL(__NR_pwrite64, sys_pwrite64)
#define __NR_readv                              19
__SYSCALL(__NR_readv, sys_readv)
#define __NR_writev                             20
__SYSCALL(__NR_writev, sys_writev)
#define __NR_access                             21
__SYSCALL(__NR_access, sys_access)
#define __NR_pipe                               22
__SYSCALL(__NR_pipe, sys_pipe)
#define __NR_select                             23
__SYSCALL(__NR_select, sys_select)

#define __NR_sched_yield                        24
__SYSCALL(__NR_sched_yield, sys_sched_yield)
#define __NR_mremap                             25
__SYSCALL(__NR_mremap, sys_mremap)
#define __NR_msync                              26
__SYSCALL(__NR_msync, sys_msync)
#define __NR_mincore                            27
__SYSCALL(__NR_mincore, sys_mincore)
#define __NR_madvise                            28
__SYSCALL(__NR_madvise, sys_madvise)
#define __NR_shmget                             29
__SYSCALL(__NR_shmget, sys_shmget)
#define __NR_shmat                              30
__SYSCALL(__NR_shmat, sys_shmat)
#define __NR_shmctl                             31
__SYSCALL(__NR_shmctl, sys_shmctl)

#define __NR_dup                                32
__SYSCALL(__NR_dup, sys_dup)
#define __NR_dup2                               33
__SYSCALL(__NR_dup2, sys_dup2)
#define __NR_pause                              34
__SYSCALL(__NR_pause, sys_pause)
#define __NR_nanosleep                          35
__SYSCALL(__NR_nanosleep, sys_nanosleep)
#define __NR_getitimer                          36
__SYSCALL(__NR_getitimer, sys_getitimer)
#define __NR_alarm                              37
__SYSCALL(__NR_alarm, sys_alarm)
#define __NR_setitimer                          38
__SYSCALL(__NR_setitimer, sys_setitimer)
#define __NR_getpid                             39
__SYSCALL(__NR_getpid, sys_getpid)

#define __NR_sendfile                           40
__SYSCALL(__NR_sendfile, sys_sendfile64)
#define __NR_socket                             41
__SYSCALL(__NR_socket, sys_socket)
#define __NR_connect                            42
__SYSCALL(__NR_connect, sys_connect)
#define __NR_accept                             43
__SYSCALL(__NR_accept, sys_accept)
#define __NR_sendto                             44
__SYSCALL(__NR_sendto, sys_sendto)
#define __NR_recvfrom                           45
__SYSCALL(__NR_recvfrom, sys_recvfrom)
#define __NR_sendmsg                            46
__SYSCALL(__NR_sendmsg, sys_sendmsg)
#define __NR_recvmsg                            47
__SYSCALL(__NR_recvmsg, sys_recvmsg)

#define __NR_shutdown                           48
__SYSCALL(__NR_shutdown, sys_shutdown)
#define __NR_bind                               49
__SYSCALL(__NR_bind, sys_bind)
#define __NR_listen                             50
__SYSCALL(__NR_listen, sys_listen)
#define __NR_getsockname                        51
__SYSCALL(__NR_getsockname, sys_getsockname)
#define __NR_getpeername                        52
__SYSCALL(__NR_getpeername, sys_getpeername)
#define __NR_socketpair                         53
__SYSCALL(__NR_socketpair, sys_socketpair)
#define __NR_setsockopt                         54
__SYSCALL(__NR_setsockopt, sys_setsockopt)
#define __NR_getsockopt                         55
__SYSCALL(__NR_getsockopt, sys_getsockopt)

#define __NR_clone                              56
__SYSCALL(__NR_clone, stub_clone)
#define __NR_fork                               57
__SYSCALL(__NR_fork, stub_fork) 
#define __NR_vfork                              58
__SYSCALL(__NR_vfork, stub_vfork)
#define __NR_execve                             59
__SYSCALL(__NR_execve, stub_execve)
#define __NR_exit                               60
__SYSCALL(__NR_exit, sys_exit)
#define __NR_wait4                              61
__SYSCALL(__NR_wait4, sys_wait4)
#define __NR_kill                               62
__SYSCALL(__NR_kill, sys_kill)
#define __NR_uname                              63
__SYSCALL(__NR_uname, sys_uname)

#define __NR_semget                             64
__SYSCALL(__NR_semget, sys_semget)
#define __NR_semop                              65
__SYSCALL(__NR_semop, sys_semop)
#define __NR_semctl                             66
__SYSCALL(__NR_semctl, sys_semctl)
#define __NR_shmdt                              67
__SYSCALL(__NR_shmdt, sys_shmdt)
#define __NR_msgget                             68
__SYSCALL(__NR_msgget, sys_msgget)
#define __NR_msgsnd                             69
__SYSCALL(__NR_msgsnd, sys_msgsnd)
#define __NR_msgrcv                             70
__SYSCALL(__NR_msgrcv, sys_msgrcv)
#define __NR_msgctl                             71
__SYSCALL(__NR_msgctl, sys_msgctl)

#define __NR_fcntl                              72
__SYSCALL(__NR_fcntl, sys_fcntl)
#define __NR_flock                              73
__SYSCALL(__NR_flock, sys_flock)
#define __NR_fsync                              74
__SYSCALL(__NR_fsync, sys_fsync)
#define __NR_fdatasync                          75
__SYSCALL(__NR_fdatasync, sys_fdatasync)
#define __NR_truncate                           76
__SYSCALL(__NR_truncate, sys_truncate)
#define __NR_ftruncate                          77
__SYSCALL(__NR_ftruncate, sys_ftruncate)
#define __NR_getdents                           78
__SYSCALL(__NR_getdents, sys_getdents)
#define __NR_getcwd                             79
__SYSCALL(__NR_getcwd, sys_getcwd)

#define __NR_chdir                              80
__SYSCALL(__NR_chdir, sys_chdir)
#define __NR_fchdir                             81
__SYSCALL(__NR_fchdir, sys_fchdir)
#define __NR_rename                             82
__SYSCALL(__NR_rename, sys_rename)
#define __NR_mkdir                              83
__SYSCALL(__NR_mkdir, sys_mkdir)
#define __NR_rmdir                              84
__SYSCALL(__NR_rmdir, sys_rmdir)
#define __NR_creat                              85
__SYSCALL(__NR_creat, sys_creat)
#define __NR_link                               86
__SYSCALL(__NR_link, sys_link)
#define __NR_unlink                             87
__SYSCALL(__NR_unlink, sys_unlink)

#define __NR_symlink                            88
__SYSCALL(__NR_symlink, sys_symlink)
#define __NR_readlink                           89
__SYSCALL(__NR_readlink, sys_readlink)
#define __NR_chmod                              90
__SYSCALL(__NR_chmod, sys_chmod)
#define __NR_fchmod                             91
__SYSCALL(__NR_fchmod, sys_fchmod)
#define __NR_chown                              92
__SYSCALL(__NR_chown, sys_chown)
#define __NR_fchown                             93
__SYSCALL(__NR_fchown, sys_fchown)
#define __NR_lchown                             94
__SYSCALL(__NR_lchown, sys_lchown)
#define __NR_umask                              95
__SYSCALL(__NR_umask, sys_umask)

#define __NR_gettimeofday                       96
__SYSCALL(__NR_gettimeofday, sys_gettimeofday)
#define __NR_getrlimit                          97
__SYSCALL(__NR_getrlimit, sys_getrlimit)
#define __NR_getrusage                          98
__SYSCALL(__NR_getrusage, sys_getrusage)
#define __NR_sysinfo                            99
__SYSCALL(__NR_sysinfo, sys_sysinfo)
#define __NR_times                             100
__SYSCALL(__NR_times, sys_times)
#define __NR_ptrace                            101
__SYSCALL(__NR_ptrace, sys_ptrace)
#define __NR_getuid                            102
__SYSCALL(__NR_getuid, sys_getuid)
#define __NR_syslog                            103
__SYSCALL(__NR_syslog, sys_syslog)

/* at the very end the stuff that never runs during the benchmarks */
#define __NR_getgid                            104
__SYSCALL(__NR_getgid, sys_getgid)
#define __NR_setuid                            105
__SYSCALL(__NR_setuid, sys_setuid)
#define __NR_setgid                            106
__SYSCALL(__NR_setgid, sys_setgid)
#define __NR_geteuid                           107
__SYSCALL(__NR_geteuid, sys_geteuid)
#define __NR_getegid                           108
__SYSCALL(__NR_getegid, sys_getegid)
#define __NR_setpgid                           109
__SYSCALL(__NR_setpgid, sys_setpgid)
#define __NR_getppid                           110
__SYSCALL(__NR_getppid, sys_getppid)
#define __NR_getpgrp                           111
__SYSCALL(__NR_getpgrp, sys_getpgrp)

#define __NR_setsid                            112
__SYSCALL(__NR_setsid, sys_setsid)
#define __NR_setreuid                          113
__SYSCALL(__NR_setreuid, sys_setreuid)
#define __NR_setregid                          114
__SYSCALL(__NR_setregid, sys_setregid)
#define __NR_getgroups                         115
__SYSCALL(__NR_getgroups, sys_getgroups)
#define __NR_setgroups                         116
__SYSCALL(__NR_setgroups, sys_setgroups)
#define __NR_setresuid                         117
__SYSCALL(__NR_setresuid, sys_setresuid)
#define __NR_getresuid                         118
__SYSCALL(__NR_getresuid, sys_getresuid)
#define __NR_setresgid                         119
__SYSCALL(__NR_setresgid, sys_setresgid)

#define __NR_getresgid                         120
__SYSCALL(__NR_getresgid, sys_getresgid)
#define __NR_getpgid                           121
__SYSCALL(__NR_getpgid, sys_getpgid)
#define __NR_setfsuid                          122
__SYSCALL(__NR_setfsuid, sys_setfsuid)
#define __NR_setfsgid                          123
__SYSCALL(__NR_setfsgid, sys_setfsgid)
#define __NR_getsid                            124
__SYSCALL(__NR_getsid, sys_getsid)
#define __NR_capget                            125
__SYSCALL(__NR_capget, sys_capget)
#define __NR_capset                            126
__SYSCALL(__NR_capset, sys_capset)

#define __NR_rt_sigpending                     127
__SYSCALL(__NR_rt_sigpending, sys_rt_sigpending)
#define __NR_rt_sigtimedwait                   128
__SYSCALL(__NR_rt_sigtimedwait, sys_rt_sigtimedwait)
#define __NR_rt_sigqueueinfo                   129
__SYSCALL(__NR_rt_sigqueueinfo, sys_rt_sigqueueinfo)
#define __NR_rt_sigsuspend                     130
__SYSCALL(__NR_rt_sigsuspend, stub_rt_sigsuspend)
#define __NR_sigaltstack                       131
__SYSCALL(__NR_sigaltstack, stub_sigaltstack)
#define __NR_utime                             132
__SYSCALL(__NR_utime, sys_utime)
#define __NR_mknod                             133
__SYSCALL(__NR_mknod, sys_mknod)

#define __NR_ustat                             136
__SYSCALL(__NR_ustat, sys_ustat)
#define __NR_statfs                            137
__SYSCALL(__NR_statfs, sys_statfs)
#define __NR_fstatfs                           138
__SYSCALL(__NR_fstatfs, sys_fstatfs)
#define __NR_sysfs                             139
__SYSCALL(__NR_sysfs, sys_sysfs)

#define __NR_getpriority                       140
__SYSCALL(__NR_getpriority, sys_getpriority)
#define __NR_setpriority                       141
__SYSCALL(__NR_setpriority, sys_setpriority)
#define __NR_sched_setparam                    142
__SYSCALL(__NR_sched_setparam, sys_sched_setparam)
#define __NR_sched_getparam                    143
__SYSCALL(__NR_sched_getparam, sys_sched_getparam)
#define __NR_sched_setscheduler                144
__SYSCALL(__NR_sched_setscheduler, sys_sched_setscheduler)
#define __NR_sched_getscheduler                145
__SYSCALL(__NR_sched_getscheduler, sys_sched_getscheduler)
#define __NR_sched_get_priority_max            146
__SYSCALL(__NR_sched_get_priority_max, sys_sched_get_priority_max)
#define __NR_sched_get_priority_min            147
__SYSCALL(__NR_sched_get_priority_min, sys_sched_get_priority_min)
#define __NR_sched_rr_get_interval             148
__SYSCALL(__NR_sched_rr_get_interval, sys_sched_rr_get_interval)

#define __NR_mlock                             149
__SYSCALL(__NR_mlock, sys_mlock)
#define __NR_munlock                           150
__SYSCALL(__NR_munlock, sys_munlock)
#define __NR_mlockall                          151
__SYSCALL(__NR_mlockall, sys_mlockall)
#define __NR_munlockall                        152
__SYSCALL(__NR_munlockall, sys_munlockall)

#define __NR_vhangup                           153
__SYSCALL(__NR_vhangup, sys_vhangup)

#define __NR_modify_ldt                        154
__SYSCALL(__NR_modify_ldt, sys_modify_ldt)

#define __NR_pivot_root                        155
__SYSCALL(__NR_pivot_root, sys_pivot_root)

#define __NR__sysctl                           156
__SYSCALL(__NR__sysctl, sys_sysctl)

#define __NR_prctl                             157
__SYSCALL(__NR_prctl, sys_prctl)
#define __NR_arch_prctl                        158
__SYSCALL(__NR_arch_prctl,	sys_arch_prctl) 

#define __NR_adjtimex                          159
__SYSCALL(__NR_adjtimex, sys_adjtimex)

#define __NR_setrlimit                         160
__SYSCALL(__NR_setrlimit, sys_setrlimit)

#define __NR_chroot                            161
__SYSCALL(__NR_chroot, sys_chroot)

#define __NR_sync                              162
__SYSCALL(__NR_sync, sys_sync)

#define __NR_acct                              163
__SYSCALL(__NR_acct, sys_acct)

#define __NR_settimeofday                      164
__SYSCALL(__NR_settimeofday, sys_settimeofday)

#define __NR_mount                             165
__SYSCALL(__NR_mount, sys_mount)
#define __NR_umount2                           166
__SYSCALL(__NR_umount2, sys_umount)

#define __NR_swapon                            167
__SYSCALL(__NR_swapon, sys_swapon)
#define __NR_swapoff                           168
__SYSCALL(__NR_swapoff, sys_swapoff)

#define __NR_reboot                            169
__SYSCALL(__NR_reboot, sys_reboot)

#define __NR_sethostname                       170
__SYSCALL(__NR_sethostname, sys_sethostname)
#define __NR_setdomainname                     171
__SYSCALL(__NR_setdomainname, sys_setdomainname)

#define __NR_iopl                              172
__SYSCALL(__NR_iopl, stub_iopl)
#define __NR_ioperm                            173
__SYSCALL(__NR_ioperm, sys_ioperm)

#define __NR_create_module                     174
__SYSCALL(__NR_create_module, sys_ni_syscall)
#define __NR_init_module                       175
__SYSCALL(__NR_init_module, sys_init_module)
#define __NR_delete_module                     176
__SYSCALL(__NR_delete_module, sys_delete_module)
#define __NR_get_kernel_syms                   177
__SYSCALL(__NR_get_kernel_syms, sys_ni_syscall)
#define __NR_query_module                      178
__SYSCALL(__NR_query_module, sys_ni_syscall)

#define __NR_quotactl                          179
__SYSCALL(__NR_quotactl, sys_quotactl)

#define __NR_nfsservctl                        180
__SYSCALL(__NR_nfsservctl, sys_nfsservctl)

#define __NR_getpmsg                           181	/* reserved for LiS/STREAMS */
__SYSCALL(__NR_getpmsg, sys_ni_syscall)
#define __NR_putpmsg                           182	/* reserved for LiS/STREAMS */
__SYSCALL(__NR_putpmsg, sys_ni_syscall)

#define __NR_afs_syscall                       183	/* reserved for AFS */ 
__SYSCALL(__NR_afs_syscall, sys_ni_syscall)

#define __NR_security			185
__SYSCALL(__NR_security, sys_ni_syscall)

#define __NR_gettid		186
__SYSCALL(__NR_gettid, sys_gettid)

#define __NR_readahead		187
__SYSCALL(__NR_readahead, sys_readahead)
#define __NR_setxattr		188
__SYSCALL(__NR_setxattr, sys_setxattr)
#define __NR_lsetxattr		189
__SYSCALL(__NR_lsetxattr, sys_lsetxattr)
#define __NR_fsetxattr		190
__SYSCALL(__NR_fsetxattr, sys_fsetxattr)
#define __NR_getxattr		191
__SYSCALL(__NR_getxattr, sys_getxattr)
#define __NR_lgetxattr		192
__SYSCALL(__NR_lgetxattr, sys_lgetxattr)
#define __NR_fgetxattr		193
__SYSCALL(__NR_fgetxattr, sys_fgetxattr) 
#define __NR_listxattr		194
__SYSCALL(__NR_listxattr, sys_listxattr) 
#define __NR_llistxattr		195
__SYSCALL(__NR_llistxattr, sys_llistxattr) 
#define __NR_flistxattr		196
__SYSCALL(__NR_flistxattr, sys_flistxattr) 
#define __NR_removexattr	197
__SYSCALL(__NR_removexattr, sys_removexattr) 
#define __NR_lremovexattr	198
__SYSCALL(__NR_lremovexattr, sys_lremovexattr) 
#define __NR_fremovexattr	199
__SYSCALL(__NR_fremovexattr, sys_fremovexattr) 
#define __NR_tkill	200
__SYSCALL(__NR_tkill, sys_tkill) 
#define __NR_time      201
__SYSCALL(__NR_time, sys_time)
#define __NR_futex     202
__SYSCALL(__NR_futex, sys_futex)
#define __NR_sched_setaffinity    203
__SYSCALL(__NR_sched_setaffinity, sys_sched_setaffinity)
#define __NR_sched_getaffinity     204
__SYSCALL(__NR_sched_getaffinity, sys_sched_getaffinity)
#define __NR_set_thread_area	205
__SYSCALL(__NR_set_thread_area, sys_ni_syscall)	/* use arch_prctl */
#define __NR_io_setup	206
__SYSCALL(__NR_io_setup, sys_io_setup)
#define __NR_io_destroy	207
__SYSCALL(__NR_io_destroy, sys_io_destroy)
#define __NR_io_getevents	208
__SYSCALL(__NR_io_getevents, sys_io_getevents)
#define __NR_io_submit	209
__SYSCALL(__NR_io_submit, sys_io_submit)
#define __NR_io_cancel	210
__SYSCALL(__NR_io_cancel, sys_io_cancel)
#define __NR_get_thread_area	211
__SYSCALL(__NR_get_thread_area, sys_ni_syscall)	/* use arch_prctl */
#define __NR_lookup_dcookie	212
__SYSCALL(__NR_lookup_dcookie, sys_lookup_dcookie)
#define __NR_epoll_create	213
__SYSCALL(__NR_epoll_create, sys_epoll_create)
#define __NR_epoll_ctl_old	214
__SYSCALL(__NR_epoll_ctl_old, sys_ni_syscall)
#define __NR_epoll_wait_old	215
__SYSCALL(__NR_epoll_wait_old, sys_ni_syscall)
#define __NR_remap_file_pages	216
__SYSCALL(__NR_remap_file_pages, sys_remap_file_pages)
#define __NR_getdents64	217
__SYSCALL(__NR_getdents64, sys_getdents64)
#define __NR_set_tid_address	218
__SYSCALL(__NR_set_tid_address, sys_set_tid_address)
#define __NR_restart_syscall	219
__SYSCALL(__NR_restart_syscall, sys_restart_syscall)
#define __NR_semtimedop		220
__SYSCALL(__NR_semtimedop, sys_semtimedop)
#define __NR_fadvise64		221
__SYSCALL(__NR_fadvise64, sys_fadvise64)
#define __NR_timer_create		222
__SYSCALL(__NR_timer_create, sys_timer_create)
#define __NR_timer_settime		223
__SYSCALL(__NR_timer_settime, sys_timer_settime)
#define __NR_timer_gettime		224
__SYSCALL(__NR_timer_gettime, sys_timer_gettime)
#define __NR_timer_getoverrun		225
__SYSCALL(__NR_timer_getoverrun, sys_timer_getoverrun)
#define __NR_timer_delete	226
__SYSCALL(__NR_timer_delete, sys_timer_delete)
#define __NR_clock_settime	227
__SYSCALL(__NR_clock_settime, sys_clock_settime)
#define __NR_clock_gettime	228
__SYSCALL(__NR_clock_gettime, sys_clock_gettime)
#define __NR_clock_getres	229
__SYSCALL(__NR_clock_getres, sys_clock_getres)
#define __NR_clock_nanosleep	230
__SYSCALL(__NR_clock_nanosleep, sys_clock_nanosleep)
#define __NR_exit_group		231
__SYSCALL(__NR_exit_group, sys_exit_group)
#define __NR_epoll_wait		232
__SYSCALL(__NR_epoll_wait, sys_epoll_wait)
#define __NR_epoll_ctl		233
__SYSCALL(__NR_epoll_ctl, sys_epoll_ctl)
#define __NR_tgkill		234
__SYSCALL(__NR_tgkill, sys_tgkill)
#define __NR_utimes		235
__SYSCALL(__NR_utimes, sys_utimes)
#define __NR_vserver		236
__SYSCALL(__NR_vserver, sys_ni_syscall)
#define __NR_mbind 		237
__SYSCALL(__NR_mbind, sys_mbind)
#define __NR_set_mempolicy 	238
__SYSCALL(__NR_set_mempolicy, sys_set_mempolicy)
#define __NR_get_mempolicy 	239
__SYSCALL(__NR_get_mempolicy, sys_get_mempolicy)
#define __NR_mq_open 		240
__SYSCALL(__NR_mq_open, sys_mq_open)
#define __NR_mq_unlink 		241
__SYSCALL(__NR_mq_unlink, sys_mq_unlink)
#define __NR_mq_timedsend 	242
__SYSCALL(__NR_mq_timedsend, sys_mq_timedsend)
#define __NR_mq_timedreceive	243
__SYSCALL(__NR_mq_timedreceive, sys_mq_timedreceive)
#define __NR_mq_notify 		244
__SYSCALL(__NR_mq_notify, sys_mq_notify)
#define __NR_mq_getsetattr 	245
__SYSCALL(__NR_mq_getsetattr, sys_mq_getsetattr)
#define __NR_kexec_load 	246
__SYSCALL(__NR_kexec_load, sys_kexec_load)
#define __NR_waitid		247
__SYSCALL(__NR_waitid, sys_waitid)
#define __NR_add_key		248
__SYSCALL(__NR_add_key, sys_add_key)
#define __NR_request_key	249
__SYSCALL(__NR_request_key, sys_request_key)
#define __NR_keyctl		250
__SYSCALL(__NR_keyctl, sys_keyctl)
#define __NR_ioprio_set		251
__SYSCALL(__NR_ioprio_set, sys_ioprio_set)
#define __NR_ioprio_get		252
__SYSCALL(__NR_ioprio_get, sys_ioprio_get)
#define __NR_inotify_init	253
__SYSCALL(__NR_inotify_init, sys_inotify_init)
#define __NR_inotify_add_watch	254
__SYSCALL(__NR_inotify_add_watch, sys_inotify_add_watch)
#define __NR_inotify_rm_watch	255
__SYSCALL(__NR_inotify_rm_watch, sys_inotify_rm_watch)
#define __NR_migrate_pages	256
__SYSCALL(__NR_migrate_pages, sys_migrate_pages)
#define __NR_openat		257
__SYSCALL(__NR_openat, sys_openat)
#define __NR_mkdirat		258
__SYSCALL(__NR_mkdirat, sys_mkdirat)
#define __NR_mknodat		259
__SYSCALL(__NR_mknodat, sys_mknodat)
#define __NR_fchownat		260
__SYSCALL(__NR_fchownat, sys_fchownat)
#define __NR_futimesat		261
__SYSCALL(__NR_futimesat, sys_futimesat)
#define __NR_newfstatat		262
__SYSCALL(__NR_newfstatat, sys_newfstatat)
#define __NR_unlinkat		263
__SYSCALL(__NR_unlinkat, sys_unlinkat)
#define __NR_renameat		264
__SYSCALL(__NR_renameat, sys_renameat)
#define __NR_linkat		265
__SYSCALL(__NR_linkat, sys_linkat)
#define __NR_symlinkat		266
__SYSCALL(__NR_symlinkat, sys_symlinkat)
#define __NR_readlinkat		267
__SYSCALL(__NR_readlinkat, sys_readlinkat)
#define __NR_fchmodat		268
__SYSCALL(__NR_fchmodat, sys_fchmodat)
#define __NR_faccessat		269
__SYSCALL(__NR_faccessat, sys_faccessat)
#define __NR_pselect6		270
__SYSCALL(__NR_pselect6, sys_pselect6)
#define __NR_ppoll		271
__SYSCALL(__NR_ppoll,	sys_ppoll)
#define __NR_unshare		272
__SYSCALL(__NR_unshare,	sys_unshare)
#define __NR_set_robust_list	273
__SYSCALL(__NR_set_robust_list, sys_set_robust_list)
#define __NR_get_robust_list	274
__SYSCALL(__NR_get_robust_list, sys_get_robust_list)
#define __NR_splice		275
__SYSCALL(__NR_splice, sys_splice)
#define __NR_tee		276
__SYSCALL(__NR_tee, sys_tee)
#define __NR_sync_file_range	277
__SYSCALL(__NR_sync_file_range, sys_sync_file_range)
#define __NR_vmsplice		278
__SYSCALL(__NR_vmsplice, sys_vmsplice)
#define __NR_move_pages		279
__SYSCALL(__NR_move_pages, sys_move_pages)
#define __NR_utimensat		280
__SYSCALL(__NR_utimensat, sys_ni_syscall)
#define __NR_epoll_pwait	281
__SYSCALL(__NR_epoll_pwait, sys_ni_syscall)
#define __NR_signalfd		282
__SYSCALL(__NR_signalfd, sys_ni_syscall)
#define __NR_timerfd_create	283
__SYSCALL(__NR_timerfd_create, sys_ni_syscall)
#define __NR_eventfd		284
__SYSCALL(__NR_eventfd, sys_eventfd)
#define __NR_fallocate		285
__SYSCALL(__NR_fallocate, sys_fallocate)
#define __NR_timerfd_settime	286
__SYSCALL(__NR_timerfd_settime, sys_ni_syscall)
#define __NR_timerfd_gettime	287
__SYSCALL(__NR_timerfd_gettime, sys_ni_syscall)
#define __NR_accept4		288
__SYSCALL(__NR_accept4, sys_ni_syscall)
#define __NR_signalfd4		289
__SYSCALL(__NR_signalfd4, sys_ni_syscall)
#define __NR_eventfd2		290
__SYSCALL(__NR_eventfd2, sys_ni_syscall)
#define __NR_epoll_create1	291
__SYSCALL(__NR_epoll_create1, sys_ni_syscall)
#define __NR_dup3		292
__SYSCALL(__NR_dup3, sys_ni_syscall)
#define __NR_pipe2		293
__SYSCALL(__NR_pipe2, sys_ni_syscall)
#define __NR_inotify_init1	294
__SYSCALL(__NR_inotify_init1, sys_ni_syscall)
#define __NR_preadv		295
__SYSCALL(__NR_preadv, sys_ni_syscall)
#define __NR_pwritev		296
__SYSCALL(__NR_pwritev, sys_ni_syscall)
#define __NR_rt_tgsigqueueinfo	297
__SYSCALL(__NR_rt_tgsigqueueinfo, sys_ni_syscall)
#define __NR_perf_event_open	298
__SYSCALL(__NR_perf_event_open, sys_ni_syscall)
#define __NR_recvmmsg		299
__SYSCALL(__NR_recvmmsg, sys_recvmmsg)
#define __NR_prlimit64		300
__SYSCALL(__NR_prlimit64, sys_prlimit64)
#endif




#define SYS__sysctl __NR__sysctl
#define SYS_access __NR_access
#define SYS_acct __NR_acct
#define SYS_add_key __NR_add_key
#define SYS_adjtimex __NR_adjtimex
#define SYS_afs_syscall __NR_afs_syscall
#define SYS_alarm __NR_alarm
#define SYS_brk __NR_brk
#define SYS_capget __NR_capget
#define SYS_capset __NR_capset
#define SYS_chdir __NR_chdir
#define SYS_chmod __NR_chmod
#define SYS_chown __NR_chown
#define SYS_chroot __NR_chroot
#define SYS_clock_getres __NR_clock_getres
#define SYS_clock_gettime __NR_clock_gettime
#define SYS_clock_nanosleep __NR_clock_nanosleep
#define SYS_clock_settime __NR_clock_settime
#define SYS_clone __NR_clone
#define SYS_close __NR_close
#define SYS_creat __NR_creat
#define SYS_create_module __NR_create_module
#define SYS_delete_module __NR_delete_module
#define SYS_dup __NR_dup
#define SYS_dup2 __NR_dup2
#define SYS_epoll_create __NR_epoll_create
#define SYS_epoll_ctl __NR_epoll_ctl
#define SYS_epoll_wait __NR_epoll_wait
#define SYS_eventfd __NR_eventfd
#define SYS_execve __NR_execve
#define SYS_exit __NR_exit
#define SYS_exit_group __NR_exit_group
#define SYS_faccessat __NR_faccessat
#define SYS_fadvise64 __NR_fadvise64
#define SYS_fallocate __NR_fallocate
#define SYS_fchdir __NR_fchdir
#define SYS_fchmod __NR_fchmod
#define SYS_fchmodat __NR_fchmodat
#define SYS_fchown __NR_fchown
#define SYS_fchownat __NR_fchownat
#define SYS_fcntl __NR_fcntl
#define SYS_fdatasync __NR_fdatasync
#define SYS_fgetxattr __NR_fgetxattr
#define SYS_flistxattr __NR_flistxattr
#define SYS_flock __NR_flock
#define SYS_fork __NR_fork
#define SYS_fremovexattr __NR_fremovexattr
#define SYS_fsetxattr __NR_fsetxattr
#define SYS_fstat __NR_fstat
#define SYS_fstatfs __NR_fstatfs
#define SYS_fsync __NR_fsync
#define SYS_ftruncate __NR_ftruncate
#define SYS_futex __NR_futex
#define SYS_futimesat __NR_futimesat
#define SYS_get_kernel_syms __NR_get_kernel_syms
#define SYS_get_mempolicy __NR_get_mempolicy
#define SYS_get_robust_list __NR_get_robust_list
#define SYS_get_thread_area __NR_get_thread_area
#define SYS_getcwd __NR_getcwd
#define SYS_getdents __NR_getdents
#define SYS_getdents64 __NR_getdents64
#define SYS_getegid __NR_getegid
#define SYS_geteuid __NR_geteuid
#define SYS_getgid __NR_getgid
#define SYS_getgroups __NR_getgroups
#define SYS_getitimer __NR_getitimer
#define SYS_getpgid __NR_getpgid
#define SYS_getpgrp __NR_getpgrp
#define SYS_getpid __NR_getpid
#define SYS_getpmsg __NR_getpmsg
#define SYS_getppid __NR_getppid
#define SYS_getpriority __NR_getpriority
#define SYS_getresgid __NR_getresgid
#define SYS_getresuid __NR_getresuid
#define SYS_getrlimit __NR_getrlimit
#define SYS_getrusage __NR_getrusage
#define SYS_getsid __NR_getsid
#define SYS_gettid __NR_gettid
#define SYS_gettimeofday __NR_gettimeofday
#define SYS_getuid __NR_getuid
#define SYS_getxattr __NR_getxattr
#define SYS_init_module __NR_init_module
#define SYS_inotify_add_watch __NR_inotify_add_watch
#define SYS_inotify_init __NR_inotify_init
#define SYS_inotify_rm_watch __NR_inotify_rm_watch
#define SYS_io_cancel __NR_io_cancel
#define SYS_io_destroy __NR_io_destroy
#define SYS_io_getevents __NR_io_getevents
#define SYS_io_setup __NR_io_setup
#define SYS_io_submit __NR_io_submit
#define SYS_ioctl __NR_ioctl
#define SYS_ioperm __NR_ioperm
#define SYS_iopl __NR_iopl
#define SYS_ioprio_get __NR_ioprio_get
#define SYS_ioprio_set __NR_ioprio_set
#define SYS_kexec_load __NR_kexec_load
#define SYS_keyctl __NR_keyctl
#define SYS_kill __NR_kill
#define SYS_lchown __NR_lchown
#define SYS_lgetxattr __NR_lgetxattr
#define SYS_link __NR_link
#define SYS_linkat __NR_linkat
#define SYS_listxattr __NR_listxattr
#define SYS_llistxattr __NR_llistxattr
#define SYS_lookup_dcookie __NR_lookup_dcookie
#define SYS_lremovexattr __NR_lremovexattr
#define SYS_lseek __NR_lseek
#define SYS_lsetxattr __NR_lsetxattr
#define SYS_lstat __NR_lstat
#define SYS_madvise __NR_madvise
#define SYS_mbind __NR_mbind
#define SYS_migrate_pages __NR_migrate_pages
#define SYS_mincore __NR_mincore
#define SYS_mkdir __NR_mkdir
#define SYS_mkdirat __NR_mkdirat
#define SYS_mknod __NR_mknod
#define SYS_mknodat __NR_mknodat
#define SYS_mlock __NR_mlock
#define SYS_mlockall __NR_mlockall
#define SYS_mmap __NR_mmap
#define SYS_modify_ldt __NR_modify_ldt
#define SYS_mount __NR_mount
#define SYS_move_pages __NR_move_pages
#define SYS_mprotect __NR_mprotect
#define SYS_mq_getsetattr __NR_mq_getsetattr
#define SYS_mq_notify __NR_mq_notify
#define SYS_mq_open __NR_mq_open
#define SYS_mq_timedreceive __NR_mq_timedreceive
#define SYS_mq_timedsend __NR_mq_timedsend
#define SYS_mq_unlink __NR_mq_unlink
#define SYS_mremap __NR_mremap
#define SYS_msync __NR_msync
#define SYS_munlock __NR_munlock
#define SYS_munlockall __NR_munlockall
#define SYS_munmap __NR_munmap
#define SYS_nanosleep __NR_nanosleep
#define SYS_nfsservctl __NR_nfsservctl
#define SYS_open __NR_open
#define SYS_openat __NR_openat
#define SYS_pause __NR_pause
#define SYS_personality __NR_personality
#define SYS_pipe __NR_pipe
#define SYS_pivot_root __NR_pivot_root
#define SYS_poll __NR_poll
#define SYS_ppoll __NR_ppoll
#define SYS_prctl __NR_prctl
#define SYS_pread64 __NR_pread64
#define SYS_prlimit64 __NR_prlimit64
#define SYS_pselect6 __NR_pselect6
#define SYS_ptrace __NR_ptrace
#define SYS_putpmsg __NR_putpmsg
#define SYS_pwrite64 __NR_pwrite64
#define SYS_query_module __NR_query_module
#define SYS_quotactl __NR_quotactl
#define SYS_read __NR_read
#define SYS_readahead __NR_readahead
#define SYS_readlink __NR_readlink
#define SYS_readlinkat __NR_readlinkat
#define SYS_readv __NR_readv
#define SYS_reboot __NR_reboot
#define SYS_recvmmsg __NR_recvmmsg
#define SYS_remap_file_pages __NR_remap_file_pages
#define SYS_removexattr __NR_removexattr
#define SYS_rename __NR_rename
#define SYS_renameat __NR_renameat
#define SYS_request_key __NR_request_key
#define SYS_restart_syscall __NR_restart_syscall
#define SYS_rmdir __NR_rmdir
#define SYS_rt_sigaction __NR_rt_sigaction
#define SYS_rt_sigpending __NR_rt_sigpending
#define SYS_rt_sigprocmask __NR_rt_sigprocmask
#define SYS_rt_sigqueueinfo __NR_rt_sigqueueinfo
#define SYS_rt_sigreturn __NR_rt_sigreturn
#define SYS_rt_sigsuspend __NR_rt_sigsuspend
#define SYS_rt_sigtimedwait __NR_rt_sigtimedwait
#define SYS_sched_get_priority_max __NR_sched_get_priority_max
#define SYS_sched_get_priority_min __NR_sched_get_priority_min
#define SYS_sched_getaffinity __NR_sched_getaffinity
#define SYS_sched_getparam __NR_sched_getparam
#define SYS_sched_getscheduler __NR_sched_getscheduler
#define SYS_sched_rr_get_interval __NR_sched_rr_get_interval
#define SYS_sched_setaffinity __NR_sched_setaffinity
#define SYS_sched_setparam __NR_sched_setparam
#define SYS_sched_setscheduler __NR_sched_setscheduler
#define SYS_sched_yield __NR_sched_yield
#define SYS_select __NR_select
#define SYS_sendfile __NR_sendfile
#define SYS_set_mempolicy __NR_set_mempolicy
#define SYS_set_robust_list __NR_set_robust_list
#define SYS_set_thread_area __NR_set_thread_area
#define SYS_set_tid_address __NR_set_tid_address
#define SYS_setdomainname __NR_setdomainname
#define SYS_setfsgid __NR_setfsgid
#define SYS_setfsuid __NR_setfsuid
#define SYS_setgid __NR_setgid
#define SYS_setgroups __NR_setgroups
#define SYS_sethostname __NR_sethostname
#define SYS_setitimer __NR_setitimer
#define SYS_setpgid __NR_setpgid
#define SYS_setpriority __NR_setpriority
#define SYS_setregid __NR_setregid
#define SYS_setresgid __NR_setresgid
#define SYS_setresuid __NR_setresuid
#define SYS_setreuid __NR_setreuid
#define SYS_setrlimit __NR_setrlimit
#define SYS_setsid __NR_setsid
#define SYS_settimeofday __NR_settimeofday
#define SYS_setuid __NR_setuid
#define SYS_setxattr __NR_setxattr
#define SYS_sigaltstack __NR_sigaltstack
#define SYS_splice __NR_splice
#define SYS_stat __NR_stat
#define SYS_statfs __NR_statfs
#define SYS_swapoff __NR_swapoff
#define SYS_swapon __NR_swapon
#define SYS_symlink __NR_symlink
#define SYS_symlinkat __NR_symlinkat
#define SYS_sync __NR_sync
#define SYS_sync_file_range __NR_sync_file_range
#define SYS_sysfs __NR_sysfs
#define SYS_sysinfo __NR_sysinfo
#define SYS_syslog __NR_syslog
#define SYS_tee __NR_tee
#define SYS_tgkill __NR_tgkill
#define SYS_time __NR_time
#define SYS_timer_create __NR_timer_create
#define SYS_timer_delete __NR_timer_delete
#define SYS_timer_getoverrun __NR_timer_getoverrun
#define SYS_timer_gettime __NR_timer_gettime
#define SYS_timer_settime __NR_timer_settime
#define SYS_times __NR_times
#define SYS_tkill __NR_tkill
#define SYS_truncate __NR_truncate
#define SYS_umask __NR_umask
#define SYS_umount2 __NR_umount2
#define SYS_uname __NR_uname
#define SYS_unlink __NR_unlink
#define SYS_unlinkat __NR_unlinkat
#define SYS_unshare __NR_unshare
#define SYS_uselib __NR_uselib
#define SYS_ustat __NR_ustat
#define SYS_utime __NR_utime
#define SYS_utimes __NR_utimes
#define SYS_vfork __NR_vfork
#define SYS_vhangup __NR_vhangup
#define SYS_vmsplice __NR_vmsplice
#define SYS_vserver __NR_vserver
#define SYS_wait4 __NR_wait4
#define SYS_waitid __NR_waitid
#define SYS_write __NR_write
#define SYS_writev __NR_writev
#if __WORDSIZE == 64
#define SYS_accept __NR_accept
#define SYS_accept4 __NR_accept4
#define SYS_arch_prctl __NR_arch_prctl
#define SYS_bind __NR_bind
#define SYS_connect __NR_connect
#define SYS_dup3 __NR_dup3
#define SYS_epoll_create1 __NR_epoll_create1
#define SYS_epoll_ctl_old __NR_epoll_ctl_old
#define SYS_epoll_pwait __NR_epoll_pwait
#define SYS_epoll_wait_old __NR_epoll_wait_old
#define SYS_eventfd2 __NR_eventfd2
#define SYS_getpeername __NR_getpeername
#define SYS_getsockname __NR_getsockname
#define SYS_getsockopt __NR_getsockopt
#define SYS_inotify_init1 __NR_inotify_init1
#define SYS_listen __NR_listen
#define SYS_msgctl __NR_msgctl
#define SYS_msgget __NR_msgget
#define SYS_msgrcv __NR_msgrcv
#define SYS_msgsnd __NR_msgsnd
#define SYS_newfstatat __NR_newfstatat
#define SYS_perf_event_open __NR_perf_event_open
#define SYS_pipe2 __NR_pipe2
#define SYS_preadv __NR_preadv
#define SYS_pwritev __NR_pwritev
#define SYS_recvfrom __NR_recvfrom
#define SYS_recvmsg __NR_recvmsg
#define SYS_rt_tgsigqueueinfo __NR_rt_tgsigqueueinfo
#define SYS_security __NR_security
#define SYS_semctl __NR_semctl
#define SYS_semget __NR_semget
#define SYS_semop __NR_semop
#define SYS_semtimedop __NR_semtimedop
#define SYS_sendmsg __NR_sendmsg
#define SYS_sendto __NR_sendto
#define SYS_setsockopt __NR_setsockopt
#define SYS_shmat __NR_shmat
#define SYS_shmctl __NR_shmctl
#define SYS_shmdt __NR_shmdt
#define SYS_shmget __NR_shmget
#define SYS_shutdown __NR_shutdown
#define SYS_signalfd __NR_signalfd
#define SYS_signalfd4 __NR_signalfd4
#define SYS_socket __NR_socket
#define SYS_socketpair __NR_socketpair
#define SYS_timerfd_create __NR_timerfd_create
#define SYS_timerfd_gettime __NR_timerfd_gettime
#define SYS_timerfd_settime __NR_timerfd_settime
#define SYS_tuxcall __NR_tuxcall
#define SYS_utimensat __NR_utimensat
#else
#define SYS__llseek __NR__llseek
#define SYS__newselect __NR__newselect
#define SYS_bdflush __NR_bdflush
#define SYS_break __NR_break
#define SYS_chown32 __NR_chown32
#define SYS_fadvise64_64 __NR_fadvise64_64
#define SYS_fchown32 __NR_fchown32
#define SYS_fcntl64 __NR_fcntl64
#define SYS_fstat64 __NR_fstat64
#define SYS_fstatat64 __NR_fstatat64
#define SYS_fstatfs64 __NR_fstatfs64
#define SYS_ftime __NR_ftime
#define SYS_ftruncate64 __NR_ftruncate64
#define SYS_getcpu __NR_getcpu
#define SYS_getegid32 __NR_getegid32
#define SYS_geteuid32 __NR_geteuid32
#define SYS_getgid32 __NR_getgid32
#define SYS_getgroups32 __NR_getgroups32
#define SYS_getresgid32 __NR_getresgid32
#define SYS_getresuid32 __NR_getresuid32
#define SYS_getuid32 __NR_getuid32
#define SYS_gtty __NR_gtty
#define SYS_idle __NR_idle
#define SYS_ipc __NR_ipc
#define SYS_lchown32 __NR_lchown32
#define SYS_lock __NR_lock
#define SYS_lstat64 __NR_lstat64
#define SYS_madvise1 __NR_madvise1
#define SYS_mmap2 __NR_mmap2
#define SYS_mpx __NR_mpx
#define SYS_nice __NR_nice
#define SYS_oldfstat __NR_oldfstat
#define SYS_oldlstat __NR_oldlstat
#define SYS_oldolduname __NR_oldolduname
#define SYS_oldstat __NR_oldstat
#define SYS_olduname __NR_olduname
#define SYS_prof __NR_prof
#define SYS_profil __NR_profil
#define SYS_readdir __NR_readdir
#define SYS_sendfile64 __NR_sendfile64
#define SYS_setfsgid32 __NR_setfsgid32
#define SYS_setfsuid32 __NR_setfsuid32
#define SYS_setgid32 __NR_setgid32
#define SYS_setgroups32 __NR_setgroups32
#define SYS_setregid32 __NR_setregid32
#define SYS_setresgid32 __NR_setresgid32
#define SYS_setresuid32 __NR_setresuid32
#define SYS_setreuid32 __NR_setreuid32
#define SYS_setuid32 __NR_setuid32
#define SYS_sgetmask __NR_sgetmask
#define SYS_sigaction __NR_sigaction
#define SYS_signal __NR_signal
#define SYS_sigpending __NR_sigpending
#define SYS_sigprocmask __NR_sigprocmask
#define SYS_sigreturn __NR_sigreturn
#define SYS_sigsuspend __NR_sigsuspend
#define SYS_socketcall __NR_socketcall
#define SYS_ssetmask __NR_ssetmask
#define SYS_stat64 __NR_stat64
#define SYS_statfs64 __NR_statfs64
#define SYS_stime __NR_stime
#define SYS_stty __NR_stty
#define SYS_truncate64 __NR_truncate64
#define SYS_ugetrlimit __NR_ugetrlimit
#define SYS_ulimit __NR_ulimit
#define SYS_umount __NR_umount
#define SYS_vm86 __NR_vm86
#define SYS_vm86old __NR_vm86old
#define SYS_waitpid __NR_waitpid
#endif

#endif /** __QKC_SYSCALL_H */

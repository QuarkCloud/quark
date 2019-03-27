
#ifndef __QKC_INTERNAL_PIPE_MGR_H
#define __QKC_INTERNAL_PIPE_MGR_H 1


#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>
#include <wintf/wobj.h>
#include "rlist.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

__BEGIN_DECLS

typedef struct __st_pipe {
	HANDLE handle;
	char * name ;
	int fd ;
	int flags;	//flags°üÀ¨ O_CLOEXEC O_DIRECT (since Linux 3.4)  O_NONBLOCK
	int reader;
	int writer;
	SRWLOCK locker;
	OVERLAPPED ovlp;
	bool connected;
} pipe_t ;

#define  PIPE_SIZE 4096


QKCAPI char * pipe_anonymous_name();
QKCAPI pipe_t * pipe_new_server(int flags);
QKCAPI bool pipe_init_server(pipe_t * p , int flags);
QKCAPI void pipe_final_server(pipe_t * p);
QKCAPI void pipe_free_server(pipe_t * p);

QKCAPI pipe_t * pipe_new_client(const char * name , int flags);
QKCAPI bool pipe_init_client(pipe_t * p, const char * name, int flags);
QKCAPI void pipe_final_client(pipe_t * p);
QKCAPI void pipe_free_client(pipe_t * p);

__END_DECLS

#endif /** __QKC_INTERNAL_PIPE_MGR_H */

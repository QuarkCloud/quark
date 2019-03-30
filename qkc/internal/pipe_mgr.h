
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
	int direct;	
	SRWLOCK locker;
	OVERLAPPED ovlp;
	bool connected;
	void * addition;
} pipe_t ;

#define PIPE_SIZE 4096
#define PIPE_READER	0x01 
#define PIPE_WRITER	0x02 

QKCAPI char * pipe_anonymous_name();
QKCAPI pipe_t * pipe_server_new(int flags);
QKCAPI bool pipe_server_init(pipe_t * p , int flags);
QKCAPI void pipe_server_final(pipe_t * p);
QKCAPI void pipe_server_free(pipe_t * p);
QKCAPI bool pipe_server_connected(pipe_t * p , int timeout);

QKCAPI pipe_t * pipe_client_new(const char * name , int flags);
QKCAPI bool pipe_client_init(pipe_t * p, const char * name, int flags);
QKCAPI void pipe_client_final(pipe_t * p);
QKCAPI void pipe_client_free(pipe_t * p);

QKCAPI int pipe_init(int pfd[2], int flags);

__END_DECLS

#endif /** __QKC_INTERNAL_PIPE_MGR_H */

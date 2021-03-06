
#ifndef __QKC_INTERNAL_FPIPE_H
#define __QKC_INTERNAL_FPIPE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include "ring_buffer.h"
#include "iocp_item.h"
#include "pipe_mgr.h"

__BEGIN_DECLS

typedef struct __st_pipe_item				pipe_item_t;
typedef struct __st_pipe_write_result		pipe_write_result_t ;
typedef struct __st_pipe_read_result		pipe_read_result_t ;

int iocp_pipe_callback(iocp_item_t * item , int evt , int result) ;
void iocp_pipe_free(iocp_item_t * item) ;


#define PIPEBUFSIZE      8192
struct __st_pipe_write_result{
    iocp_ovlp_t     link ;
    ring_buffer_t   ring_buffer ;
};

struct __st_pipe_read_result {
    iocp_ovlp_t   link ;
};

struct __st_pipe_item {
	pipe_t * pipe;
	pipe_write_result_t * writer;
	pipe_read_result_t	* reader;

	iocp_item_t * iocp_item; 
};


pipe_item_t * pipe_item_new();
void pipe_item_free(pipe_item_t * item);

pipe_write_result_t * pipe_write_result_new() ;
void pipe_write_result_free(pipe_write_result_t * result) ;
bool pipe_write_result_init(pipe_write_result_t * result) ;
bool pipe_write_result_final(pipe_write_result_t * result) ;

bool pipe_write_iocp(pipe_write_result_t * result) ;
ssize_t pipe_write(pipe_item_t * pipe_item, const void * buf, size_t nbytes);

pipe_read_result_t * pipe_read_result_new() ;
void pipe_read_result_free(pipe_read_result_t * result) ;
bool pipe_read_result_init(pipe_read_result_t * result) ;
bool pipe_read_result_final(pipe_read_result_t * result) ;

bool pipe_start_read(pipe_read_result_t * result) ;
ssize_t pipe_read(pipe_item_t * pipe_item, void * buf, size_t nbytes);

__END_DECLS

#endif /** __QKC_INTERNAL_FPIPE_H */

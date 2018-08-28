
#ifndef __QKC_INTERNAL_FPIPE_H
#define __QKC_INTERNAL_FPIPE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include "ring_buffer.h"
#include "iocp_item.h"

__BEGIN_DECLS

typedef struct __st_pipe_ovlp       pipe_ovlp_t ;
typedef struct __st_write_result    write_result_t ;
typedef struct __st_read_result     read_result_t ;
typedef struct __st_pipe            pipe_t;

int iocp_pipe_callback(iocp_item_t * item , int evt , int result) ;
void iocp_pipe_free(iocp_item_t * item) ;

typedef struct __st_pipe{
    HANDLE handle ;
    int crtfd ;
    int8_t noblock :1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

    HANDLE            locker ;    //创建销毁保护
    write_result_t *  writer ;
    read_result_t *   reader ;

    void * addition ;               //附加的信息
} pipe_t;


#define PIPEBUFSIZE      8192
struct __st_write_result{
    iocp_ovlp_t     link ;
    ring_buffer_t   ring_buffer ;
};

struct __st_read_result{
    iocp_ovlp_t   link ;
};

pipe_t * pipe_new() ;
bool pipe_init(pipe_t * pipe) ;
void pipe_free(pipe_t * pipe) ;
bool pipe_final(pipe_t * pipe) ;

int add_pipe_obj(int fd) ;

write_result_t * write_result_new() ;
void write_result_free(write_result_t * result) ;
bool write_result_init(write_result_t * result) ;
bool write_result_final(write_result_t * result) ;

bool pipe_write(write_result_t * result , int flags) ;

read_result_t * read_result_new() ;
void read_result_free(read_result_t * result) ;
bool read_result_init(read_result_t * result) ;
bool read_result_final(read_result_t * result) ;

bool pipe_start_read(read_result_t * result) ;


__END_DECLS

#endif /** __QKC_INTERNAL_FPIPE_H */

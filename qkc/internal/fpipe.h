
#ifndef __QKC_INTERNAL_FPIPE_H
#define __QKC_INTERNAL_FPIPE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include "ring_buffer.h"

__BEGIN_DECLS

typedef struct __st_pipe_ovlp       pipe_ovlp_t ;
typedef struct __st_write_result    write_result_t ;
typedef struct __st_read_result     read_result_t ;
typedef struct __st_pipe            pipe_t;

typedef int (*pipe_callback_t)(pipe_t * pipe , int evt , int result) ;
#define kPipeBeforeClose      1 
#define kPipeWrite            2
#define kPipeRead             3


void pipe_callback(pipe_t *pipe , int evt , int result) ;

typedef struct __st_pipe{
    HANDLE rhandle ;
    HANDLE whandle ;
    int8_t noblock :1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

    HANDLE            locker ;    //创建销毁保护
    write_result_t   *writer ;
    read_result_t   * reader ;

    void * addition ;               //附加的信息
    pipe_callback_t callback ;
} pipe_t;

typedef enum{
    OVLP_PIPE_VOID   =   0 ,
    OVLP_PIPE_INPUT  =   2 ,
    OVLP_PIPE_OUTPUT =   3 
} ovlp_pipe_type_t ;

struct __st_pipe_ovlp
{
    OVERLAPPED          ovlp ;
    int                 status ;
    ovlp_pipe_type_t    type ;
    pipe_t    *         owner ;
    volatile LONG       counter ;
} ;

#define PIPEBUFSIZE      8192
struct __st_write_result{
    pipe_ovlp_t   link ;
    ring_buffer_t   ring_buffer ;
};

struct __st_read_result{
    pipe_ovlp_t   link ;
};


pipe_t * pipe_new() ;
bool pipe_init(pipe_t * pipe) ;
void pipe_free(pipe_t * pipe) ;
bool pipe_final(pipe_t * pipe) ;

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

bool pipe_ovlp_lock(pipe_ovlp_t * ovlp) ;
bool pipe_ovlp_unlock(pipe_ovlp_t * ovlp) ;
int pipe_ovlp_counter(pipe_ovlp_t * ovlp) ;


__END_DECLS

#endif /** __QKC_INTERNAL_FPIPE_H */

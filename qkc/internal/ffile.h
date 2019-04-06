
#ifndef __QKC_INTERNAL_FFILE_H
#define __QKC_INTERNAL_FFILE_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include "ring_buffer.h"
#include "iocp_item.h"

__BEGIN_DECLS

typedef struct __st_file			file_t;
typedef iocp_ovlp_t					file_read_result_t;
typedef iocp_ovlp_t					file_write_result_t;

int iocp_file_callback(iocp_item_t * item, int evt, int result);
void iocp_file_free(iocp_item_t * item);

typedef struct __st_file {
	HANDLE	handle;
	int8_t noblock : 1;            //默认阻塞，是一个很重要的标识!!!，其他貌似没有必要。

	HANDLE            locker;    //创建销毁保护
	file_read_result_t   * reader;
	file_write_result_t   * writer;

	void * addition;               //附加的信息
} file_t;


file_t * file_new();
bool file_init(file_t * f);
void file_free(file_t * f);
bool file_final(file_t * f);

file_write_result_t * file_write_result_new();
void file_write_result_free(file_write_result_t * result);
bool file_write_result_init(file_write_result_t * result);
bool file_write_result_final(file_write_result_t * result);

bool file_write(file_write_result_t * result, int flags);

file_read_result_t * file_read_result_new();
void file_read_result_free(file_read_result_t * result);
bool file_read_result_init(file_read_result_t * result);
bool file_read_result_final(file_read_result_t * result);

bool file_start_read(file_read_result_t * result);


__END_DECLS

#endif /** __QKC_INTERNAL_FFILE_H */


#include "ffile.h"
#include "iocp_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>


file_t * file_new()
{
	return NULL;
}

bool file_init(file_t * f)
{
	return false;
}

void file_free(file_t * f)
{
	return ;
}

bool file_final(file_t * f)
{
	return false;
}


file_write_result_t * file_write_result_new()
{
	return NULL;
}
void file_write_result_free(file_write_result_t * result)
{
	
}
bool file_write_result_init(file_write_result_t * result)
{
	return false;
}
bool file_write_result_final(file_write_result_t * result)
{
	return false;
}

bool file_write(file_write_result_t * result, int flags)
{
	return false;
}

file_read_result_t * file_read_result_new()
{
	return NULL;
}
void file_read_result_free(file_read_result_t * result)
{

}
bool file_read_result_init(file_read_result_t * result)
{
	return false;
}
bool file_read_result_final(file_read_result_t * result)
{
	return false;
}

bool file_start_read(file_read_result_t * result)
{
	return false;
}

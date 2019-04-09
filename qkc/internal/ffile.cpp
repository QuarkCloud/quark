
#include "ffile.h"
#include "iocp_mgr.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>


file_t * file_new()
{
	size_t file_size = sizeof(file_t);
	file_t * f = (file_t *)::malloc(file_size);
	if(f == NULL)
		return NULL;
	if (file_init(f) == true)
		return f;
	::free(f);
	return NULL;
}

bool file_init(file_t * f)
{
	if(f == NULL)
		return false;
	::memset(f, 0, sizeof(file_t));

	f->locker = ::CreateMutex(NULL, FALSE, NULL);
	if (f->locker == INVALID_HANDLE_VALUE)
		return false;
	return true;
}

void file_free(file_t * f)
{
	file_final(f);
	if (f != NULL)
		::free(f);
}

bool file_final(file_t * f)
{
	if (f->locker != NULL)
	{
		::CloseHandle(f->locker);
		f->locker = NULL;
	}
	return true;
}


file_write_result_t * file_write_result_new()
{
	size_t result_size = sizeof(file_write_result_t);
	file_write_result_t * result = (file_write_result_t *)::malloc(result_size);
	if (result == NULL)
		return NULL;
	if(file_write_result_init (result)== true)
		return result;
	::free(result);
	return NULL;
}
void file_write_result_free(file_write_result_t * result)
{
	if (result != NULL)
	{
		file_write_result_final(result);
		::free(result);
	}	
}
bool file_write_result_init(file_write_result_t * result)
{
	if(result == NULL)
		return false;
	::memset(result, 0, sizeof(file_write_result_t));
	result->ovlp.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	return (result->ovlp.hEvent != INVALID_HANDLE_VALUE);
}

bool file_write_result_final(file_write_result_t * result)
{
	if (result == NULL)
		return false;
	if (result->ovlp.hEvent != NULL && result->ovlp.hEvent != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(result->ovlp.hEvent);
		result->ovlp.hEvent = NULL;
	}
	return true;
}

bool file_write(file_write_result_t * result, int flags)
{
	return false;
}

file_read_result_t * file_read_result_new()
{
	size_t file_size = sizeof(file_read_result_t);
	file_read_result_t * result = (file_read_result_t *)::malloc(file_size);
	if(result == NULL)
		return NULL;
	if (file_read_result_init(result) == true)
		return result;

	::free(result);
	return NULL;
}
void file_read_result_free(file_read_result_t * result)
{
	if (result == NULL)
		return;
	file_read_result_final(result);
	::free(result);
}

bool file_read_result_init(file_read_result_t * result)
{
	if(result == NULL)
		return false;

	result->ovlp.hEvent = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	return (result->ovlp.hEvent != INVALID_HANDLE_VALUE);
}

bool file_read_result_final(file_read_result_t * result)
{
	if(result == NULL)
		return false;
	if (result->ovlp.hEvent != NULL && result->ovlp.hEvent != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(result->ovlp.hEvent);
		result->ovlp.hEvent = INVALID_HANDLE_VALUE;
	}
	return true;
}

bool file_start_read(file_read_result_t * result)
{
	return false;
}

int iocp_file_callback(iocp_item_t * item, int evt, int result)
{
	if (item == NULL || item->addition == NULL)
		return -1;

	file_t * f = (file_t *)item->addition;
	uint32_t events = item->data.events;
	int old_occur = item->occur, new_occur = item->occur;

	if (evt == IOCP_EVENT_OPEN)
		iocp_process_event(events & EPOLLERR, result, new_occur);
	else if (evt == IOCP_EVENT_WRITE)
		iocp_process_event(events & EPOLLOUT, result, new_occur);
	else if (evt == IOCP_EVENT_READ)
		iocp_process_event(events & EPOLLIN, result, new_occur);
	else if (evt == IOCP_EVENT_CLOSE)
		new_occur = 0;
	item->occur = new_occur;

	if ((events & EPOLLET) != 0)
	{
		//边缘触发，只在状态翻转时，执行操作
		if (old_occur == 0 && new_occur != 0)
			iocp_mgr_item_ready(item->owner, item);
		else if (old_occur != 0 && new_occur == 0)
			iocp_mgr_item_unready(item->owner, item);
	}
	else
	{
		if (new_occur != 0)
			iocp_mgr_item_ready(item->owner, item);
		else
			iocp_mgr_item_unready(item->owner, item);
	}

	return 0;
}

void iocp_file_free(iocp_item_t * item)
{
	if (item != NULL && item->addition != NULL)
	{
		file_t * f = (file_t *)item->addition;
		item->addition = NULL;
		file_free(f);
	}
}


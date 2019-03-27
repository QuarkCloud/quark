
#include "pipe_mgr.h"

static uint32_t __pipe_anonymous_sequence__ = 63875;
static SRWLOCK __pipe_anonymous_name_gen_locker__ = SRWLOCK_INIT;

char * pipe_anonymous_name()
{
	char tmpstr[256];
	int slen = 0;
	::AcquireSRWLockExclusive(&__pipe_anonymous_name_gen_locker__);
	__pipe_anonymous_sequence__++;

	slen = ::sprintf(tmpstr , "\\.\\pipe\\anonymous.%u.%u" , (uint32_t)::_getpid() , __pipe_anonymous_sequence__);
	::ReleaseSRWLockExclusive(&__pipe_anonymous_name_gen_locker__);
	if (slen <= 0)
		return NULL;

	char * name = (char *)::malloc(slen + 1);
	if (name == NULL)
		return NULL;

	::memcpy(name, tmpstr, slen + 1);
	return name;
}

pipe_t * pipe_new_server(int flags)
{
	size_t pipe_size = sizeof(pipe_t);
	pipe_t * p = (pipe_t *)::malloc(pipe_size);
	if (p == NULL)
		return NULL;

	if (pipe_init_server(p, flags) == true)
		return p;

	::free(p);
	return NULL;
}

bool pipe_init_server(pipe_t * p, int flags)
{
	if (p == NULL)
		return false;

	char * name = pipe_anonymous_name();
	if (name == NULL)
		return false;


	DWORD open_mode = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
	DWORD pipe_mode = PIPE_TYPE_BYTE | PIPE_NOWAIT | PIPE_READMODE_BYTE | PIPE_NOWAIT;

	HANDLE handle = ::CreateNamedPipeA(name , open_mode , pipe_mode , 1 , 
		PIPE_SIZE , PIPE_SIZE, 0 , NULL);
	if (handle == INVALID_HANDLE_VALUE)
	{
		::free(name);
		return false;
	}

	p->reader = p->writer = p->fd = -1;
	p->handle = handle;
	p->locker = SRWLOCK_INIT;
	p->flags = flags;
	p->name = name;
	p->fd = ::wobj_set(WOBJ_PIPE, handle, p);

	::memset(&p->ovlp, 0, sizeof(p->ovlp));
	p->ovlp.hEvent = ::CreateEventA(NULL, TRUE, FALSE, NULL);
	p->connected = false;

	BOOL result = ::ConnectNamedPipe(handle, &p->ovlp);
	if (result == FALSE)
	{
		if (::GetLastError() == ERROR_PIPE_CONNECTED)
			result = TRUE;
	}
	if (result == TRUE)
		p->connected = true;
	return true;
}

void pipe_free_server(pipe_t * p)
{
	pipe_final_server(p);
	if (p != NULL)
		::free(p);
}

void pipe_final_server(pipe_t * p)
{
	if (p == NULL)
		return;

	int fd = p->fd;
	wobj_t * obj = wobj_get(fd);
	if (obj->addition != p)
		return;

	wobj_del(fd);

	::AcquireSRWLockExclusive(&p->locker);
	if (p->name != NULL)
	{
		::free(p->name);
		p->name = NULL;
	}
	if (p->handle != NULL || p->handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(p->handle);
		p->handle = INVALID_HANDLE_VALUE;
	}
	::ReleaseSRWLockExclusive(&p->locker);
}

pipe_t * pipe_new_client(const char * name, int flags)
{
	if (name == NULL)
		return NULL;
	
	size_t pipe_size = sizeof(pipe_t);
	pipe_t * p = (pipe_t *)::malloc(pipe_size);
	if (p == NULL)
		return NULL;
	if (pipe_init_client(p, name, flags) == true)
		return p;
	::free(p);
	return NULL;
}

bool pipe_init_client(pipe_t * p, const char * name, int flags)
{
	if (p == NULL || name == NULL)
		return false;

	if (::WaitNamedPipeA(name, 1000) == FALSE)
		return false;

	HANDLE handle = ::CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle == INVALID_HANDLE_VALUE || handle == NULL)
		return false;

	p->fd = p->reader = p->writer = -1;
	p->handle = handle;
	p->locker = SRWLOCK_INIT;
	p->flags = flags;

	int name_size = ::strlen(name) + 1;
	char * str = (char *)::malloc(name_size);
	::memcpy(str, name, name_size);
	p->name = str;
	p->fd = ::wobj_set(WOBJ_PIPE, handle, p);

	return true;
}

void pipe_final_client(pipe_t * p)
{
	if (p == NULL)
		return;

	wobj_t * wobj = ::wobj_get(p->fd);
	if (wobj == NULL || wobj->addition != p)
		return;
	wobj_del(p->fd);

	::AcquireSRWLockExclusive(&p->locker);

	if (p->name)
	{
		::free(p->name);
		p->name = NULL;
	}

	if (p->handle)
	{
		::CloseHandle(p->handle);
		p->handle = INVALID_HANDLE_VALUE;
	}
	::ReleaseSRWLockExclusive(&p->locker);
}

void pipe_free_client(pipe_t * p)
{
	pipe_final_client(p);
	if (p != NULL)
		::free(p);
}

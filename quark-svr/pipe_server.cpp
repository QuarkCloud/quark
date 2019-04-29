
#include "pipe_server.h"
#include "pipe_util.h"


PipeServer::PipeServer()
{
	name_ = NULL;
}

PipeServer::~PipeServer()
{
	Final();
}

bool PipeServer::Init(const char * name)
{
	if (name == NULL)
		return false;

	if (loop_.Init() == false)
		return false;

	size_t nsize = ::strlen(name) + 1;
	char * str = (char *)::malloc(nsize);
	if (str == NULL)
		return false;

	::memcpy(str, name, nsize);
	name_ = str;
	return true;	
}

void PipeServer::Final()
{
	connections_.Final();
	loop_.Final();

	if (name_ != NULL)
	{
		::free(name_);
		name_ = NULL;
	}
}

void PipeServer::Run()
{
	StartListen();

	while (terminated_ == false)
	{
		PipeBase * obj = NULL;
		PipeOvlp * ovlp = NULL;
		DWORD bytes = 0;

		if (loop_.Wait(bytes, obj, ovlp, 1) == true)
		{
		}
	}
}

void PipeServer::Terminate()
{
	terminated_ = true;
}

bool PipeServer::StartListen()
{
	HANDLE pipe = ::CreateNamedPipe(name_, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_NOWAIT | PIPE_REJECT_REMOTE_CLIENTS, PIPE_UNLIMITED_INSTANCES,
		4096, 4096, 0, NULL);
	if (pipe == INVALID_HANDLE_VALUE)
	{
		DWORD errcode = ::GetLastError();
		return false;
	}

	PipeConnection * conn = new PipeConnection(pipe);
	PipeAccess<PipeConnection> scoped_access(conn);
	if (loop_.Bind(conn->Handle(), conn) == false)
		return false;

	if (connections_.InsertConnection(conn) == false)
		return false;

	if (conn->Start() == false)
	{
		connections_.DeleteConnection(conn);
		return false;
	}

	return true;
}


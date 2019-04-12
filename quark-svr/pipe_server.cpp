
#include "pipe_server.h"
#include "pipe_util.h"


PipeServer::PipeServer()
{
	handle_ = INVALID_HANDLE_VALUE;
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

	HANDLE iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (iocp == INVALID_HANDLE_VALUE)
		return false;

	handle_ = iocp;

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
	connections_.Clear();
	if (handle_ != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(handle_);
		handle_ = INVALID_HANDLE_VALUE;
	}
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
		DWORD bytes = 0;
		OVERLAPPED * ovlp = NULL;
		ULONG_PTR key = 0;
		::GetQueuedCompletionStatus(handle_, &bytes, &key, &ovlp, 1000);

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
	if (::CreateIoCompletionPort(pipe, handle_, (ULONG_PTR)PipeBase::kPipeConnection, 0) != handle_)
	{
		DWORD errcode = ::GetLastError();
		::CloseHandle(pipe);
		delete conn;
		return false;
	}

	if (connections_.InsertConnection(conn) == false)
		return false;

	if (::ConnectNamedPipe(conn->Handle(), conn->Ovlp()) == FALSE)
	{
		DWORD errcode = ::GetLastError();
		if (errcode != ERROR_PIPE_CONNECTED && errcode != ERROR_IO_PENDING)
		{
			connections_.DeleteConnection(conn);
			return false;
		}
	}

	return true;
}


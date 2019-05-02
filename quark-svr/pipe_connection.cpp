
#include <windows.h>
#include "pipe_connection.h"
#include "pipe_util.h"
#include "pipe_base.h"

PipeConnection::PipeConnection()
{
	handle_ = INVALID_HANDLE_VALUE;
	PipeBase::Type(PipeBase::kPipeConnection);
	ovlp_.Owner(this);
}

PipeConnection::PipeConnection(HANDLE handle)
{
	handle_ = handle;
	PipeBase::Type(PipeBase::kPipeConnection);
	ovlp_.Owner(this);
}

PipeConnection::~PipeConnection()
{
	::CloseHandle(handle_);
}

bool PipeConnection::Start()
{
	if (::ConnectNamedPipe(Handle(), &ovlp_) == FALSE)
	{
		DWORD errcode = ::GetLastError();
		if (errcode != ERROR_PIPE_CONNECTED && errcode != ERROR_IO_PENDING)
		{
			return false;
		}
	}

	return true;
}

bool PipeConnection::StartRead()
{
	return reader_.StartRead();
}


PipeConnectionMgr::PipeConnectionMgr()
{
	guard_ = SRWLOCK_INIT;
	::memset(connections_, 0, sizeof(connections_));
}

PipeConnectionMgr::~PipeConnectionMgr()
{
	Final();
}

bool PipeConnectionMgr::InsertConnection(PipeConnection * conn)
{
	bool result = false;
	::AcquireSRWLockExclusive(&guard_);

	for (size_t idx = 0; idx < kMaxPipeSize; ++idx)
	{
		if (connections_[idx].Valid() == true)
			continue;
		connections_[idx] = conn;
		result = true;
	}
	
	::ReleaseSRWLockExclusive(&guard_);
	return result;
}

bool PipeConnectionMgr::DeleteConnection(HANDLE handle)
{
	bool result = false;
	::AcquireSRWLockExclusive(&guard_);

	for (size_t idx = 0; idx < kMaxPipeSize; ++idx)
	{
		PipeConnection * conn = connections_[idx].Object();
		if (conn == NULL || conn->Handle() != handle)
			continue;

		connections_[idx] = NULL;
		delete conn;
		result = true;
		break;
	}

	::ReleaseSRWLockExclusive(&guard_);
	return result;
}

bool PipeConnectionMgr::DeleteConnection(PipeConnection * conn)
{
	bool result = false;
	::AcquireSRWLockExclusive(&guard_);

	for (size_t idx = 0; idx < kMaxPipeSize; ++idx)
	{		
		if (conn != connections_[idx].Object())
			continue;

		connections_[idx] = NULL;
		delete conn;
		result = true;
		break;
	}

	::ReleaseSRWLockExclusive(&guard_);
	return result;
}


void PipeConnectionMgr::Clear()
{

}

void PipeConnectionMgr::Final()
{

}









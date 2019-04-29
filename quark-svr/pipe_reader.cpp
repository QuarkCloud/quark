
#include "pipe_reader.h"


PipeReader::PipeReader()
{
	completed_bytes_ = 0;
	ovlp_.Owner(this);
	Type(kPipeReader);
}

PipeReader::~PipeReader()
{
	//
}

int PipeReader::Read(void  * buffer, int size)
{
	DWORD bytes = 0;
	if (::ReadFile(this->Handle(), buffer, (DWORD)size, &bytes, this->ovlp_.Value()) == FALSE)
	{
		DWORD errcode = ::GetLastError();
		return -1;
	}
	return bytes;
}

bool PipeReader::StartRead()
{
	DWORD bytes = 0;
	if (::ReadFile(this->Handle(), NULL, 0, &bytes, this->ovlp_.Value()) == FALSE)
	{
		DWORD errcode = ::GetLastError();
		return false;
	}
	return true;
}

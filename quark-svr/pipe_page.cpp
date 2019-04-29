
#include <windows.h>
#include "pipe_page.h"

PipePage::PipePage()
{
	offset_ = size_ = buflen_ = 0;
	buffer_ = NULL;
	status_ = kIdle;

	PVOID  addr = ::VirtualAlloc(NULL, PipePage::kSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (addr != NULL)
	{
		buffer_ = (char *)addr;
		buflen_ = PipePage::kSize;
	}
}

PipePage::~PipePage()
{
	offset_ = size_ = buflen_ = 0;
	if (buffer_ != NULL)
	{
		::VirtualFree(buffer_, 0, MEM_RELEASE);
		buffer_ = NULL;
	}
}

bool PipePage::Consume(size_t len)
{
	offset_ += len;
	return true;
}

bool PipePage::Append(size_t len)
{
	if (offset_ + size_ + len > buflen_)
		return false;
	size_ += len;
	return true;
}

size_t PipePage::Left() const
{
	size_t tail = offset_ + size_;
	if (tail >= buflen_)
		return 0;
	else
		return (buflen_ - tail);
}


#ifndef __QUARK_PIPE_PAGE_H
#define __QUARK_PIPE_PAGE_H 1

#include <windows.h>

class PipePage{
public:
	PipePage();
	~PipePage();

	inline char * Buffer() { return buffer_; }
	inline size_t Buflen() { return buflen_; }
	inline size_t Offset() { return offset_; }
	inline size_t Size() { return size_; }
	inline char * Head() { return buffer_ + offset_; }
	inline char * Tail() { return buffer_ + offset_ + size_; }
	inline bool Empty() const { return (size_ == 0); }
	inline bool Full() const { return (size_ + offset_== buflen_); }

	inline int Status() const { return status_; }
	inline void Status(int status) { status_ = status; }

	bool Consume(size_t len);
	bool Append(size_t len);
	size_t Left() const;

	static const size_t kSize = 4096;

	static const int kIdle		= 0;
	static const int kInSend	= 1;
	static const int kInWrite	= 2;

private:
	char * buffer_;
	size_t buflen_;

	size_t offset_;
	size_t size_;
	int status_;
};

#endif // ! __QUARK_PIPE_PAGE_H



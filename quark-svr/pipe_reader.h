
#ifndef __QUARK_PIPE_READER_H
#define __QUARK_PIPE_READER_H 1

#include <windows.h>
#include "pipe_base.h"
#include "pipe_util.h"

class PipeReader : public PipeBase{
public:
	PipeReader();
	~PipeReader();

	int Read(void  * buffer, int size);
	bool StartRead();

	inline size_t CompletedBytes() const { return completed_bytes_; }
	inline void CompletedBytes(size_t bytes) { completed_bytes_ = bytes; }

private:
	PipeOvlp ovlp_;
	size_t completed_bytes_;
};

#endif // ! __QUARK_PIPE_READER_H



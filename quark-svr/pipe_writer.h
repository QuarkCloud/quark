
#ifndef __QUARK_PIPE_WRITER_H
#define __QUARK_PIPE_WRITER_H 1

#include <windows.h>
#include "pipe_base.h"
#include "pipe_util.h"
#include "pipe_page.h"
#include "pipe_spinlock.h"

class PipeWriter : public PipeBase {
public:
	PipeWriter();
	~PipeWriter();

	int Write(const void * buffer, int buflen);
	void StartSend();
	void Sent();
private:
	PipeOvlp ovlp_;
	PipePage pages_[2];
	PipeSpinlock guard_;
	int sending_index_;	//被锁定用来发送的页，不能写入
	int writing_index_;
	PipePage * PrepareWritePage();
	void CompletedWritePage(PipePage * page);
};

#endif // ! __QUARK_PIPE_WRITER_H



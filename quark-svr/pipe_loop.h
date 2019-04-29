
#ifndef __QUARK_PIPE_LOOP_H
#define __QUARK_PIPE_LOOP_H 1

#include <windows.h>
#include "pipe_base.h"

class PipeLoop {
public:
	PipeLoop();
	~PipeLoop();

	bool Init();
	void Final();
	void Terminate();
	bool Bind(HANDLE handle , PipeBase * obj);
	bool Wait(DWORD& bytes , PipeBase* & obj , PipeOvlp *& ovlp , int timeout);

	inline bool Terminated() const { return terminated_; }
private:
	HANDLE handle_;
	bool terminated_;
	bool finaled_;
	SRWLOCK guard_;
};

#endif // ! __QUARK_PIPE_LOOP_H





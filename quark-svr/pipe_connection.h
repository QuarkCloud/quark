
#ifndef __QUARK_PIPE_CONNECTION_H
#define __QUARK_PIPE_CONNECTION_H 1

#include <windows.h>
#include "pipe_base.h"

class PipeConnection : public PipeBase{
public:
	PipeConnection();
	PipeConnection(HANDLE handle);
	~PipeConnection();

	inline const HANDLE Handle() const { return handle_; }
	inline HANDLE Handle() { return handle_; }
	inline OVERLAPPED *  Ovlp() { return ovlp_.Value(); }

private:
	HANDLE handle_;
	PipeOvlp ovlp_;
};

class PipeConnectionMgr {
public:
	PipeConnectionMgr();
	~PipeConnectionMgr();


	bool InsertConnection(PipeConnection * conn);
	bool DeleteConnection(HANDLE handle);
	bool DeleteConnection(PipeConnection * conn);

	void Clear();
	void Final();

	static const size_t kMaxPipeSize = 256;

private:
	SRWLOCK guard_;
	PipeAccess<PipeConnection> connections_[kMaxPipeSize];
};

#endif // ! __QUARK_PIPE_CONNECTION_H





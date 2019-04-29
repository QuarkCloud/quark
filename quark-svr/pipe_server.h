
#ifndef __QUARK_PIPE_SERVER_H
#define __QUARK_PIPE_SERVER_H 1

#include <windows.h>

#include "pipe_connection.h"
#include "pipe_loop.h"

class PipeServer {
public:
	PipeServer();
	~PipeServer();

	bool Init(const char * name);
	void Final();
	void Run();
	void Terminate();

	inline const char * Name() const { return name_; }
	inline bool Terminated() const { return terminated_; }

private:
	PipeLoop loop_;
	char * name_;
	bool terminated_;
	PipeConnectionMgr connections_;
	bool StartListen();
};

#endif // ! __QUARK_PIPE_SERVER_H





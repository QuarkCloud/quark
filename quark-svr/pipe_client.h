
#ifndef __QUARK_PIPE_CLIENT_H
#define __QUARK_PIPE_CLIENT_H 1

#include <windows.h>
#include "pipe_base.h"

class PipeClient : public PipeBase{
public:
	PipeClient();
	~PipeClient();
};

#endif // ! __QUARK_PIPE_CLIENT_H



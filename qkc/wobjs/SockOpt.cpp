

#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "wobjs/SockOpt.h"

namespace qkc {


	bool SockOpt::KeepAlive(bool &optval)
	{
		BOOL val = FALSE;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, SOL_SOCKET, SO_KEEPALIVE, (char *)&val, &len) == 0)
		{
			optval = (val == TRUE);
			return true;
		}
		return false;

	}
	bool SockOpt::KeepAlive(bool optval)
	{
		BOOL val = (optval == true ? TRUE : FALSE);
		return (::_imp_setsockopt(s_, SOL_SOCKET, SO_KEEPALIVE, (const char *)&val, sizeof(val)) == 0);
	}

	bool SockOpt::RecvBufferSize(int& optval)
	{
		int val = 0;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, SOL_SOCKET, SO_RCVBUF, (char *)&val, &len) == 0)
		{
			optval = val;
			return true;
		}
		return false;
	}

	bool SockOpt::RecvBufferSize(int optval)
	{
		return (::_imp_setsockopt(s_, SOL_SOCKET, SO_RCVBUF, (const char *)&optval, sizeof(optval)) == 0);
	}

	bool SockOpt::SendBufferSize(int& optval)
	{
		int val = 0;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, SOL_SOCKET, SO_SNDBUF, (char *)&val, &len) == 0)
		{
			optval = val;
			return true;
		}
		return false;
	}

	bool SockOpt::SendBufferSize(int optval)
	{
		return (::_imp_setsockopt(s_, SOL_SOCKET, SO_RCVBUF, (const char *)&optval, sizeof(optval)) == 0);
	}

	bool SockOpt::RecvTimeout(DWORD& optval)
	{
		DWORD val = 0;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, SOL_SOCKET, SO_RCVTIMEO, (char *)&val, &len) == 0)
		{
			optval = val;
			return true;
		}
		return false;
	}

	bool SockOpt::RecvTimeout(DWORD optval)
	{
		return (::_imp_setsockopt(s_, SOL_SOCKET, SO_RCVTIMEO, (const char *)&optval, sizeof(optval)) == 0);
	}
	
	bool SockOpt::SendTimeout(DWORD& optval)
	{
		DWORD val = 0;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, SOL_SOCKET, SO_SNDTIMEO, (char *)&val, &len) == 0)
		{
			optval = val;
			return true;
		}
		return false;
	}
	bool SockOpt::SendTimeout(DWORD optval)
	{
		return (::_imp_setsockopt(s_, SOL_SOCKET, SO_SNDTIMEO, (const char *)&optval, sizeof(optval)) == 0);
	}

	bool SockOpt::NoDelay(bool& optval)
	{
		BOOL val = FALSE;
		int len = sizeof(val);
		if (::_imp_getsockopt(s_, IPPROTO_TCP, TCP_NODELAY, (char *)&val, &len) == 0)
		{
			optval = (val == TRUE);
			return true;
		}
		return false;
	}

	bool SockOpt::NoDelay(bool optval)
	{
		BOOL val = (optval == true ? TRUE : FALSE);
		return (::_imp_setsockopt(s_, IPPROTO_TCP, TCP_NODELAY, (char *)&val, sizeof(val)) == 0);
	}

	bool SockOpt::ReadableSize(DWORD& size)
	{
		u_long value = 0;
		if (::_imp_ioctlsocket(s_, FIONREAD, &value) != 0)
			return false;

		size = value;
		return true;
	}

	bool SockOpt::NonBlock(bool enable)
	{
		u_long value = (enable == false ? 0 : 1);
		return (::_imp_ioctlsocket(s_, FIONBIO, &value) == 0);
	}


}

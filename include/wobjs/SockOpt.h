
#ifndef __QUARK_WOBJS_SOCK_OPT_H
#define __QUARK_WOBJS_SOCK_OPT_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>


namespace qkc {

	class QKCAPI SockOpt {
	public:
		SockOpt(SOCKET& s) : s_(s) {}


		bool KeepAlive(bool &optval);
		bool KeepAlive(bool optval);

		bool RecvBufferSize(int& optval);
		bool RecvBufferSize(int optval);

		bool SendBufferSize(int& optval);
		bool SendBufferSize(int optval);

		bool RecvTimeout(DWORD& optval);
		bool RecvTimeout(DWORD optval);

		bool SendTimeout(DWORD& optval);
		bool SendTimeout(DWORD optval);

		bool NoDelay(bool& optval);
		bool NoDelay(bool optval);

		bool ReadableSize(DWORD& size);
		bool NonBlock(bool enable);

	private:
		SOCKET& s_;
	};

}

#endif  /** __QUARK_WOBJS_SOCK_OPT_H */

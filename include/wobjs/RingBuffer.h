
#ifndef __QUARK_WOBJS_RING_BUFFER_H
#define __QUARK_WOBJS_RING_BUFFER_H 1

#include <quark_compile.h>
#include "wobjs/MemMgr.h"

namespace qkc {

	class QKCAPI RingBuffer {
	public:
		RingBuffer(MemMgr * mmgr = NULL);
		virtual ~RingBuffer();

		bool Init(size_t size);
		void Final();

		size_t WriteStream(const void * buf, size_t size);
		bool ReferStream(char *& buf, size_t& size);
		bool MoveSize(size_t size);

		size_t WriteMessage(const void * buf, size_t size);
		bool ReferMessage(char *&buf, size_t& size);
	private:
		MemMgr * mmgr_;
		char * buffer_;
		size_t size_;
		size_t head_;
		size_t tail_;
	};
}

#endif  /** __QUARK_WOBJS_RING_BUFFER_H */

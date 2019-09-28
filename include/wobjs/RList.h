
#ifndef __QKC_WOBJS_RLIST_H
#define __QKC_WOBJS_RLIST_H 1

#include <quark_compile.h>
#include <stdint.h>

namespace qkc {

	class QKCAPI RLNode {
	public:
		RLNode();
		void Init();
		bool Empty() const;
		RLNode * Prev;
		RLNode * Next;
	};

	class QKCAPI RList : public RLNode {
	public:
		RList() {}

		void Add(RLNode * entry);
		void Del(RLNode * entry);
	};
}

#endif /** __QKC_WOBJS_RLIST_H */

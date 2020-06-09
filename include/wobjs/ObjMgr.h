
#ifndef __QUARK_WOBJS_OBJ_MGR_H
#define __QUARK_WOBJS_OBJ_MGR_H 1

#include <quark_compile.h>
#include "wobjs/Object.h"
#include "wobjs/AddrMgr.h"
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		2019-10-24
		管理qkc的内部对象，通过两个维度，一个是ID，由数组来对应；另外一个是句柄，也就是内存地址，主要对应于由windows查找
		2019-11-08
		将Object指针间接保存在节点中，原因是没有地方保存内存地址
	*/
	class QKCAPI ObjMgrNode {
	public:
		ObjMgrNode();
		ObjMgrNode(int idx , void * addr , Object * obj);
		~ObjMgrNode();

		void Clear();
		void Assign(int idx, void * addr, Object * obj);
		inline bool IsEmpty() const { return Obj == NULL; }

		int Index;
		void * Addr;
		Object * Obj;
	};

	class QKCAPI ObjMgr {
	public:
		ObjMgr();
		~ObjMgr();

		int Add(void * addr , Object * obj);
		bool Delete(int index , Object*& obj);
		bool Delete(void * addr , Object *& obj);

		const Object * Get(int idx) const;
		Object * Get(int idx) ;

		const Object * Find(void * addr) const;
		Object * Find(void * addr);

		inline int Size() const { return  size_; }	

		bool CheckExtend() const;
		bool CheckShrink() const;


		/**
			定义对象数组的最小容量，为了避免频繁扩张和收缩，采用的策略是
			1、如果超过原来容量的7/8则扩张空间
			2、如果小于原来容量的3/8则收缩空间
			二者之间有1/8的缓冲空间，则可以避免在临界值附近发生乒乓现象。
		*/
		static const int kMinCapacity = 1024;
		static ObjMgr& Singleton();

	private:
		ObjMgrNode * items_;
		int size_;
		int last_;
		int capacity_;

		AddrMgr addrs_;
		RWLocker guard_;
	} ;
}


#endif  /** __QUARK_WOBJS_OBJ_MGR_H */

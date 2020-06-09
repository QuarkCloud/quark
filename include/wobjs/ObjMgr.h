
#ifndef __QUARK_WOBJS_OBJ_MGR_H
#define __QUARK_WOBJS_OBJ_MGR_H 1

#include <quark_compile.h>
#include "wobjs/Object.h"
#include "wobjs/AddrMgr.h"
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		2019-10-24
		����qkc���ڲ�����ͨ������ά�ȣ�һ����ID������������Ӧ������һ���Ǿ����Ҳ�����ڴ��ַ����Ҫ��Ӧ����windows����
		2019-11-08
		��Objectָ���ӱ����ڽڵ��У�ԭ����û�еط������ڴ��ַ
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
			��������������С������Ϊ�˱���Ƶ�����ź����������õĲ�����
			1���������ԭ��������7/8�����ſռ�
			2�����С��ԭ��������3/8�������ռ�
			����֮����1/8�Ļ���ռ䣬����Ա������ٽ�ֵ��������ƹ������
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


#ifndef __QUARK_WOBJS_CRT_LOADER_H
#define __QUARK_WOBJS_CRT_LOADER_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <windows.h>
#include "wobjs/DllLoader.h"

namespace qkc {

	/**
		20191121
		暂时还没有想到哪些需要动态加载的。主要是为了避免和自实现的函数同名，而将系统提供的函数用动态加载办法改成别的名字。
	*/
	class CrtLoader : public DllLoader {
	public:
		CrtLoader();
		virtual ~CrtLoader();

		virtual bool Init();
		virtual void Final();


	};
}

#endif /** __QUARK_WOBJS_CRT_LOADER_H */

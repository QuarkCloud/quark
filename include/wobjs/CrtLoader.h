
#ifndef __QUARK_WOBJS_CRT_LOADER_H
#define __QUARK_WOBJS_CRT_LOADER_H 1

#include <quark_compile.h>
#include <stdio.h>
#include <windows.h>
#include "wobjs/DllLoader.h"

namespace qkc {

	/**
		20191121
		��ʱ��û���뵽��Щ��Ҫ��̬���صġ���Ҫ��Ϊ�˱������ʵ�ֵĺ���ͬ��������ϵͳ�ṩ�ĺ����ö�̬���ذ취�ĳɱ�����֡�
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

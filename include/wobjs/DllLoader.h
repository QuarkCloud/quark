
#ifndef __QUARK_WOBJS_DLL_LOADER_H
#define __QUARK_WOBJS_DLL_LOADER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		2019-07-26
		�ӳټ��ض�̬�⣬����Ϊ�󲿷�������������еĺ������ᱻ�õ������û�б�Ҫ��̬���ء�
		�����๤����΢Ҫ�鷳�㣬ֱ���Լ�����Ƚϼ򵥡���DllLoader�������ṩ�ֶΣ���Ϊ����Ĺ���
		ÿ���������ж���һ����̬�������ɡ�
	*/
	class QKCAPI DllLoader : public SafeObj{
	public:
		DllLoader();
		virtual ~DllLoader();

		virtual bool Init();
		virtual void Final();

		inline bool Loaded() const { return loaded_; }
		inline void Loaded(bool loaded) { loaded_ = loaded; }

	protected:
		bool Open(const char * name);
		void Close();
		FARPROC GetAddress(const char * name);
		bool ConfirmLoad();
	private:
		HMODULE handle_;
		bool loaded_;
		RWLocker guard_;
	};

}

#endif  /** __QUARK_WOBJS_DLL_LOADER_H */

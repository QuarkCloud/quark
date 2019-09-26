
#ifndef __QUARK_WOBJS_DLL_LOADER_H
#define __QUARK_WOBJS_DLL_LOADER_H 1

#include <quark_compile.h>
#include <windows.h>
#include "wobjs/Object.h"
#include "wobjs/RWLocker.h"

namespace qkc {

	/**
		2019-07-26
		延迟加载动态库，是因为大部分情况，不是所有的函数都会被用到，因此没有必要静态加载。
		采用类工厂稍微要麻烦点，直接自己管理比较简单。在DllLoader基类中提供手段，简化为子类的管理。
		每个子类自行定义一个静态变量即可。
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

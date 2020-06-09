
#ifndef __QUARK_WOBJS_DLL_INIT_H
#define __QUARK_WOBJS_DLL_INIT_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>

namespace qkc {

	QKCAPI uintptr_t GetThisDllHandle();
	QKCAPI void SetThisDllHandle(uintptr_t handle);


}

#endif  /** __QUARK_WOBJS_DLL_INIT_H */

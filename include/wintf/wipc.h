
#ifndef __QKC_WINTF_WIPC_H
#define __QKC_WINTF_WIPC_H 1

#include <quark_compile.h>
#include <windows.h>

#ifdef	__cplusplus
extern "C" {
#endif

/**
    2018-03-16
    cygwin通过一个service来处理进程之间的通讯，qkc使用windows原生共享内存处理。
    事先创建一个全局的进程间共享内存来管理IPC，部分借鉴了boost的IPC解决方案，但
    更贴近原生解决方案，降低实现的代价。
*/
#define IPC_ITEM_SIZE       64 
#define IPC_ITEM_COUNT      1024
#define IPC_SECTION_SIZE    65536

typedef char[IPC_ITEM_SIZE] ipc_item_t ;

typedef struct __st_ipc_section_head{
    uint32_t section_size ;

} ipc_section_head_t;

typedef struct __st_ipc_section{
    ipc_item_t items[1] ;    
} ipc_section_t ;

QKCAPI ipc_section_t * ipc_section_init() ;

QKCAPI void ipc_section_final(ipc_section_t * section) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WIPC_H */

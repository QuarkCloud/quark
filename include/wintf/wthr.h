
#ifndef __QKC_WINTF_WTHR_H
#define __QKC_WINTF_WTHR_H 1

#include <quark_compile.h>
#include <stdint.h>

typedef struct __st_wthr_info{
    int tid ;               //操作系统分配的编号
    int wid ;               //wobj分配的编号
    uintptr_t handle ;
    int errno ;
    int rand ;
    char * errmsg ;
} wthr_info_t ;

__BEGIN_DECLS

QKCAPI uintptr_t dll_handle_get() ;

QKCAPI void dll_handle_set(uintptr_t handle) ;

QKCAPI bool wthr_process_attach() ;

QKCAPI bool wthr_process_detach() ;

QKCAPI bool wthr_thread_attach() ;

QKCAPI bool wthr_thread_detach() ;

//仅仅处理内存，不涉及更多的东西。
QKCAPI wthr_info_t * wthr_info_alloc() ;

QKCAPI void wthr_info_init(wthr_info_t * info) ;

QKCAPI void wthr_info_free(wthr_info_t * info) ;

//保存到本地线程变量中
QKCAPI bool wthr_info_save(wthr_info_t * info) ;
//从本地线程变量中删除
QKCAPI void wthr_info_remove() ;
//从本地线程变量中加载
QKCAPI wthr_info_t * wthr_info_load() ;


//线程变量的管理
typedef void (*tls_cleanup_t)(void *) ;

QKCAPI int wthr_tls_alloc(tls_cleanup_t cleanup) ;

QKCAPI void wthr_tls_delete(int tls) ;

QKCAPI void wthr_tls_cleanup() ;

QKCAPI int wthr_tls_first() ;

QKCAPI int wthr_tls_next(int prev) ;

QKCAPI int wthr_tls_set_val(int key , void * val) ;

QKCAPI void * wthr_tls_get_val(int key) ;

QKCAPI void wthr_tls_free_val(int key) ;

QKCAPI void wthr_tls_cleanup_vals() ;

__END_DECLS

#endif /** __QKC_WINTF_WTHR_H */

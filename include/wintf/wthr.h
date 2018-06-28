
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

#ifdef	__cplusplus
extern "C" {
#endif

QKCAPI bool wthr_process_attach() ;

QKCAPI bool wthr_process_detach() ;

QKCAPI bool wthr_thread_attach() ;

QKCAPI bool wthr_thread_detach() ;

QKCAPI wthr_info_t * wthr_info_get() ;

QKCAPI void wthr_info_init(wthr_info_t * info) ;

QKCAPI void wthr_info_free(wthr_info_t * info) ;

//线程变量的管理
typedef void (*tls_cleanup_t)(int tls) ;

QKCAPI int whtr_tls_alloc(tls_cleanup_t cleanup) ;

QKCAPI void whtr_tls_delete(int tls) ;

QKCAPI void whtr_tls_cleanup() ;

QKCAPI int whtr_tls_first() ;

QKCAPI int whtr_tls_next() ;

QKCAPI int whtr_tls_set_val(int pid , int key , void * val) ;

QKCAPI void * whtr_tls_get_val(int pid , int key) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WTHR_H */

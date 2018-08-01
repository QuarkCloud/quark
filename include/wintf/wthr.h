
#ifndef __QKC_WINTF_WTHR_H
#define __QKC_WINTF_WTHR_H 1

#include <quark_compile.h>
#include <stdint.h>

typedef struct __st_wthr_info{
    int tid ;               //����ϵͳ����ı��
    int wid ;               //wobj����ı��
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

//���������ڴ棬���漰����Ķ�����
QKCAPI wthr_info_t * wthr_info_alloc() ;

QKCAPI void wthr_info_init(wthr_info_t * info) ;

QKCAPI void wthr_info_free(wthr_info_t * info) ;

//���浽�����̱߳�����
QKCAPI bool wthr_info_save(wthr_info_t * info) ;
//�ӱ����̱߳�����ɾ��
QKCAPI void wthr_info_remove() ;
//�ӱ����̱߳����м���
QKCAPI wthr_info_t * wthr_info_load() ;


//�̱߳����Ĺ���
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

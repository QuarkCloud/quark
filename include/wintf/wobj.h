
#ifndef __QKC_WINTF_WOBJ_H
#define __QKC_WINTF_WOBJ_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>

__BEGIN_DECLS


/**
    linuxʹ��int��ʾ����ϵͳ�������������windows��ʹ��HANDLE��ʾ����windows��crt�У�
    ֻ���ļ��������ת����������û�д���Ϊ��ʵ��posix�ӿڣ���Ҫһ��ӳ���������������֮��Ĳ��졣
    ���ļ�������linux�����ã�Ҳ����ֱ����windows��ʹ��

    ���ͷ�ļ���Ҫֱ������
*/
typedef enum __wobj_type__{
    WOBJ_VOID ,
    WOBJ_OTHR ,
    WOBJ_PROC ,
    WOBJ_THRD ,
    WOBJ_FILE ,
    WOBJ_MODU ,
    WOBJ_MUTEX ,
    WOBJ_SEMA ,
    WOBJ_EVENT ,
    WOBJ_SOCK ,
    WOBJ_SHM ,
    WOBJ_NOTF ,
    WOBJ_IOCP ,
    WOBJ_RWLOCK,
	WOBJ_PIPE ,
	WOBJ_ALIAS	//��������Ҫ����pipe
} wobj_type;

#define WOBJ_ID_BASE_SHIFT      20
#define WOBJ_ID_BASE            (1 << WOBJ_ID_BASE_SHIFT)
#define INVALID_WOBJ_ID         0

typedef struct __st_wobj{    
    int wid ;
    wobj_type type ;
    HANDLE handle ;
    void * addition ;
} wobj_t ;


QKCAPI wobj_t * wobj_get(int wid) ;

QKCAPI int wobj_set(wobj_type type , HANDLE handle , void * addition) ;

QKCAPI bool wobj_del(int wid) ;

QKCAPI wobj_t *  wobj_find_by_handle(wobj_type type , HANDLE handle) ;

QKCAPI HANDLE default_heap_get() ;

QKCAPI void global_wrlock() ;

QKCAPI void global_wrunlock() ;

QKCAPI void global_rdlock() ;

QKCAPI void global_rdunlock() ;

__END_DECLS

#endif /** __QKC_WINTF_WOBJ_H */

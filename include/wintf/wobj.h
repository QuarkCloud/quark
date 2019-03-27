
#ifndef __QKC_WINTF_WOBJ_H
#define __QKC_WINTF_WOBJ_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>

__BEGIN_DECLS


/**
    linux使用int表示描述系统对象的索引，而windows是使用HANDLE表示。在windows的crt中，
    只对文件句柄作了转化，其他都没有处理。为了实现posix接口，需要一个映射机制来屏蔽两者之间的差异。
    该文件不能在linux下引用，也不能直接在windows中使用

    这个头文件不要直接引用
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
	WOBJ_ALIAS	//别名，主要用于pipe
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

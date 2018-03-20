
#ifndef __QKC_WINTF_WIPC_H
#define __QKC_WINTF_WIPC_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

/**
    2018-03-16
    cygwin通过一个service来处理进程之间的通讯，qkc由于是动态库，所以使用windows
    原生共享内存处理。事先创建一个全局的进程间共享内存来管理IPC，部分借鉴了boost的
    IPC解决方案，但更贴近原生解决方案，降低实现的代价。

    2018-03-20
    WIPC在空间利用上浪费颇多，等待继续完善。
*/
#define IPC_ITEM_SIZE       128 
#define IPC_ITEM_COUNT      512
#define IPC_SECTION_SIZE    65536

#define IPC_TYPE_UNK        0
#define IPC_TYPE_SHM        1
#define IPC_TYPE_SEM        2
#define IPC_TYPE_MTX        3
#define IPC_TYPE_EVT        4
#define IPC_TYPE_MAX        5

static const char * ipc_type_names[IPC_TYPE_MAX] = {
    "\0" , "shm" , "sem" , "mtx" , "evt"
} ;

#define IPC_NAME_UNK        {'\0' , '\0' , '\0'}
#define IPC_NAME_SHM        {'s' , 'h' , 'm'}
#define IPC_NAME_SEM        {'s' , 'e' , 'm'}
#define IPC_NAME_MTX        {'m' , 't' , 'x'}
#define IPC_NAME_EVT        {'e' , 'v' , 't'}

typedef struct __st_ipc_item{
    uint8_t cont[IPC_ITEM_SIZE] ;
} ipc_item_t;

typedef struct __st_ipc_item_head{
    char        magic[4] ;
    uint32_t    hash ;                 //用于校验名字
} ipc_item_head_t ;

static const char * IPC_HEAD_MAGIC = "qkc" ;

typedef struct __st_ipc_section_head{
    uint8_t     magic[4] ;
    int32_t     start ;
    int32_t     count ;
    int32_t     last_id ;
    uint8_t     bitmap[8] ;
} ipc_section_head_t;

typedef struct __st_ipc_shm_item{
    ipc_item_head_t     head ;
    int32_t             key ;
    int32_t             shmid ;
    int                 perms ;
    uint32_t            bytes ;
    uint32_t            nattch ;
    char                name[4] ;
} ipc_shm_item_t ;

typedef struct __st_ipc_sem_item{
    ipc_item_head_t     head ;
    int32_t             semid ;
    int                 perms ;
    uint32_t            nsems ;
    uint32_t            hash ;
    char                name[8] ;
} ipc_sem_item_t ;

typedef struct __st_ipc_bulk_item{
    ipc_item_head_t     head ;
    uint32_t            size ;
    uint32_t            nattch ;
} ipc_bulk_item_t ;


typedef struct __st_ipc_section{
    ipc_item_t          items[IPC_ITEM_COUNT] ;
} ipc_section_t ;

QKCAPI const char * ipc_name() ;
QKCAPI const char * ipc_glmtx_name() ;
QKCAPI const char * ipc_glshm_name() ;

QKCAPI ipc_section_t * ipc_section_init() ;

QKCAPI ipc_section_t * ipc_section_get() ;

QKCAPI void ipc_section_final() ;

QKCAPI int ipc_item_get_type(const ipc_item_head_t * head) ;

QKCAPI bool ipc_item_set_type(ipc_item_head_t * head , int type) ;

QKCAPI ipc_item_t * ipc_item_alloc(int type , const char * name) ;

QKCAPI bool ipc_item_free(const ipc_item_t * item) ;

#ifdef	__cplusplus
}
#endif

#endif /** __QKC_WINTF_WIPC_H */

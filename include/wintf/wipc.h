
#ifndef __QKC_WINTF_WIPC_H
#define __QKC_WINTF_WIPC_H 1

#include <quark_compile.h>
#include <windows.h>
#include <stdint.h>
#include <sys/ipc.h>

__BEGIN_DECLS

/**
    2018-03-16
    cygwinͨ��һ��service���������֮���ͨѶ��qkc�����Ƕ�̬�⣬����ʹ��windows
    ԭ�������ڴ洦�����ȴ���һ��ȫ�ֵĽ��̼乲���ڴ�������IPC�����ֽ����boost��
    IPC�����������������ԭ���������������ʵ�ֵĴ��ۡ�

    2018-03-20
    WIPC�ڿռ��������˷��Ķ࣬�ȴ��������ơ�

    2018-05-10
    ����һ�������Ĺ�������������һ�������飬һ�������飬���ڿ��ټ��key_t�Ƿ���ڣ�
    һ����ϸ�飬��������������Լ�����ϸ�ڡ�
*/

//��linux��ֻ֧���ź�����Ϊ����ͬ��������Ϣ������ʱ�����ˡ�
#define IPC_TYPE_UNK        0
#define IPC_TYPE_SHM        1
#define IPC_TYPE_SEM        2
#define IPC_TYPE_MTX        3
#define IPC_TYPE_MAX        4

static const char * ipc_type_names[IPC_TYPE_MAX] = {"\0" , "shm" , "sem" , "mtx"} ;

#define IPC_NAME_UNK        {'\0' , '\0' , '\0'}
#define IPC_NAME_SHM        {'s' , 'h' , 'm'}
#define IPC_NAME_SEM        {'s' , 'e' , 'm'}
#define IPC_NAME_MTX        {'m' , 't' , 'x'}

//12���ֽ�
#define IPC_ITEM_MEMBERS    \
    uint32_t    key ;       \
    uint32_t    id ;        \
    uint16_t    type ;      \
    uint16_t    perms ;     \
    uint32_t    nattch ;    


typedef struct __st_ipc_item{
    IPC_ITEM_MEMBERS
    uint8_t    pad0[16] ;       //����32���ֽ�
} ipc_item_t;

typedef struct __st_ipc_shm{
    IPC_ITEM_MEMBERS 
    uint32_t    bytes;
    int32_t     atime;
    int32_t     dtime;
    int32_t     ctime;
}ipc_shm_t ;

typedef struct __st_ipc_sem{
    IPC_ITEM_MEMBERS 
    int32_t     otime;
    int32_t     ctime;
    volatile long        value ;
    uint8_t     pad1[4] ;
}ipc_sem_t ;


#define IPC_INFO_BITMAP_SIZE    1024 
#define IPC_ITEM_COUNT          8192
#define IPC_SUPER_SIZE          4096
#define IPC_GLOBAL_SIZE         0X40000     //256K

//������
typedef struct __st_ipc_super{
    char        magic[4] ;      //4
    uint32_t    ipc_size ;      //8
    uint8_t     major ;
    uint8_t     minor ;
    uint16_t    build ;         //12

    uint16_t    offset ;
    uint16_t    count   ;       //16
    uint32_t    last_id ;       //20

    uint8_t     unused[236] ;   //�ճ�256
    uint8_t     bitmap[IPC_INFO_BITMAP_SIZE] ;
}ipc_super_t ;

//���������ڴ��Ķ���
typedef struct __st_ipc_global{
    uint8_t super[IPC_SUPER_SIZE] ;
    ipc_item_t  items[IPC_ITEM_COUNT] ;
}ipc_global_t ;

QKCAPI const char * ipc_mtx_name() ;
QKCAPI const char * ipc_shm_name() ;
QKCAPI const char * ipc_sem_name() ;

QKCAPI uint32_t ipc_version() ;

QKCAPI bool ipc_super_validate_magic(ipc_super_t * super) ;
QKCAPI void ipc_super_assign_magic(ipc_super_t * super) ;
QKCAPI void ipc_super_bitmap_set(ipc_super_t * super , uint32_t id , bool val) ;
QKCAPI void ipc_super_bitmap_get(ipc_super_t * super , uint32_t id , bool& val) ;


QKCAPI bool ipc_global_init() ;
QKCAPI bool ipc_global_final() ;

QKCAPI int ipc_alloc_id(key_t key , int type , int flag) ;
QKCAPI void ipc_free_id(int id) ;

QKCAPI ipc_item_t * ipc_get_item_by_id(int id) ;


/**
    2018-05-11
    ����Ĺ����ڴ汾��ʵ��
*/
typedef struct __st_win_shm{
    uint32_t key ;
    uint32_t shmid ;
    int      oid ;
    HANDLE fhandle ;
    void * map_addr ;
    size_t size ;
    char * name ;
    DWORD page_protect ;
    DWORD map_access ;
    ipc_shm_t * ipc ;
} win_shm_t ;

QKCAPI win_shm_t * ipc_shm_create(uint32_t shmid , size_t size) ;
QKCAPI bool ipc_shm_init(win_shm_t * shm) ;
QKCAPI bool ipc_shm_final(win_shm_t * shm) ;
QKCAPI bool ipc_shm_destroy(win_shm_t * shm) ;

QKCAPI bool ipc_shm_addr_add(void * addr , win_shm_t * shm) ;
QKCAPI win_shm_t * ipc_shm_addr_find(const void * addr) ;
QKCAPI bool ipc_shm_addr_del(const void * addr) ;

/**
    2018-05-26
    �ź����ı���ʵ��
*/
typedef struct __st_win_sem{
    uint32_t    key ;
    uint32_t    semid ;
    int         oid ;

    HANDLE      handle ;
    char *      name ;
    ipc_sem_t  *ipc ;
} win_sem_t;

QKCAPI win_sem_t * ipc_sem_create(uint32_t semid) ;
QKCAPI bool ipc_sem_init(win_sem_t * sem) ;
QKCAPI bool ipc_sem_final(win_sem_t * sem) ;
QKCAPI bool ipc_sem_destroy(win_sem_t * sem) ;

__END_DECLS

#endif /** __QKC_WINTF_WIPC_H */

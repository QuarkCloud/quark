
#include <wintf/wipc.h>
#include <wintf/wobj.h>
#include <windows.h>

static const char * __wipc_name__ = "Global\\qkc.ipc" ;

int ipc_item_get_type(const ipc_item_head_t * head)
{
    char type = head->magic[0] ;
    if(type <= 0  || type >= IPC_TYPE_MAX)
        return IPC_TYPE_UNK ;

    const char * desc = head->magic + 1 ;
    const char * names = ipc_type_names[type] ;
    if(desc[0] == names[0] && desc[1] == names[1] && desc[2] == names[2])
        return type ;
    else
        return IPC_TYPE_UNK ; 
}

bool ipc_item_set_type(ipc_item_head_t * head , int type)
{
    if(head == NULL || (type <= 0 || type >= IPC_TYPE_MAX))
        return false ;

    char * magic = head->magic ;
    const char * names = ipc_type_names[type] ;
    magic[0] = (char)type ;
    magic[1] = names[0] ;
    magic[2] = names[1] ;
    magic[3] = names[2] ;

    return true ;
}

static ipc_section_t * __global_ipc_section__ = NULL ;
static HANDLE __global_ipc_section_mutex__ = NULL ;

ipc_section_t * ipc_section_init() 
{
    global_wrlock() ;
    //char 

    return NULL ;
}

void ipc_section_final(ipc_section_t * section) 
{

}


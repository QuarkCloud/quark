
#ifndef __QKC_INTERNAL_MMAN_MGR_H
#define __QKC_INTERNAL_MMAN_MGR_H 1


#include <sys/mman.h>
#include <windows.h>
#include <wintf/wcrt.h>
#include "rlist.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

__BEGIN_DECLS


typedef enum __mmap_func_type__ {
	MMAP_FUNC_TYPE_NONE = 0,
	MMAP_FUNC_TYPE_FILE = 1,
	MMAP_FUNC_TYPE_ALLOC = 2
} mmap_func_type;

/**
	2019-03-11
	为了兼容FileMap和VirtualAlloc，设计并不好，先这样吧。
	linux下的mmap，有一些和windows不太一样的行为，也是够烧脑子的了。
*/
typedef struct __st_mmap_commits {
	rlist_t link;
	void * start_addr;
	size_t len ;
} mmap_commits_t;

typedef struct __st_mmap_info {
	rlist_t link;
	SRWLOCK  locker;

	int fd;
	int prot;
	int flags;
	DWORD wflags;

	size_t len;
	off_t offset;
	void * expect_addr;

	rlist_t commits;

	HANDLE file_handle;
	HANDLE map_handle;
	void * map_addr;

	//用于munmap
	void * use_addr;
	size_t use_size;
	mmap_func_type func_type;
	char * name ;
} mmap_info_t;

//偷个懒，因为mmap本身就不会太多，所以也无需太复杂。
typedef struct __st_mmap_mgr {
	SRWLOCK guard;
	rlist_t infos;
}mmap_mgr_t;

mmap_mgr_t * mmap_mgr_default();

bool mmap_mgr_insert(mmap_mgr_t * mgr , mmap_info_t * info);
bool mmap_mgr_delete(mmap_mgr_t * mgr, mmap_info_t *info);
mmap_info_t * mmap_mgr_find(mmap_mgr_t * mgr , void * addr);

bool mmap_info_alloc(mmap_info_t *& info);
bool mmap_info_free(mmap_info_t * info);
mmap_commits_t * mmap_info_find(mmap_info_t * info , void * addr , size_t size);
bool mmap_info_commit(mmap_info_t * info, void * addr, size_t size);
bool mmap_info_decommit(mmap_info_t * info, void * addr, size_t size);

static inline bool mmap_info_inside(mmap_info_t * info, void * addr , size_t size)
{
	if((uintptr_t)info->map_addr > (uintptr_t)addr)
		return false ;

	if (size == 0)
		return ((uintptr_t)info->map_addr + info->len > (uintptr_t)addr);
	else
		return  ((uintptr_t)info->map_addr + info->len >= (uintptr_t)addr + size);
}

DWORD __mmap_prot_to_win(int prot);
DWORD __mmap_flag_to_win(int flags);

__END_DECLS

#endif /** __QKC_INTERNAL_MMAN_MGR_H */

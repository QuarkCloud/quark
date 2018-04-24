
#ifndef __EPOLL_MGR_H
#define __EPOLL_MGR_H 1

#include <sys/epoll.h>
#include <wintf/wobj.h>
#include <windows.h>
#include <winsock2.h>
#include <errno.h>
#include "inner/rlist.h"
#include "inner/rbtree.h"
#include "inner/fsocket.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct __st_epoll_mgr{
    
} epoll_mgr_t;



#ifdef __cplusplus
}
#endif

#endif /** __EPOLL_MGR_H */

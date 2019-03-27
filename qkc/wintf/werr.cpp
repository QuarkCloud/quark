
#include <wintf/werr.h>
#include <errno.h>
#include <winerror.h>

struct errentry {
    DWORD oscode;           /* OS return value */
    int errnocode;  /* System V error code */
};

static struct errentry __errtable__[] = {
    {  ERROR_INVALID_FUNCTION,       EINVAL    },  /* 1 */
    {  ERROR_FILE_NOT_FOUND,         ENOENT    },  /* 2 */
    {  ERROR_PATH_NOT_FOUND,         ENOENT    },  /* 3 */
    {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },  /* 4 */
    {  ERROR_ACCESS_DENIED,          EACCES    },  /* 5 */
    {  ERROR_INVALID_HANDLE,         EBADF     },  /* 6 */
    {  ERROR_ARENA_TRASHED,          ENOMEM    },  /* 7 */
    {  ERROR_NOT_ENOUGH_MEMORY,      ENOMEM    },  /* 8 */
    {  ERROR_INVALID_BLOCK,          ENOMEM    },  /* 9 */
    {  ERROR_BAD_ENVIRONMENT,        E2BIG     },  /* 10 */
    {  ERROR_BAD_FORMAT,             ENOEXEC   },  /* 11 */
    {  ERROR_INVALID_ACCESS,         EINVAL    },  /* 12 */
    {  ERROR_INVALID_DATA,           EINVAL    },  /* 13 */
    {  ERROR_INVALID_DRIVE,          ENOENT    },  /* 15 */
    {  ERROR_CURRENT_DIRECTORY,      EACCES    },  /* 16 */
    {  ERROR_NOT_SAME_DEVICE,        EXDEV     },  /* 17 */
    {  ERROR_NO_MORE_FILES,          ENOENT    },  /* 18 */
    {  ERROR_LOCK_VIOLATION,         EACCES    },  /* 33 */
    {  ERROR_BAD_NETPATH,            ENOENT    },  /* 53 */
    {  ERROR_NETWORK_ACCESS_DENIED,  EACCES    },  /* 65 */
    {  ERROR_BAD_NET_NAME,           ENOENT    },  /* 67 */
    {  ERROR_FILE_EXISTS,            EEXIST    },  /* 80 */
    {  ERROR_CANNOT_MAKE,            EACCES    },  /* 82 */
    {  ERROR_FAIL_I24,               EACCES    },  /* 83 */
    {  ERROR_INVALID_PARAMETER,      EINVAL    },  /* 87 */
    {  ERROR_NO_PROC_SLOTS,          EAGAIN    },  /* 89 */
    {  ERROR_DRIVE_LOCKED,           EACCES    },  /* 108 */
    {  ERROR_BROKEN_PIPE,            EPIPE     },  /* 109 */
    {  ERROR_DISK_FULL,              ENOSPC    },  /* 112 */
    {  ERROR_INVALID_TARGET_HANDLE,  EBADF     },  /* 114 */
    {  ERROR_INVALID_HANDLE,         EINVAL    },  /* 124 */
    {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },  /* 128 */
    {  ERROR_CHILD_NOT_COMPLETE,     ECHILD    },  /* 129 */
    {  ERROR_DIRECT_ACCESS_HANDLE,   EBADF     },  /* 130 */
    {  ERROR_NEGATIVE_SEEK,          EINVAL    },  /* 131 */
    {  ERROR_SEEK_ON_DEVICE,         EACCES    },  /* 132 */
    {  ERROR_DIR_NOT_EMPTY,          ENOTEMPTY },  /* 145 */
    {  ERROR_NOT_LOCKED,             EACCES    },  /* 158 */
    {  ERROR_BAD_PATHNAME,           ENOENT    },  /* 161 */
    {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },  /* 164 */
    {  ERROR_LOCK_FAILED,            EACCES    },  /* 167 */
    {  ERROR_ALREADY_EXISTS,         EEXIST    },  /* 183 */
    {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },  /* 206 */
    {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },  /* 215 */
    {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    },   /* 1816 */
		
	{ ERROR_NOACCESS,                    	EACCES	},
	{ WSAEACCES,                         	EACCES	},
	{ ERROR_ELEVATION_REQUIRED,          	EACCES	},
	{ ERROR_ADDRESS_ALREADY_ASSOCIATED,  	EADDRINUSE	},
	{ WSAEADDRINUSE,                     	EADDRINUSE	},
	{ WSAEADDRNOTAVAIL,                  	EADDRNOTAVAIL	},
	{ WSAEAFNOSUPPORT,                   	EAFNOSUPPORT	},
	{ WSAEWOULDBLOCK,                    	EAGAIN	},
	{ WSAEALREADY,                       	EALREADY	},
	{ ERROR_INVALID_FLAGS,               	EBADF	},
	{ ERROR_INVALID_HANDLE,              	EBADF	},
	{ ERROR_LOCK_VIOLATION,              	EBUSY	},
	{ ERROR_PIPE_BUSY,                   	EBUSY	},
	{ ERROR_SHARING_VIOLATION,           	EBUSY	},
	{ ERROR_OPERATION_ABORTED,           	ECANCELED	},
	{ WSAEINTR,                          	ECANCELED	},
	//{ ERROR_NO_UNICODE_TRANSLATION,      	ECHARSET	},
	{ ERROR_CONNECTION_ABORTED,          	ECONNABORTED	},
	{ WSAECONNABORTED,                   	ECONNABORTED	},
	{ ERROR_CONNECTION_REFUSED,          	ECONNREFUSED	},
	{ WSAECONNREFUSED,                   	ECONNREFUSED	},
	{ ERROR_NETNAME_DELETED,             	ECONNRESET	},
	{ WSAECONNRESET,                     	ECONNRESET	},
	{ ERROR_ALREADY_EXISTS,              	EEXIST	},
	{ ERROR_FILE_EXISTS,                 	EEXIST	},
	{ ERROR_BUFFER_OVERFLOW,             	EFAULT	},
	{ WSAEFAULT,                         	EFAULT	},
	{ ERROR_HOST_UNREACHABLE,            	EHOSTUNREACH	},
	{ WSAEHOSTUNREACH,                   	EHOSTUNREACH	},
	{ ERROR_INSUFFICIENT_BUFFER,         	EINVAL	},
	{ ERROR_INVALID_DATA,                	EINVAL	},
	{ ERROR_INVALID_PARAMETER,           	EINVAL	},
	{ ERROR_SYMLINK_NOT_SUPPORTED,       	EINVAL	},
	{ WSAEINVAL,                         	EINVAL	},
	{ WSAEPFNOSUPPORT,                   	EINVAL	},
	{ WSAESOCKTNOSUPPORT,                	EINVAL	},
	{ ERROR_BEGINNING_OF_MEDIA,          	EIO	},
	{ ERROR_BUS_RESET,                   	EIO	},
	{ ERROR_CRC,                         	EIO	},
	{ ERROR_DEVICE_DOOR_OPEN,            	EIO	},
	{ ERROR_DEVICE_REQUIRES_CLEANING,    	EIO	},
	{ ERROR_DISK_CORRUPT,                	EIO	},
	{ ERROR_EOM_OVERFLOW,                	EIO	},
	{ ERROR_FILEMARK_DETECTED,           	EIO	},
	{ ERROR_GEN_FAILURE,                 	EIO	},
	{ ERROR_INVALID_BLOCK_LENGTH,        	EIO	},
	{ ERROR_IO_DEVICE,                   	EIO	},
	{ ERROR_NO_DATA_DETECTED,            	EIO	},
	{ ERROR_NO_SIGNAL_SENT,              	EIO	},
	{ ERROR_OPEN_FAILED,                 	EIO	},
	{ ERROR_SETMARK_DETECTED,            	EIO	},
	{ ERROR_SIGNAL_REFUSED,              	EIO	},
	{ WSAEISCONN,                        	EISCONN	},
	{ ERROR_CANT_RESOLVE_FILENAME,       	ELOOP	},
	{ ERROR_TOO_MANY_OPEN_FILES,         	EMFILE	},
	{ WSAEMFILE,                         	EMFILE	},
	{ WSAEMSGSIZE,                       	EMSGSIZE	},
	{ ERROR_FILENAME_EXCED_RANGE,        	ENAMETOOLONG	},
	{ ERROR_NETWORK_UNREACHABLE,         	ENETUNREACH	},
	{ WSAENETUNREACH,                    	ENETUNREACH	},
	{ WSAENOBUFS,                        	ENOBUFS	},
	{ ERROR_BAD_PATHNAME,                	ENOENT	},
	{ ERROR_DIRECTORY,                   	ENOENT	},
	{ ERROR_FILE_NOT_FOUND,              	ENOENT	},
	{ ERROR_INVALID_NAME,                	ENOENT	},
	{ ERROR_INVALID_DRIVE,               	ENOENT	},
	{ ERROR_INVALID_REPARSE_DATA,        	ENOENT	},
	{ ERROR_MOD_NOT_FOUND,               	ENOENT	},
	{ ERROR_PATH_NOT_FOUND,              	ENOENT	},
	{ WSAHOST_NOT_FOUND,                 	ENOENT	},
	{ WSANO_DATA,                        	ENOENT	},
	{ ERROR_NOT_ENOUGH_MEMORY,           	ENOMEM	},
	{ ERROR_OUTOFMEMORY,                 	ENOMEM	},
	{ ERROR_CANNOT_MAKE,                 	ENOSPC	},
	{ ERROR_DISK_FULL,                   	ENOSPC	},
	{ ERROR_EA_TABLE_FULL,               	ENOSPC	},
	{ ERROR_END_OF_MEDIA,                	ENOSPC	},
	{ ERROR_HANDLE_DISK_FULL,            	ENOSPC	},
	{ ERROR_NOT_CONNECTED,               	ENOTCONN	},
	{ WSAENOTCONN,                       	ENOTCONN	},
	{ ERROR_DIR_NOT_EMPTY,               	ENOTEMPTY	},
	{ WSAENOTSOCK,                       	ENOTSOCK	},
	{ ERROR_NOT_SUPPORTED,               	ENOTSUP	},
	{ ERROR_BROKEN_PIPE,                 	EOF	},
	{ ERROR_ACCESS_DENIED,               	EPERM	},
	{ ERROR_PRIVILEGE_NOT_HELD,          	EPERM	},
	{ ERROR_BAD_PIPE,                    	EPIPE	},
	{ ERROR_NO_DATA,                     	EPIPE	},
	{ ERROR_PIPE_NOT_CONNECTED,          	EPIPE	},
	{ WSAESHUTDOWN,                      	EPIPE	},
	{ WSAEPROTONOSUPPORT,                	EPROTONOSUPPORT	},
	{ ERROR_WRITE_PROTECT,               	EROFS	},
	{ ERROR_SEM_TIMEOUT,                 	ETIMEDOUT	},
	{ WSAETIMEDOUT,                      	ETIMEDOUT	},
	{ ERROR_NOT_SAME_DEVICE,             	EXDEV	},
	{ ERROR_INVALID_FUNCTION,            	EISDIR	},
	{ ERROR_META_EXPANSION_TOO_LONG,     	E2BIG	} ,
    { 0                            ,     	0	} 
};

int oserr_map(DWORD oserr)
{
    int idx = 0 ;
    while(true)
    {
        struct errentry *entry = __errtable__ + idx ;
        if(entry->oscode == 0)
            break ;

        if(entry->oscode == oserr)
            return entry->errnocode ;
        ++idx ;
    }

    return 0 ;
}


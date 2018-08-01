
#ifndef __QKC_SYS_TYPES_H
#define __QKC_SYS_TYPES_H 1

#include <quark_compile.h>
#include <stdint.h>

__BEGIN_DECLS


typedef long int                long_t ;
typedef unsigned long int       ulong_t ;

typedef int64_t                 quad_t  ;
typedef uint64_t                uquad_t  ;

typedef uint32_t                uid_t ;
typedef uint32_t                gid_t ;
typedef ulong_t                 dev_t ;
typedef ulong_t                 ino_t ;
typedef uquad_t                 ino64_t ;
typedef uint32_t                mode_t;
typedef long_t                  off_t ;
typedef quad_t                  off64_t ;
typedef quad_t                  fpos_t ;
typedef int32_t                 pid_t ;
typedef quad_t                  clock_t ;
typedef long_t                  ssize_t ;
typedef uint32_t                useconds_t ;
typedef long_t                  suseconds_t ;
typedef int64_t                 time_t ;
typedef int32_t                 key_t ;
typedef int32_t                 clockid_t ;
typedef uint32_t                socklen_t ;
typedef ulong_t                 nlink_t ;


__END_DECLS

#endif  /** __QKC_SYS_TYPES_H */

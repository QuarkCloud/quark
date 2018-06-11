
#ifndef __QKC_DLFCN_H
#define __QKC_DLFCN_H 1

#include <quark_compile.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
    在windows下，这些标识都是无效的，只是为了编译可以通过。统一设置为NOW。
*/
#define RTLD_LAZY	0x00001	/* Lazy function call binding.  */
#define RTLD_NOW	0x00002	/* Immediate function call binding.  */
#define	RTLD_BINDING_MASK   0x3	/* Mask of binding time value.  */
#define RTLD_NOLOAD	0x00004	/* Do not load the object.  */
#define RTLD_DEEPBIND	0x00008	/* Use deep binding.  */

/* If the following bit is set in the MODE argument to `dlopen',
   the symbols of the loaded object and its dependencies are made
   visible as if the object were linked directly into the program.  */
#define RTLD_GLOBAL	0x00100

# define RTLD_NEXT      ((void *) -1l)
# define RTLD_DEFAULT   ((void *) 0)


QKCAPI void *dlopen (const char * file, int mode) ;

QKCAPI int dlclose (void * handle) ;

QKCAPI void *dlsym (void * handle , const char * name) ;

QKCAPI char *dlerror (void) ;

#ifdef __cplusplus
}
#endif


#endif	/** __QKC_DLFCN_H */

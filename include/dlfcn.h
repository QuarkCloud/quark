
#ifndef __QKC_DLFCN_H
#define __QKC_DLFCN_H 1

#include <quark_compile.h>

__BEGIN_DECLS

/**
    ��windows�£���Щ��ʶ������Ч�ģ�ֻ��Ϊ�˱������ͨ����ͳһ����ΪNOW��
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

/**
    RTLD_DEFAULT/RTLD_NEXT��Ҫ���⴦��
*/
QKCAPI void *dlsym (void * handle , const char * name) ;

QKCAPI char *dlerror (void) ;

__END_DECLS


#endif	/** __QKC_DLFCN_H */


#ifndef __QKC_STDDEF_H
#define __QKC_STDDEF_H 1


#ifdef container_of
#undef container_of
#endif

#define container_of(ptr, type, member) ((type *) ((char *) (ptr) - offsetof(type, member)))


#ifdef offsetof
#undef offsetof
#endif

#define offsetof(TYPE, MEMBER)	((size_t)&((TYPE *)0)->MEMBER)


#endif  /** __QKC_STDDEF_H */

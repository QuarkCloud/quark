/* Abstraction layer for threading in tests. */
typedef pthread_t thd_t;

void	thd_create(thd_t *thd, void *(*proc)(void *), void *arg);
void	thd_join(thd_t thd, void **ret);

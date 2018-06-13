#include "test/jemalloc_test.h"

void
thd_create(thd_t *thd, void *(*proc)(void *), void *arg) {
	if (pthread_create(thd, NULL, proc, arg) != 0) {
		test_fail("Error in pthread_create()\n");
	}
}

void
thd_join(thd_t thd, void **ret) {
	pthread_join(thd, ret);
}


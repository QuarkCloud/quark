#include "test/jemalloc_test.h"

/*
 * Sleep for approximately ns nanoseconds.  No lower *nor* upper bound on sleep
 * time is guaranteed.
 */
void
mq_nanosleep(unsigned ns) {
	assert(ns <= 1000*1000*1000);

	{
		struct timespec timeout;

		if (ns < 1000*1000*1000) {
			timeout.tv_sec = 0;
			timeout.tv_nsec = ns;
		} else {
			timeout.tv_sec = 1;
			timeout.tv_nsec = 0;
		}
		nanosleep(&timeout, NULL);
	}
}

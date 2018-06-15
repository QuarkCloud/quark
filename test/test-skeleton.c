
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <malloc.h>
#include <paths.h>
#include <search.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <time.h>
#include <stdint.h>

#include <support/support.h>
#include <support/check.h>
#include <support/xsignal.h>
#include <support/temp_file.h>

/* TEST_FUNCTION is no longer used. */
static int
legacy_test_function (int argc __attribute__ ((unused)),
		      char **argv __attribute__ ((unused)))
{
#ifdef TEST_FUNCTION
  return TEST_FUNCTION;
# undef TEST_FUNCTION
#else
  return do_test (argc, argv);
#endif
}
#define TEST_FUNCTION_ARGV legacy_test_function

/* PREPARE is a function name in the new skeleton.  */
#ifdef PREPARE
static void
legacy_prepare_function  (int argc __attribute__ ((unused)),
			  char **argv __attribute__ ((unused)))
{
  PREPARE (argc, argv);
}
# undef PREPARE
# define PREPARE legacy_prepare_function
#endif

/* CLEANUP_HANDLER is a function name in the new skeleton.  */
#ifdef CLEANUP_HANDLER
static void
legacy_cleanup_handler_function  (void)
{
  CLEANUP_HANDLER;
}
# undef CLEANUP_HANDLER
# define CLEANUP_HANDLER legacy_cleanup_handler_function
#endif

/* CMDLINE_PROCESS is a function name in the new skeleton.  */
#ifdef CMDLINE_PROCESS
static void
legacy_cmdline_process_function (int c)
{
  switch (c)
    {
      CMDLINE_PROCESS
    }
}
# undef CMDLINE_PROCESS
# define CMDLINE_PROCESS legacy_cmdline_process_function
#endif

/* Include the new test-skeleton.  */
#include <support/test-driver.c>

/* The following functionality is only available if <pthread.h> was
   included before this file.  */
#ifdef _PTHREAD_H
# include <support/xthread.h>
#endif	/* _PTHREAD_H */


#ifndef __QUARK_TEST_H
#define __QUARK_TEST_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "time/time_testcase.h"

__BEGIN_DECLS

typedef struct __st_test_case{
    int (*func)(void) ;
    const char * name ;
} test_case_t ;

#define TEST_CASE(func) {func , #func} 

static test_case_t __test_cases__[] = {
    TEST_CASE(do_test_mktime) ,

    {NULL , NULL}
} ;

__END_DECLS

#endif /** __QUARK_TEST_H */

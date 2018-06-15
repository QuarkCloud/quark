
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

int main(int argc , char * argv[])
{
    int idx = 0 ;
    while(true)
    {
        test_case_t *ts = __test_cases__ + idx ;
        if(ts->func == NULL)
            break ;
        ++idx ;


        ::printf("**************begin to test %s************** \n" , ts->name) ;
        int result = ts->func() ;
        if(result != 0)
        {
            printf("************** failed to test %s , result = %d ***\n" , ts->name , result) ;
            break ;
        }
        ::printf("**************succeed to test %s************** \n" , ts->name) ;
    }
    return 0 ;
}


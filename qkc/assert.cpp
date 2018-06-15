
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void __assert_fail (const char *assertion, const char *file, unsigned int line,   const char *function)
{
    char str[2048] = {'\0'} ;
    size_t slen = ::sprintf("Assertion '%s' file '%s' line %d function %s .\n" , assertion , file , line , function) ;
    ::write(STDERR_FILENO , str , slen + 1);

    abort ();
}


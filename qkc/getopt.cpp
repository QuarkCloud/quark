
#include <getopt.h>

int	opterr = 1;		/* if error message should be printed */
int	optind = 1;		/* index into parent argv vector */
int	optopt = '?';		/* character checked for validity */
int	optreset;		/* reset getopt */
char    *optarg;		/* argument associated with option */

int getopt (int argc , char *const *argv, const char *shortopts) 
{
    return 0 ;
}

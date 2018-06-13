
#ifndef __QKC_MATH_H
#define __QKC_MATH_H 1

#include <quark_compile.h>

#ifdef	__cplusplus
extern "C" {
#endif

QUARK_LINKAGE int     abs(int x);
QUARK_LINKAGE long    labs(long x);

QUARK_LINKAGE double  acos(double x);
QUARK_LINKAGE double  asin(double x);
QUARK_LINKAGE double  atan(double x);
QUARK_LINKAGE double  atan2(double y, double x);

QUARK_LINKAGE double  cos(double x);
QUARK_LINKAGE double  cosh(double x);
QUARK_LINKAGE double  exp(double x);
QUARK_LINKAGE double  fabs(double x);
QUARK_LINKAGE double  fmod(double x, double y);
QUARK_LINKAGE double  log(double x);
QUARK_LINKAGE double  log10(double x);
QUARK_LINKAGE double  pow(double x, double y);
QUARK_LINKAGE double  sin(double x);
QUARK_LINKAGE double  sinh(double x);
QUARK_LINKAGE double  tan(double x);
QUARK_LINKAGE double  tanh(double x);
QUARK_LINKAGE double  sqrt(double x);
QUARK_LINKAGE double  atof(const char *str);
QUARK_LINKAGE double  ceil(double x);
QUARK_LINKAGE double  floor(double x);
QUARK_LINKAGE double  frexp(double x, int * y);

#ifdef	__cplusplus
}
#endif

#endif /**__QKC_MATH_H*/


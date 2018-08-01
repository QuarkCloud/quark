
#ifndef __QKC_SCHED_H
#define __QKC_SCHED_H 1

#include <quark_compile.h>
#include <sys/types.h>

#define SCHED_OTHER    0
#define SCHED_FIFO     1
#define SCHED_RR       2

#define SCHED_MIN      SCHED_OTHER 
#define SCHED_MAX      SCHED_RR


struct sched_param {
  int sched_priority;           /* Process execution scheduling priority */
};

#define CPU_SETSIZE (sizeof(size_t)*8)

typedef union
{ char     cpuset[CPU_SETSIZE/8];
  size_t  _align;
} cpu_set_t;


__BEGIN_DECLS

QKCAPI int sched_yield (void);

QKCAPI int sched_get_priority_min (int policy);

QKCAPI int sched_get_priority_max (int policy);

QKCAPI int sched_setscheduler (pid_t pid, int policy);

QKCAPI int sched_setaffinity (pid_t pid, size_t cpusetsize, cpu_set_t *set);

QKCAPI int sched_getaffinity (pid_t pid, size_t cpusetsize, cpu_set_t *set);

QKCAPI int _sched_affinitycpucount (const cpu_set_t *set);

QKCAPI void _sched_affinitycpuzero (cpu_set_t *pset);

QKCAPI void _sched_affinitycpuset (int cpu, cpu_set_t *pset);

QKCAPI void _sched_affinitycpuclr (int cpu, cpu_set_t *pset);

QKCAPI int _sched_affinitycpuisset (int cpu, const cpu_set_t *pset);

QKCAPI void _sched_affinitycpuand(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2);

QKCAPI void _sched_affinitycpuor(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2);

QKCAPI void _sched_affinitycpuxor(cpu_set_t *pdestset, const cpu_set_t *psrcset1, const cpu_set_t *psrcset2);

QKCAPI int _sched_affinitycpuequal (const cpu_set_t *pset1, const cpu_set_t *pset2);


/**
    2018-06-06 int sched_getcpu(void); 新增函数，返回当前线程所运行的cpu标号
*/
QKCAPI int sched_getcpu();

__END_DECLS


#define CPU_COUNT(setptr) (_sched_affinitycpucount(setptr))

#define CPU_ZERO(setptr) (_sched_affinitycpuzero(setptr))

#define CPU_SET(cpu, setptr) (_sched_affinitycpuset((cpu),(setptr)))

#define CPU_CLR(cpu, setptr) (_sched_affinitycpuclr((cpu),(setptr)))

#define CPU_ISSET(cpu, setptr) (_sched_affinitycpuisset((cpu),(setptr)))

#define CPU_AND(destsetptr, srcset1ptr, srcset2ptr) (_sched_affinitycpuand((destsetptr),(srcset1ptr),(srcset2ptr)))

#define CPU_OR(destsetptr, srcset1ptr, srcset2ptr) (_sched_affinitycpuor((destsetptr),(srcset1ptr),(srcset2ptr)))

#define CPU_XOR(destsetptr, srcset1ptr, srcset2ptr) (_sched_affinitycpuxor((destsetptr),(srcset1ptr),(srcset2ptr)))

#define CPU_EQUAL(set1ptr, set2ptr) (_sched_affinitycpuequal((set1ptr),(set2ptr)))

typedef union
{
    char cpuset[CPU_SETSIZE/8];
    size_t _cpuset;
} _sched_cpu_set_vector_;


#endif /* __QKC_SCHED_H */

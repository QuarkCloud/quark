
#include "iocp_item.h"

bool iocp_ovlp_lock(iocp_ovlp_t * ovlp)
{
    return (::InterlockedCompareExchange(&ovlp->counter , 1 , 0) == 0) ;
}

bool iocp_ovlp_unlock(iocp_ovlp_t * ovlp)
{
    return (::InterlockedCompareExchange(&ovlp->counter , 0 , 1) == 1) ;
}

int iocp_ovlp_counter(iocp_ovlp_t * ovlp)
{
    return (int)::InterlockedCompareExchange(&ovlp->counter , 0 , 0) ;
}

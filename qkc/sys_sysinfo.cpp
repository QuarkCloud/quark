
#include <sys/sysinfo.h>
#include <windows.h>
#include <errno.h>
#include <stdlib.h>
#include <wintf/wcap.h>

int sysinfo (struct sysinfo * info)
{
    unsigned long long uptime = 0ULL, totalram = 0ULL, freeram = 0ULL, totalswap = 0ULL, freeswap = 0ULL;
    MEMORYSTATUSEX memory_status;
    PSYSTEM_PAGEFILE_INFORMATION spi = NULL;
    ULONG sizeof_spi = 512;
    PSYSTEM_TIMEOFDAY_INFORMATION stodi = NULL;
    const ULONG sizeof_stodi = sizeof (SYSTEM_TIMEOFDAY_INFORMATION);
    NTSTATUS status = STATUS_SUCCESS;

    stodi = (PSYSTEM_TIMEOFDAY_INFORMATION) malloc (sizeof_stodi);
    status = NtQuerySystemInformation (SystemTimeOfDayInformation, (PVOID) stodi, sizeof_stodi, NULL);
    if (status == STATUS_SUCCESS)
        uptime = (stodi->CurrentTime.QuadPart - stodi->BootTime.QuadPart)  / 10000000ULL;
    //else
    //    printf ("NtQuerySystemInformation(SystemTimeOfDayInformation), status %y", status);

    if(stodi)
        free (stodi);

    memory_status.dwLength = sizeof (MEMORYSTATUSEX);
    GlobalMemoryStatusEx (&memory_status);
    totalram = memory_status.ullTotalPhys / GetPageSize ();
    freeram = memory_status.ullAvailPhys / GetPageSize ();

    spi = (PSYSTEM_PAGEFILE_INFORMATION) malloc (sizeof_spi);
    if (spi)
    {
        status = NtQuerySystemInformation (SystemPagefileInformation, (PVOID) spi,  sizeof_spi, &sizeof_spi);
        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            free (spi);
            spi = (PSYSTEM_PAGEFILE_INFORMATION) malloc (sizeof_spi);
            if (spi)
                status = NtQuerySystemInformation (SystemPagefileInformation, (PVOID) spi, sizeof_spi,  &sizeof_spi);
        }
    }
    if (!spi || status != STATUS_SUCCESS)
    {
        //printf ("NtQuerySystemInformation(SystemPagefileInformation), status %y", status);
        totalswap = (memory_status.ullTotalPageFile - memory_status.ullTotalPhys)   / GetPageSize ();
        freeswap = (memory_status.ullAvailPageFile - memory_status.ullTotalPhys)   / GetPageSize ();
    }
    else
    {
        PSYSTEM_PAGEFILE_INFORMATION spp = spi;
        do
        {
            totalswap += spp->CurrentSize;
            freeswap += spp->CurrentSize - spp->TotalUsed;
        } while (spp->NextEntryOffset && (spp = (PSYSTEM_PAGEFILE_INFORMATION) ((char *) spp + spp->NextEntryOffset)));
    }

    if (spi)
        free (spi);

    info->uptime = (long) uptime;
    info->totalram = (unsigned long) totalram;
    info->freeram = (unsigned long) freeram;
    info->totalswap = (unsigned long) totalswap;
    info->freeswap = (unsigned long) freeswap;
    info->procs = 0;
    info->mem_unit = (unsigned int) GetPageSize ();

    /* FIXME: unsupported */
    info->loads[0] = 0UL;
    info->loads[1] = 0UL;
    info->loads[2] = 0UL;
    info->sharedram = 0UL;
    info->bufferram = 0UL;
    info->totalhigh = 0UL;
    info->freehigh = 0UL;

    return 0;
}

int get_nprocs_conf (void)
{
    return 0 ;
}

int get_nprocs (void)
{
    return 0 ;
}

long int get_phys_pages (void)
{
    return 0 ;
}

long int get_avphys_pages (void)
{
    return 0 ;
}


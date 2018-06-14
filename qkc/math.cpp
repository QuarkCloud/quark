
#include <math.h>
#include <stdint.h>


double round(double x) 
{
    int32_t msw, exponent_less_1023;
    uint32_t lsw , *words = (uint32_t *)(&x);

    msw = words[1] ;
    lsw = words[0] ;

    exponent_less_1023 = ((msw & 0x7ff00000) >> 20) - 1023;

    if (exponent_less_1023 < 20)
    {
        if (exponent_less_1023 < 0)
        {
            msw &= 0x80000000;
            if (exponent_less_1023 == -1)
                msw |= (1023 << 20);
            lsw = 0;
        }
        else
        {
            uint32_t exponent_mask = 0x000fffff >> exponent_less_1023;
            if ((msw & exponent_mask) == 0 && lsw == 0)
                return x;

            msw += 0x00080000 >> exponent_less_1023;
            msw &= ~exponent_mask;
            lsw = 0;
        }
    }
    else if (exponent_less_1023 > 51)
    {
        if (exponent_less_1023 == 1024)
            return x + x;
        else
            return x;
    }
    else
    {
        uint32_t exponent_mask = 0xffffffff >> (exponent_less_1023 - 20);
        uint32_t tmp;

        if ((lsw & exponent_mask) == 0)
            return x;

        tmp = lsw + (1 << (51 - exponent_less_1023));
        if (tmp < lsw)
            msw += 1;
        lsw = tmp;

        lsw &= ~exponent_mask;
    }

    words[1] = msw ;
    words[0] = lsw ;

    return x;
}


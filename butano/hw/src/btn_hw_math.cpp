#include "../include/btn_hw_math.h"

#include "tonc.h"

extern "C"
{
    unsigned isqrt32(unsigned x) BTN_CODE_IWRAM;
}

namespace btn::hw
{

int bios_division(int numerator, int denominator)
{
    return Div(numerator, denominator);
}

int bios_division(int numerator, int denominator, int& remainder)
{
    int result;

    asm volatile(
        "mov   r0, %[numerator]   \n\t"
        "mov   r1, %[denominator]   \n\t"
        #if defined(__thumb__)
            "swi   6        \n\t" // Put 6 here for Thumb C Compiler mode.
        #else
            "swi   6<<16    \n\t" // Put 0x60000 there for ARM C Compiler mode.
        #endif
        "mov   %[result], r0\n\t"
        "mov   %[remainder], r1\n\t"
        : [result] "=l" (result), [remainder] "=l" (remainder)
        : [numerator] "l" (numerator), [denominator] "l" (denominator)
        : "r0","r1","r2","r3"
    );

    return result;
}

int bios_remainder(int numerator, int denominator)
{
    return Mod(numerator, denominator);
}

int sqrt(int value)
{
    return int(isqrt32(unsigned(value)));
}

int bios_sqrt(int value)
{
    return int(Sqrt(unsigned(value)));
}

int lut_sin(int s16_angle)
{
    return lu_sin(unsigned(s16_angle));
}

int lut_cos(int s16_angle)
{
    return lu_cos(unsigned(s16_angle));
}

}

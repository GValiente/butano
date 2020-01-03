#include "../include/btn_hw_math.h"

#include "tonc.h"
#include "btn_assert.h"

extern "C"
{
    unsigned isqrt32(unsigned x) BTN_CODE_IWRAM;
}

namespace btn::hw
{

int bios_division(int numerator, int denominator)
{
    BTN_ASSERT(denominator, "Division by 0: ", numerator);

    return Div(numerator, denominator);
}

int bios_division(int numerator, int denominator, int& remainder)
{
    BTN_ASSERT(denominator, "Division by 0: ", numerator);

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
    BTN_ASSERT(denominator, "Remainder of division by 0: ", numerator);

    return Mod(numerator, denominator);
}

int sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return int(isqrt32(unsigned(value)));
}

int bios_sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return int(Sqrt(unsigned(value)));
}

int lut_sin(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return lu_sin(unsigned(s16_angle));
}

int lut_cos(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return lu_cos(unsigned(s16_angle));
}

}

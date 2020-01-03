#include "btn_math_bios_division.h"

#include "btn_assert.h"
#include "../hw/include/btn_hw_math.h"

namespace btn
{

int bios_division(int numerator, int denominator)
{
    BTN_ASSERT(denominator, "Division by 0: ", numerator);

    return hw::bios_division(numerator, denominator);
}

int bios_division(int numerator, int denominator, int& remainder)
{
    BTN_ASSERT(denominator, "Division by 0: ", numerator);

    return hw::bios_division(numerator, denominator, remainder);
}

int bios_remainder(int numerator, int denominator)
{
    BTN_ASSERT(denominator, "Remainder of division by 0: ", numerator);

    return hw::bios_remainder(numerator, denominator);
}

}

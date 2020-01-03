#include "btn_math.h"

#include "btn_fixed.h"
#include "../hw/include/btn_hw_math.h"

namespace btn
{

int sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return hw::sqrt(value);
}

int bios_sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return hw::bios_sqrt(value);
}

fixed degrees_sin(fixed degrees_angle)
{
    BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
               "Angle must be in the range [0, 360]: ", degrees_angle.integer());

    constexpr rule_of_three_aproximation rule_of_three(65536, 360);
    fixed s16_angle = rule_of_three.calculate(degrees_angle);
    int sin = lut_sin(s16_angle.integer());
    return fixed(sin) / 4096;
}

int lut_sin(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return hw::lut_sin(s16_angle);
}

fixed degrees_cos(fixed degrees_angle)
{
    BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
               "Angle must be in the range [0, 360]: ", degrees_angle.integer());

    constexpr rule_of_three_aproximation rule_of_three(65536, 360);
    fixed s16_angle = rule_of_three.calculate(degrees_angle);
    int cos = lut_cos(s16_angle.integer());
    return fixed(cos) / 4096;
}

int lut_cos(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return hw::lut_cos(s16_angle);
}

}

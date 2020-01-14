#include "btn_math.h"

#include "btn_fixed.h"
#include "btn_rule_of_three_approximation.h"
#include "../hw/include/btn_hw_math.h"

namespace btn
{

int sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return hw::sqrt(value);
}

fixed degrees_sin(fixed degrees_angle)
{
    BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
               "Angle must be in the range [0, 360]: ", degrees_angle);

    constexpr rule_of_three_approximation rule_of_three(360, 65536);
    fixed s16_angle = rule_of_three.calculate(degrees_angle);
    return fixed::create(hw::lut_sin(int(s16_angle.unsigned_integer())));
}

fixed lut_sin(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return fixed::create(hw::lut_sin(s16_angle));
}

fixed degrees_cos(fixed degrees_angle)
{
    BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
               "Angle must be in the range [0, 360]: ", degrees_angle);

    constexpr rule_of_three_approximation rule_of_three(360, 65536);
    fixed s16_angle = rule_of_three.calculate(degrees_angle);
    return fixed::create(hw::lut_cos(int(s16_angle.unsigned_integer())));
}

fixed lut_cos(int s16_angle)
{
    BTN_ASSERT(s16_angle >= 0 && s16_angle <= 65536, "Angle must be in the range [0, 65536]: ", s16_angle);

    return fixed::create(hw::lut_cos(s16_angle));
}

}

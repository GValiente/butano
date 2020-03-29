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

    constexpr rule_of_three_approximation rule_of_three(360, 512);
    fixed lut_angle = rule_of_three.calculate(degrees_angle);
    return fixed::create(hw::lut_sin(int(lut_angle.unsigned_integer())));
}

fixed lut_sin(int lut_angle)
{
    BTN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

    return fixed::create(hw::lut_sin(lut_angle));
}

fixed degrees_cos(fixed degrees_angle)
{
    BTN_ASSERT(degrees_angle >= 0 && degrees_angle <= 360,
               "Angle must be in the range [0, 360]: ", degrees_angle);

    constexpr rule_of_three_approximation rule_of_three(360, 512);
    fixed lut_angle = rule_of_three.calculate(degrees_angle);
    return fixed::create(hw::lut_cos(int(lut_angle.unsigned_integer())));
}

fixed lut_cos(int lut_angle)
{
    BTN_ASSERT(lut_angle >= 0 && lut_angle <= 512, "Angle must be in the range [0, 512]: ", lut_angle);

    return fixed::create(hw::lut_cos(lut_angle));
}

}

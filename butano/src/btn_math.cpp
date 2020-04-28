#include "btn_math.h"

#include "../hw/include/btn_hw_math.h"

namespace btn
{

int sqrt(int value)
{
    BTN_ASSERT(value >= 0, "Value must be positive: ", value);

    return hw::sqrt(value);
}

}

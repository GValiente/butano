#include "btn_math.h"

#include "../hw/include/btn_hw_math.h"

namespace _btn
{

int sqrt_impl(int value)
{
    return btn::hw::sqrt(value);
}

}

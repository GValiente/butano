/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_timer.h"

#include "btn_fixed.h"
#include "btn_limits.h"
#include "../hw/include/btn_hw_timer.h"

namespace btn
{

timer::timer() :
    _last_ticks(hw::timer::ticks())
{
}

int timer::elapsed_ticks() const
{
    unsigned ticks = hw::timer::ticks();

    if(ticks < _last_ticks) [[unlikely]]
    {
        uint64_t overflow_result = ticks;
        overflow_result += numeric_limits<unsigned>::max();
        overflow_result -= _last_ticks;
        return int(overflow_result);
    }

    return int(ticks - _last_ticks);
}

fixed timer::elapsed_frames() const
{
    return fixed(elapsed_ticks()) / ticks_per_frame();
}

void timer::restart()
{
    _last_ticks = hw::timer::ticks();
}

}

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_timer.h"

#include "../hw/include/bn_hw_timer.h"

namespace bn
{

timer::timer() :
    _last_ticks(hw::timer::ticks())
{
}

int timer::elapsed_ticks() const
{
    return int(hw::timer::ticks() - _last_ticks);
}

void timer::restart()
{
    _last_ticks = hw::timer::ticks();
}

int timer::elapsed_ticks_with_restart()
{
    unsigned last_ticks = _last_ticks;

    BN_BARRIER;

    unsigned ticks = hw::timer::ticks();
    _last_ticks = ticks;

    return int(ticks - last_ticks);
}

}

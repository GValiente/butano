/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TIMERS_H
#define BTN_TIMERS_H

#include "../hw/include/btn_hw_timer_constants.h"

/**
 * @brief Timers related functions.
 *
 * @ingroup other
 */
namespace btn::timers
{
    /**
     * @brief Returns the number of ticks elapsed in a screen refresh (V-Draw + V-Blank).
     */
    [[nodiscard]] constexpr int ticks_per_frame()
    {
        return hw::timers::ticks_per_frame();
    }

    /**
     * @brief Returns the number of ticks elapsed in a V-Blank period.
     */
    [[nodiscard]] constexpr int ticks_per_vblank()
    {
        return hw::timers::ticks_per_vblank();
    }
}

#endif

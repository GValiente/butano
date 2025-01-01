/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TIMERS_H
#define BN_TIMERS_H

/**
 * @file
 * bn::timers header file.
 *
 * @ingroup timer
 */

#include "../hw/include/bn_hw_timer_constants.h"

/**
 * @brief Timing related functions.
 *
 * @ingroup timer
 */
namespace bn::timers
{
    /**
     * @brief Returns the number of ticks elapsed in a screen refresh (V-Draw + V-Blank).
     */
    [[nodiscard]] constexpr int ticks_per_frame()
    {
        return hw::timers::ticks_per_frame();
    }

    /**
     * @brief Returns the number of ticks elapsed in a second.
     */
    [[nodiscard]] constexpr int ticks_per_second()
    {
        return hw::timers::ticks_per_second();
    }

    /**
     * @brief Returns the number of ticks elapsed in a V-Blank period.
     */
    [[nodiscard]] constexpr int ticks_per_vblank()
    {
        return hw::timers::ticks_per_vblank();
    }

    /**
     * @brief Returns the number of CPU clocks per timer tick.
     */
    [[nodiscard]] constexpr int cpu_clocks_per_tick()
    {
        return hw::timers::divisor();
    }
}

#endif

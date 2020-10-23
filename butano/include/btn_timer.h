/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TIMER_H
#define BTN_TIMER_H

#include "../hw/include/btn_hw_timer_constants.h"

namespace btn
{

/**
 * @brief Provides a way to calculate elapsed times.
 *
 * One timer tick is equivalent to 64 CPU clock cycles.
 *
 * @ingroup other
 */
class timer
{

public:
    /**
     * @brief Returns the number of ticks elapsed in a screen refresh (V-Draw + V-Blank).
     */
    [[nodiscard]] constexpr static int ticks_per_frame()
    {
        return timers::ticks_per_frame();
    }

    /**
     * @brief Returns the number of ticks elapsed in a V-Blank period.
     */
    [[nodiscard]] constexpr static int ticks_per_vblank()
    {
        return timers::ticks_per_vblank();
    }

    /**
     * @brief Constructor.
     */
    timer();

    /**
     * @brief Returns the number of ticks since this timer was built or restarted.
     */
    [[nodiscard]] int elapsed_ticks() const;

    /**
     * @brief Restarts the timer's internal state.
     *
     * Equivalent to:
     * @code{.cpp}
     * *this = timer();
     * @endcode
     */
    void restart();

private:
    unsigned _last_ticks;
};

}

#endif

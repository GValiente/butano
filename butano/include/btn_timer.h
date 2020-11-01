/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TIMER_H
#define BTN_TIMER_H

#include "btn_common.h"

namespace btn
{

/**
 * @brief Provides a way to calculate elapsed times.
 *
 * One timer tick is equivalent to 64 CPU clock cycles.
 *
 * @ingroup timer
 */
class timer
{

public:
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

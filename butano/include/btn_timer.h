#ifndef BTN_TIMER_H
#define BTN_TIMER_H

#include "btn_fixed_fwd.h"
#include "../hw/include/btn_hw_timer_constants.h"

namespace btn
{

class timer
{

public:
    [[nodiscard]] constexpr static int ticks_per_frame()
    {
        return timers::ticks_per_frame();
    }

    [[nodiscard]] constexpr static int ticks_per_second()
    {
        return timers::ticks_per_second();
    }

    timer();

    [[nodiscard]] int elapsed_ticks() const;

    [[nodiscard]] fixed elapsed_frames() const;

    [[nodiscard]] fixed elapsed_seconds() const;

    void restart();

private:
    unsigned _last_ticks;
};

}

#endif

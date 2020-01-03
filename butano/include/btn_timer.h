#ifndef BTN_TIMER_H
#define BTN_TIMER_H

#include "btn_fixed_fwd.h"

namespace btn
{

class timer
{

public:
    [[nodiscard]] BTN_CODE_CONST static int ticks_per_frame();

    [[nodiscard]] BTN_CODE_CONST static int ticks_per_second();

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

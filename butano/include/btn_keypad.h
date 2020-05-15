#ifndef BTN_KEYPAD_H
#define BTN_KEYPAD_H

#include "btn_common.h"

namespace btn::keypad
{
    enum class key_type
    {
        A =         0x0001,
        B =         0x0002,
        SELECT =    0x0004,
        START =     0x0008,
        RIGHT =     0x0010,
        LEFT =      0x0020,
        UP =        0x0040,
        DOWN =      0x0080,
        R =         0x0100,
        L =         0x0200
    };

    [[nodiscard]] bool down(key_type key);

    [[nodiscard]] bool up(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);
}

#endif

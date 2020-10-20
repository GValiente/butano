/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

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

    [[nodiscard]] bool held(key_type key);

    [[nodiscard]] bool pressed(key_type key);

    [[nodiscard]] bool released(key_type key);

    [[nodiscard]] bool a_held();

    [[nodiscard]] bool a_pressed();

    [[nodiscard]] bool a_released();

    [[nodiscard]] bool b_held();

    [[nodiscard]] bool b_pressed();

    [[nodiscard]] bool b_released();

    [[nodiscard]] bool select_held();

    [[nodiscard]] bool select_pressed();

    [[nodiscard]] bool select_released();

    [[nodiscard]] bool start_held();

    [[nodiscard]] bool start_pressed();

    [[nodiscard]] bool start_released();

    [[nodiscard]] bool right_held();

    [[nodiscard]] bool right_pressed();

    [[nodiscard]] bool right_released();

    [[nodiscard]] bool left_held();

    [[nodiscard]] bool left_pressed();

    [[nodiscard]] bool left_released();

    [[nodiscard]] bool up_held();

    [[nodiscard]] bool up_pressed();

    [[nodiscard]] bool up_released();

    [[nodiscard]] bool down_held();

    [[nodiscard]] bool down_pressed();

    [[nodiscard]] bool down_released();

    [[nodiscard]] bool r_held();

    [[nodiscard]] bool r_pressed();

    [[nodiscard]] bool r_released();

    [[nodiscard]] bool l_held();

    [[nodiscard]] bool l_pressed();

    [[nodiscard]] bool l_released();
}

#endif

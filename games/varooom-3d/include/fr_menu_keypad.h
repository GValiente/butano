/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MENU_KEYPAD_H
#define FR_MENU_KEYPAD_H

#include "bn_common.h"

namespace fr
{

class menu_keypad
{

public:
    [[nodiscard]] bool up() const
    {
        return _valid_key(_up_counter);
    }

    [[nodiscard]] bool down() const
    {
        return _valid_key(_down_counter);
    }

    [[nodiscard]] bool left() const
    {
        return _valid_key(_left_counter);
    }

    [[nodiscard]] bool right() const
    {
        return _valid_key(_right_counter);
    }

    void reset()
    {
        *this = menu_keypad();
    }

    void update();

private:
    int _up_counter = 0;
    int _down_counter = 0;
    int _left_counter = 0;
    int _right_counter = 0;

    [[nodiscard]] static bool _valid_key(int counter)
    {
        return counter == 1 || (counter >= 24 && counter % 12 == 0);
    }
};

}

#endif

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_menu_keypad.h"

#include "bn_keypad.h"

namespace fr
{

void menu_keypad::update()
{
    if(bn::keypad::a_held() || bn::keypad::b_held())
    {
        _up_counter = 0;
        _down_counter = 0;
        _left_counter = 0;
        _right_counter = 0;
    }
    else
    {
        if(bn::keypad::up_held())
        {
            ++_up_counter;
            _down_counter = 0;
        }
        else
        {
            _up_counter = 0;

            if(bn::keypad::down_held())
            {
                ++_down_counter;
            }
            else
            {
                _down_counter = 0;
            }
        }

        if(bn::keypad::left_held())
        {
            ++_left_counter;
            _right_counter = 0;
        }
        else
        {
            _left_counter = 0;

            if(bn::keypad::right_held())
            {
                ++_right_counter;
            }
            else
            {
                _right_counter = 0;
            }
        }
    }
}

}

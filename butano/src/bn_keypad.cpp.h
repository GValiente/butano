/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_keypad.h"

#include "bn_keypad_manager.h"

namespace bn::keypad
{

bool held(key_type key)
{
    return keypad_manager::held(key);
}

bool pressed(key_type key)
{
    return keypad_manager::pressed(key);
}

bool released(key_type key)
{
    return keypad_manager::released(key);
}

bool a_held()
{
    return keypad_manager::held(key_type::A);
}

bool a_pressed()
{
    return keypad_manager::pressed(key_type::A);
}

bool a_released()
{
    return keypad_manager::released(key_type::A);
}

bool b_held()
{
    return keypad_manager::held(key_type::B);
}

bool b_pressed()
{
    return keypad_manager::pressed(key_type::B);
}

bool b_released()
{
    return keypad_manager::released(key_type::B);
}

bool select_held()
{
    return keypad_manager::held(key_type::SELECT);
}

bool select_pressed()
{
    return keypad_manager::pressed(key_type::SELECT);
}

bool select_released()
{
    return keypad_manager::released(key_type::SELECT);
}

bool start_held()
{
    return keypad_manager::held(key_type::START);
}

bool start_pressed()
{
    return keypad_manager::pressed(key_type::START);
}

bool start_released()
{
    return keypad_manager::released(key_type::START);
}

bool right_held()
{
    return keypad_manager::held(key_type::RIGHT);
}

bool right_pressed()
{
    return keypad_manager::pressed(key_type::RIGHT);
}

bool right_released()
{
    return keypad_manager::released(key_type::RIGHT);
}

bool left_held()
{
    return keypad_manager::held(key_type::LEFT);
}

bool left_pressed()
{
    return keypad_manager::pressed(key_type::LEFT);
}

bool left_released()
{
    return keypad_manager::released(key_type::LEFT);
}

bool up_held()
{
    return keypad_manager::held(key_type::UP);
}

bool up_pressed()
{
    return keypad_manager::pressed(key_type::UP);
}

bool up_released()
{
    return keypad_manager::released(key_type::UP);
}

bool down_held()
{
    return keypad_manager::held(key_type::DOWN);
}

bool down_pressed()
{
    return keypad_manager::pressed(key_type::DOWN);
}

bool down_released()
{
    return keypad_manager::released(key_type::DOWN);
}

bool r_held()
{
    return keypad_manager::held(key_type::R);
}

bool r_pressed()
{
    return keypad_manager::pressed(key_type::R);
}

bool r_released()
{
    return keypad_manager::released(key_type::R);
}

bool l_held()
{
    return keypad_manager::held(key_type::L);
}

bool l_pressed()
{
    return keypad_manager::pressed(key_type::L);
}

bool l_released()
{
    return keypad_manager::released(key_type::L);
}

bool any_held()
{
    return keypad_manager::any_held();
}

bool any_pressed()
{
    return keypad_manager::any_pressed();
}

bool any_released()
{
    return keypad_manager::any_released();
}

}

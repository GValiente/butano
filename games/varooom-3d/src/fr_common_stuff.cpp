/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_common_stuff.h"

#include "bn_core.h"
#include "bn_keypad.h"

#include "fr_big_sprite_font.h"
#include "fr_small_fixed_sprite_font.h"
#include "fr_small_variable_sprite_font.h"

namespace fr
{

common_stuff::common_stuff() :
    big_text_generator(big_sprite_font),
    small_fixed_text_generator(small_fixed_sprite_font),
    small_variable_text_generator(small_variable_sprite_font)
{
    big_text_generator.set_bg_priority(1);
    small_fixed_text_generator.set_bg_priority(1);
    small_variable_text_generator.set_bg_priority(1);
    storage.init();
}

void common_stuff::update()
{
    bool reset_keys_held = bn::keypad::select_held() && bn::keypad::start_held() &&
            bn::keypad::b_held() && bn::keypad::a_held();

    if(_reset_ready)
    {
        if(reset_keys_held)
        {
            bn::core::reset();
        }
    }
    else
    {
        if(! reset_keys_held)
        {
            _reset_ready = true;
        }
    }
}

}

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_colors.h"
#include "bn_sprite_ptr.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palette_ptr.h"

#include "bn_regular_bg_items_gba.h"
#include "bn_sprite_items_up_button.h"
#include "bn_sprite_items_down_button.h"
#include "bn_sprite_items_left_button.h"
#include "bn_sprite_items_right_button.h"
#include "bn_sprite_items_b_button.h"
#include "bn_sprite_items_a_button.h"
#include "bn_sprite_items_l_button.h"
#include "bn_sprite_items_r_button.h"
#include "bn_sprite_items_start_button.h"
#include "bn_sprite_items_select_button.h"

namespace
{
    void update_button(bn::keypad::key_type key, bn::sprite_ptr& sprite)
    {
        bn::sprite_palette_ptr sprite_palette = sprite.palette();

        if(bn::keypad::pressed(key))
        {
            sprite_palette.set_fade(bn::colors::red, 0.5);
        }
        else if(bn::keypad::released(key))
        {
            sprite_palette.set_fade(bn::colors::orange, 0.5);
        }
        else if(bn::keypad::held(key))
        {
            sprite_palette.set_fade(bn::colors::yellow, 0.5);
        }
        else
        {
            sprite_palette.set_fade_intensity(0);
        }
    }
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr background = bn::regular_bg_items::gba.create_bg(0, 0);

    bn::sprite_ptr up_sprite = bn::sprite_items::up_button.create_sprite(-63, -47);
    bn::sprite_ptr down_sprite = bn::sprite_items::down_button.create_sprite(-72, -32);
    bn::sprite_ptr left_sprite = bn::sprite_items::left_button.create_sprite(-77, -42);
    bn::sprite_ptr right_sprite = bn::sprite_items::right_button.create_sprite(-60, -35);
    bn::sprite_ptr b_sprite = bn::sprite_items::b_button.create_sprite(62, 10);
    bn::sprite_ptr a_sprite = bn::sprite_items::a_button.create_sprite(83, 12);
    bn::sprite_ptr l_sprite = bn::sprite_items::l_button.create_sprite(-54, -69);
    bn::sprite_ptr r_sprite = bn::sprite_items::r_button.create_sprite(94, -15);
    bn::sprite_ptr start_sprite = bn::sprite_items::start_button.create_sprite(-70, -12);
    bn::sprite_ptr select_sprite = bn::sprite_items::select_button.create_sprite(-76, -2);

    while(true)
    {
        update_button(bn::keypad::key_type::UP, up_sprite);
        update_button(bn::keypad::key_type::DOWN, down_sprite);
        update_button(bn::keypad::key_type::LEFT, left_sprite);
        update_button(bn::keypad::key_type::RIGHT, right_sprite);
        update_button(bn::keypad::key_type::B, b_sprite);
        update_button(bn::keypad::key_type::A, a_sprite);
        update_button(bn::keypad::key_type::L, l_sprite);
        update_button(bn::keypad::key_type::R, r_sprite);
        update_button(bn::keypad::key_type::START, start_sprite);
        update_button(bn::keypad::key_type::SELECT, select_sprite);
        bn::core::update();
    }
}

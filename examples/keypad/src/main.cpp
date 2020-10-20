/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_fixed.h"
#include "btn_keypad.h"
#include "btn_colors.h"
#include "btn_sprite_ptr.h"
#include "btn_regular_bg_ptr.h"
#include "btn_sprite_palette_ptr.h"

#include "btn_regular_bg_items_gba.h"
#include "btn_sprite_items_up_button.h"
#include "btn_sprite_items_down_button.h"
#include "btn_sprite_items_left_button.h"
#include "btn_sprite_items_right_button.h"
#include "btn_sprite_items_b_button.h"
#include "btn_sprite_items_a_button.h"
#include "btn_sprite_items_l_button.h"
#include "btn_sprite_items_r_button.h"
#include "btn_sprite_items_start_button.h"
#include "btn_sprite_items_select_button.h"

namespace
{
    void update_button(btn::keypad::key_type key, btn::sprite_ptr& sprite)
    {
        btn::sprite_palette_ptr sprite_palette = sprite.palette();

        if(btn::keypad::pressed(key))
        {
            sprite_palette.set_fade(btn::colors::red, 0.5);
        }
        else if(btn::keypad::released(key))
        {
            sprite_palette.set_fade(btn::colors::orange, 0.5);
        }
        else if(btn::keypad::held(key))
        {
            sprite_palette.set_fade(btn::colors::yellow, 0.5);
        }
        else
        {
            sprite_palette.set_fade_intensity(0);
        }
    }
}

int main()
{
    btn::core::init();

    btn::regular_bg_ptr background = btn::regular_bg_items::gba.create_bg(0, 0);

    btn::sprite_ptr up_sprite = btn::sprite_items::up_button.create_sprite(-63, -47);
    btn::sprite_ptr down_sprite = btn::sprite_items::down_button.create_sprite(-72, -32);
    btn::sprite_ptr left_sprite = btn::sprite_items::left_button.create_sprite(-77, -42);
    btn::sprite_ptr right_sprite = btn::sprite_items::right_button.create_sprite(-60, -35);
    btn::sprite_ptr b_sprite = btn::sprite_items::b_button.create_sprite(62, 10);
    btn::sprite_ptr a_sprite = btn::sprite_items::a_button.create_sprite(83, 12);
    btn::sprite_ptr l_sprite = btn::sprite_items::l_button.create_sprite(-54, -69);
    btn::sprite_ptr r_sprite = btn::sprite_items::r_button.create_sprite(94, -15);
    btn::sprite_ptr start_sprite = btn::sprite_items::start_button.create_sprite(-70, -12);
    btn::sprite_ptr select_sprite = btn::sprite_items::select_button.create_sprite(-76, -2);

    while(true)
    {
        update_button(btn::keypad::key_type::UP, up_sprite);
        update_button(btn::keypad::key_type::DOWN, down_sprite);
        update_button(btn::keypad::key_type::LEFT, left_sprite);
        update_button(btn::keypad::key_type::RIGHT, right_sprite);
        update_button(btn::keypad::key_type::B, b_sprite);
        update_button(btn::keypad::key_type::A, a_sprite);
        update_button(btn::keypad::key_type::L, l_sprite);
        update_button(btn::keypad::key_type::R, r_sprite);
        update_button(btn::keypad::key_type::START, start_sprite);
        update_button(btn::keypad::key_type::SELECT, select_sprite);
        btn::core::update();
    }
}

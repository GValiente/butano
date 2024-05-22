/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h"
#include "bn_green_swap_actions.h"
#include "bn_green_swap_hbe_ptr.h"
#include "bn_sprite_text_generator.h"

#include "bn_regular_bg_items_village.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void green_swap_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: toggle green swap",
            "",
            "START: go to next scene",
        };

        common::info info("Green swap", info_text_lines, text_generator);

        bn::green_swap::set_enabled(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                bn::green_swap::set_enabled(! bn::green_swap::enabled());
            }

            info.update();
            bn::core::update();
        }

        bn::green_swap::set_enabled(false);
    }

    void green_swap_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Green swap actions", info_text_lines, text_generator);

        bn::green_swap_toggle_action action(60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::green_swap::set_enabled(false);
    }

    void green_swap_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Green swap H-Blank effect", info_text_lines, text_generator);

        bn::array<bool, bn::display::height()> green_swap_states;
        bn::green_swap_hbe_ptr green_swap_states_hbe = bn::green_swap_hbe_ptr::create(green_swap_states);

        int green_swap_lines = 64;
        int first_green_swap_line_index = 0;
        int green_swap_line_index_inc = 1;

        while(! bn::keypad::start_pressed())
        {
            if(first_green_swap_line_index == 0)
            {
                green_swap_line_index_inc = 1;
            }
            else if(first_green_swap_line_index == bn::display::height() - green_swap_lines)
            {
                green_swap_line_index_inc = -1;
            }

            first_green_swap_line_index += green_swap_line_index_inc;
            green_swap_states.fill(false);

            for(int index = 0; index < green_swap_lines; ++index)
            {
                green_swap_states[first_green_swap_line_index + index] = true;
            }

            green_swap_states_hbe.reload_states_ref();

            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    while(true)
    {
        green_swap_scene(text_generator);
        bn::core::update();

        green_swap_actions_scene(text_generator);
        bn::core::update();

        green_swap_hbe_scene(text_generator);
        bn::core::update();
    }
}

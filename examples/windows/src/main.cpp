/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_regular_bg_ptr.h"
#include "bn_rect_window_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_rect_window_boundaries_hbe_ptr.h"

#include "bn_sprite_items_circle.h"
#include "bn_regular_bg_items_land.h"
#include "bn_regular_bg_items_clouds.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void window_visibility_scene(bn::regular_bg_ptr& clouds_bg, bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: toggle window visibility",
            "",
            "START: go to next scene",
        };

        common::info info("Window", info_text_lines, text_generator);

        bn::rect_window internal_window = bn::rect_window::internal();
        internal_window.set_boundaries(-48, -96, 48, 96);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                internal_window.set_visible(! internal_window.visible());
            }

            clouds_bg.set_position(clouds_bg.x() + 0.5, clouds_bg.y() + 0.5);
            info.update();
            bn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
        internal_window.set_visible(true);
        clouds_bg.set_position(0, 0);
    }

    void window_boundaries_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move window boundaries",
            "",
            "START: go to next scene",
        };

        common::info info("Window boundaries", info_text_lines, text_generator);

        bn::rect_window internal_window = bn::rect_window::internal();
        bn::fixed_point internal_window_position;

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                internal_window_position.set_x(internal_window_position.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                internal_window_position.set_x(internal_window_position.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                internal_window_position.set_y(internal_window_position.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                internal_window_position.set_y(internal_window_position.y() + 1);
            }

            internal_window.set_boundaries(internal_window_position.y() - 48, internal_window_position.x() - 48,
                                           internal_window_position.y() + 48, internal_window_position.x() + 48);
            info.update();
            bn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
    }

    void sprite_window_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move circle sprite",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite window", info_text_lines, text_generator);

        bn::sprite_ptr circle_sprite = bn::sprite_items::circle.create_sprite(0, 0);
        circle_sprite.set_window_enabled(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                circle_sprite.set_x(circle_sprite.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                circle_sprite.set_x(circle_sprite.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                circle_sprite.set_y(circle_sprite.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                circle_sprite.set_y(circle_sprite.y() + 1);
            }

            info.update();
            bn::core::update();
        }
    }

    void window_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Window actions", info_text_lines, text_generator);

        bn::rect_window internal_window = bn::rect_window::internal();
        internal_window.set_boundaries(-48, -96, 48, 96);

        bn::rect_window_move_top_loop_action move_top_action(internal_window, 120, 0);
        bn::rect_window_move_left_loop_action move_left_action(internal_window, 120, 0);
        bn::rect_window_move_bottom_loop_action move_bottom_action(internal_window, 120, 0);
        bn::rect_window_move_right_loop_action move_right_action(internal_window, 120, 0);

        while(! bn::keypad::start_pressed())
        {
            move_top_action.update();
            move_left_action.update();
            move_bottom_action.update();
            move_right_action.update();
            info.update();
            bn::core::update();
        }

        internal_window.set_boundaries(0, 0, 0, 0);
    }

    void window_hbe_scene(bn::regular_bg_ptr& clouds_bg, bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Window H-Blank effect", info_text_lines, text_generator);

        bn::rect_window internal_window = bn::rect_window::internal();
        int amplitude = 56;
        internal_window.set_top(-amplitude);
        internal_window.set_bottom(amplitude);

        bn::array<bn::pair<bn::fixed, bn::fixed>, bn::display::height()> horizontal_boundaries;
        bn::rect_window_boundaries_hbe_ptr horizontal_boundaries_hbe =
                bn::rect_window_boundaries_hbe_ptr::create_horizontal(internal_window, horizontal_boundaries);
        bn::fixed base_degrees_angle;

        while(! bn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            bn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0; index < amplitude; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 8;
                bn::fixed stretch = amplitude - index;
                bn::pair<bn::fixed, bn::fixed> left_right(desp - stretch, desp + stretch);
                horizontal_boundaries[(bn::display::height() / 2) + index] = left_right;
                horizontal_boundaries[(bn::display::height() / 2) - index - 1] = left_right;
            }

            horizontal_boundaries_hbe.reload_deltas_ref();
            clouds_bg.set_position(clouds_bg.x() + 0.5, clouds_bg.y() + 0.5);
            info.update();
            bn::core::update();
        }

        internal_window.set_top(0);
        internal_window.set_bottom(0);
        clouds_bg.set_position(0, 0);
    }
}

int main()
{
    bn::core::init();

    bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);
    bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);

    bn::window outside_window = bn::window::outside();
    outside_window.set_show_bg(clouds_bg, false);

    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    while(true)
    {
        window_visibility_scene(clouds_bg, text_generator);
        bn::core::update();

        window_boundaries_scene(text_generator);
        bn::core::update();

        sprite_window_scene(text_generator);
        bn::core::update();

        window_actions_scene(text_generator);
        bn::core::update();

        window_hbe_scene(clouds_bg, text_generator);
        bn::core::update();
    }
}

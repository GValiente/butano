/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_builder.h"
#include "bn_sprites_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_position_hbe_ptr.h"
#include "bn_sprite_first_attributes_hbe_ptr.h"
#include "bn_sprite_third_attributes_hbe_ptr.h"
#include "bn_sprite_affine_second_attributes.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_sprite_affine_second_attributes_hbe_ptr.h"
#include "bn_sprite_regular_second_attributes_hbe_ptr.h"

#include "bn_sprite_items_ninja.h"
#include "bn_sprite_items_caveman.h"
#include "bn_sprite_items_red_sprite.h"
#include "bn_sprite_items_blue_sprite.h"
#include "bn_sprite_items_green_sprite.h"
#include "bn_sprite_items_yellow_sprite.h"
#include "bn_regular_bg_items_red_bg.h"
#include "bn_regular_bg_items_blue_bg.h"
#include "bn_regular_bg_items_green_bg.h"
#include "bn_regular_bg_items_yellow_bg.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void sprites_visibility_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: hide/show sprite",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites visibility", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                red_sprite.set_visible(! red_sprite.visible());
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_visibility_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites visibility actions", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(0, 0);
        bn::sprite_visible_toggle_action action(green_sprite, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprite_layer_visibility_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: hide/show sprite layer",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite layer visibility", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                bn::sprites::set_visible(! bn::sprites::visible());
            }

            info.update();
            bn::core::update();
        }

        bn::sprites::set_visible(true);
    }

    void sprite_layer_visibility_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite layer visibility actions", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(0, 0);
        bn::sprites_visible_toggle_action action(60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::sprites::set_visible(true);
    }

    void sprites_position_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move sprite",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites position", info_text_lines, text_generator);

        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                blue_sprite.set_x(blue_sprite.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                blue_sprite.set_x(blue_sprite.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                blue_sprite.set_y(blue_sprite.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                blue_sprite.set_y(blue_sprite.y() + 1);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_position_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites position actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(-amplitude, -amplitude);
        bn::sprite_move_loop_action action(yellow_sprite, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_position_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites position H-Blank effect", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);

        bn::array<bn::fixed, bn::display::height()> horizontal_deltas;

        bn::sprite_position_hbe_ptr horizontal_position_hbe =
                bn::sprite_position_hbe_ptr::create_horizontal(red_sprite, horizontal_deltas);

        bn::fixed base_degrees_angle;

        while(! bn::keypad::start_pressed())
        {
            base_degrees_angle += 4;

            if(base_degrees_angle >= 360)
            {
                base_degrees_angle -= 360;
            }

            bn::fixed degrees_angle = base_degrees_angle;

            for(int index = 0, limit = bn::display::height() / 2; index < limit; ++index)
            {
                degrees_angle += 16;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 6;
                horizontal_deltas[(bn::display::height() / 2) + index] = desp;
                horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_position_hbe.reload_deltas_ref();
            info.update();
            bn::core::update();
        }
    }

    void sprites_animation_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: change sprite's direction",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites animation", info_text_lines, text_generator);

        bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(8));
            }
            else if(bn::keypad::right_held())
            {
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(12));
            }

            if(bn::keypad::up_held())
            {
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(4));
            }
            else if(bn::keypad::down_held())
            {
                ninja_sprite.set_tiles(bn::sprite_items::ninja.tiles_item().create_tiles(0));
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_animation_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: change sprite's direction",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites animation actions", info_text_lines, text_generator);

        bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
        bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(
                    ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
            }
            else if(bn::keypad::right_pressed())
            {
                action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
            }

            if(bn::keypad::up_pressed())
            {
                action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
            }
            else if(bn::keypad::down_pressed())
            {
                action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
            }

            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_rotation_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease rotation angle",
            "RIGHT: increase rotation angle",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites rotation", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(0, 0);
        green_sprite.set_rotation_angle(45);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed rotation_angle = green_sprite.rotation_angle();

            if(bn::keypad::left_held())
            {
                green_sprite.set_rotation_angle(bn::max(rotation_angle - 1, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                green_sprite.set_rotation_angle(bn::min(rotation_angle + 1, bn::fixed(360)));
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_rotation_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites rotation actions", info_text_lines, text_generator);

        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(0, 0);
        bn::sprite_rotate_loop_action action(blue_sprite, 180, 360);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_scale_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal scale",
            "RIGHT: increase horizontal scale",
            "DOWN: decrease vertical scale",
            "UP: increase vertical scale",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites scale", info_text_lines, text_generator);

        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(0, 0);
        yellow_sprite.set_horizontal_scale(1.5);
        yellow_sprite.set_vertical_scale(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_scale = yellow_sprite.horizontal_scale();
            bn::fixed vertical_scale = yellow_sprite.vertical_scale();

            if(bn::keypad::left_held())
            {
                yellow_sprite.set_horizontal_scale(bn::max(horizontal_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::right_held())
            {
                yellow_sprite.set_horizontal_scale(bn::min(horizontal_scale + 0.01, bn::fixed(2)));
            }

            if(bn::keypad::down_held())
            {
                yellow_sprite.set_vertical_scale(bn::max(vertical_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::up_held())
            {
                yellow_sprite.set_vertical_scale(bn::min(vertical_scale + 0.01, bn::fixed(2)));
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_scale_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites scale actions", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(0.01);

        bn::sprite_scale_loop_action action(red_sprite, 120, 2);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_shear_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal shear",
            "RIGHT: increase horizontal shear",
            "DOWN: decrease vertical shear",
            "UP: increase vertical shear",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites shear", info_text_lines, text_generator);

        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(0, 0);
        yellow_sprite.set_horizontal_shear(0.5);
        yellow_sprite.set_vertical_shear(-0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_shear = yellow_sprite.horizontal_shear();
            bn::fixed vertical_shear = yellow_sprite.vertical_shear();

            if(bn::keypad::left_held())
            {
                yellow_sprite.set_horizontal_shear(horizontal_shear - 0.01);
            }
            else if(bn::keypad::right_held())
            {
                yellow_sprite.set_horizontal_shear(horizontal_shear + 0.01);
            }

            if(bn::keypad::down_held())
            {
                yellow_sprite.set_vertical_shear(vertical_shear - 0.01);
            }
            else if(bn::keypad::up_held())
            {
                yellow_sprite.set_vertical_shear(vertical_shear + 0.01);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_shear_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites shear actions", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_shear(-0.5);

        bn::sprite_shear_loop_action action(red_sprite, 120, 0.5);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_flip_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: disable horizontal flip",
            "RIGHT: enable horizontal flip",
            "DOWN: disable vertical flip",
            "UP: enable vertical flip",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites flip", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(0, 0);
        green_sprite.set_horizontal_flip(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                green_sprite.set_horizontal_flip(false);
            }
            else if(bn::keypad::right_pressed())
            {
                green_sprite.set_horizontal_flip(true);
            }

            if(bn::keypad::down_pressed())
            {
                green_sprite.set_vertical_flip(false);
            }
            else if(bn::keypad::up_pressed())
            {
                green_sprite.set_vertical_flip(true);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_flip_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites flip actions", info_text_lines, text_generator);

        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(0, 0);
        bn::sprite_horizontal_flip_toggle_action action(blue_sprite, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void sprites_double_size_mode_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: toggle double size mode",
            "B: enable auto double size mode",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites double size mode", info_text_lines, text_generator);

        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(0, 0);
        yellow_sprite.set_scale(2);
        yellow_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                if(yellow_sprite.double_size_mode() == bn::sprite_double_size_mode::DISABLED)
                {
                    yellow_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);
                }
                else
                {
                    yellow_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);
                }
            }

            if(bn::keypad::b_pressed())
            {
                yellow_sprite.set_double_size_mode(bn::sprite_double_size_mode::AUTO);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_bg_priority_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease caveman BG priority",
            "RIGHT: increase caveman BG priority",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites BG priority", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green_bg.create_bg(-56, -56);
        green_bg.set_priority(0);

        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue_bg.create_bg(-56, 56);
        blue_bg.set_priority(1);

        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow_bg.create_bg(56, -56);
        yellow_bg.set_priority(2);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red_bg.create_bg(56, 56);
        red_bg.set_priority(3);

        bn::sprite_ptr caveman_sprite = bn::sprite_items::caveman.create_sprite(0, 0);
        caveman_sprite.set_bg_priority(2);
        caveman_sprite.set_z_order(1);

        while(! bn::keypad::start_pressed())
        {
            int priority = caveman_sprite.bg_priority();

            if(bn::keypad::left_pressed())
            {
                caveman_sprite.set_bg_priority(bn::max(priority - 1, 0));
            }
            else if(bn::keypad::right_pressed())
            {
                caveman_sprite.set_bg_priority(bn::min(priority + 1, 3));
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_z_order_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease red sprite Z order",
            "RIGHT: increase red sprite Z order",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites Z order", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        green_sprite.set_z_order(0);

        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        blue_sprite.set_z_order(1);

        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        yellow_sprite.set_z_order(2);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_z_order(2);

        while(! bn::keypad::start_pressed())
        {
            int z_order = red_sprite.z_order();

            if(bn::keypad::left_pressed())
            {
                red_sprite.set_z_order(bn::max(z_order - 1, 0));
            }
            else if(bn::keypad::right_pressed())
            {
                red_sprite.set_z_order(bn::min(z_order + 1, 3));
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_put_above_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: put blue sprite above red sprite",
            "A: put yellow sprite above red sprite",
            "L: put green sprite above red sprite",
            "R: put red sprite above all",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites put above", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                blue_sprite.put_above();
            }

            if(bn::keypad::a_pressed())
            {
                yellow_sprite.put_above();
            }

            if(bn::keypad::l_pressed())
            {
                green_sprite.put_above();
            }

            if(bn::keypad::r_pressed())
            {
                red_sprite.put_above();
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_first_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        common::info info("First attributes", info_text_lines, text_generator);

        bn::sprites_mosaic::set_stretch(0.1);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        bn::sprite_first_attributes red_sprite_attributes = red_sprite.first_attributes();
        green_sprite.set_mosaic_enabled(true);
        yellow_sprite.set_mosaic_enabled(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_sprite.set_first_attributes(blue_sprite.first_attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_sprite.set_first_attributes(yellow_sprite.first_attributes());
            }

            if(bn::keypad::l_pressed())
            {
                red_sprite.set_first_attributes(green_sprite.first_attributes());
            }

            if(bn::keypad::r_pressed())
            {
                red_sprite.set_first_attributes(red_sprite_attributes);
            }

            info.update();
            bn::core::update();
        }

        bn::sprites_mosaic::set_stretch(0);
    }

    void sprites_first_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("First attributes H-Blank effect", info_text_lines, text_generator);

        bn::sprites_mosaic::set_stretch(0.2);
        bn::blending::set_transparency_alpha(0.6);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        bn::sprite_first_attributes red_attributes = red_sprite.first_attributes();
        bn::sprite_first_attributes alt_attributes = red_attributes;
        alt_attributes.set_mosaic_enabled(true);
        alt_attributes.set_blending_enabled(true);

        bn::vector<bn::sprite_first_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        bn::span<const bn::sprite_first_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::sprite_first_attributes_hbe_ptr attributes_hbe =
                bn::sprite_first_attributes_hbe_ptr::create(red_sprite, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::sprites_mosaic::set_stretch(0);
        bn::blending::set_transparency_alpha(1);
    }

    void sprites_regular_second_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        common::info info("Regular second attributes", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        bn::sprite_regular_second_attributes red_sprite_attributes = red_sprite.regular_second_attributes();
        green_sprite.set_horizontal_flip(true);
        yellow_sprite.set_vertical_flip(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_sprite.set_regular_second_attributes(blue_sprite.regular_second_attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_sprite.set_regular_second_attributes(yellow_sprite.regular_second_attributes());
            }

            if(bn::keypad::l_pressed())
            {
                red_sprite.set_regular_second_attributes(green_sprite.regular_second_attributes());
            }

            if(bn::keypad::r_pressed())
            {
                red_sprite.set_regular_second_attributes(red_sprite_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_regular_second_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Reg. second attribs. H-Blank effect", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        bn::sprite_regular_second_attributes red_attributes = red_sprite.regular_second_attributes();
        bn::sprite_regular_second_attributes alt_attributes = red_attributes;
        alt_attributes.set_horizontal_flip(true);
        alt_attributes.set_vertical_flip(true);

        bn::vector<bn::sprite_regular_second_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        bn::span<const bn::sprite_regular_second_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::sprite_regular_second_attributes_hbe_ptr attributes_hbe =
                bn::sprite_regular_second_attributes_hbe_ptr::create(red_sprite, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void sprites_affine_second_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        common::info info("Affine second attributes", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        green_sprite.set_rotation_angle(45);
        blue_sprite.set_rotation_angle(200);
        yellow_sprite.set_scale(1.5);
        red_sprite.set_scale(2);

        bn::sprite_affine_second_attributes red_sprite_attributes = red_sprite.affine_second_attributes();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_sprite.set_affine_second_attributes(blue_sprite.affine_second_attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_sprite.set_affine_second_attributes(yellow_sprite.affine_second_attributes());
            }

            if(bn::keypad::l_pressed())
            {
                red_sprite.set_affine_second_attributes(green_sprite.affine_second_attributes());
            }

            if(bn::keypad::r_pressed())
            {
                red_sprite.set_affine_second_attributes(red_sprite_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_affine_second_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Affine second attribs. H-Blank effect", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        bn::sprite_affine_second_attributes red_attributes = red_sprite.affine_second_attributes();
        bn::sprite_affine_second_attributes alt_attributes = red_attributes;
        bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();
        affine_mat.set_rotation_angle(45);
        affine_mat.set_scale(1.5);
        alt_attributes.set_affine_mat(affine_mat);

        bn::vector<bn::sprite_affine_second_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(alt_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        bn::span<const bn::sprite_affine_second_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::sprite_affine_second_attributes_hbe_ptr attributes_hbe =
                bn::sprite_affine_second_attributes_hbe_ptr::create(red_sprite, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void sprites_third_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        common::info info("Third attributes", info_text_lines, text_generator);

        bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32);
        bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32);
        bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, 32);
        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        bn::sprite_third_attributes red_sprite_attributes = red_sprite.third_attributes();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_sprite.set_third_attributes(blue_sprite.third_attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_sprite.set_third_attributes(yellow_sprite.third_attributes());
            }

            if(bn::keypad::l_pressed())
            {
                red_sprite.set_third_attributes(green_sprite.third_attributes());
            }

            if(bn::keypad::r_pressed())
            {
                red_sprite.set_third_attributes(red_sprite_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void sprites_third_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Third attributes H-Blank effect", info_text_lines, text_generator);

        bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(0, 0);
        red_sprite.set_scale(2);

        bn::sprite_palette_ptr green_palette = bn::sprite_items::green_sprite.palette_item().create_palette();
        bn::sprite_third_attributes red_attributes = red_sprite.third_attributes();
        bn::sprite_third_attributes green_attributes = red_attributes;
        green_attributes.set_palette(green_palette);

        bn::vector<bn::sprite_third_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(green_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        bn::span<const bn::sprite_third_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::sprite_third_attributes_hbe_ptr attributes_hbe =
                bn::sprite_third_attributes_hbe_ptr::create(red_sprite, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void sprite_builder_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite builder", info_text_lines, text_generator);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red_bg.create_bg(0, 0);
        bn::sprites_mosaic::set_stretch(0.1);
        bn::blending::set_transparency_alpha(0.6);

        bn::sprite_builder builder(bn::sprite_items::blue_sprite);
        builder.set_position(48, 24);
        builder.set_scale(2);
        builder.set_rotation_angle(45);
        builder.set_horizontal_flip(true);
        builder.set_mosaic_enabled(true);
        builder.set_blending_enabled(true);

        bn::sprite_ptr blue_sprite = builder.build();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::sprites_mosaic::set_stretch(0);
        bn::blending::set_transparency_alpha(1);
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        sprites_visibility_scene(text_generator);
        bn::core::update();

        sprites_visibility_actions_scene(text_generator);
        bn::core::update();

        sprite_layer_visibility_scene(text_generator);
        bn::core::update();

        sprite_layer_visibility_actions_scene(text_generator);
        bn::core::update();

        sprites_position_scene(text_generator);
        bn::core::update();

        sprites_position_actions_scene(text_generator);
        bn::core::update();

        sprites_position_hbe_scene(text_generator);
        bn::core::update();

        sprites_animation_scene(text_generator);
        bn::core::update();

        sprites_animation_actions_scene(text_generator);
        bn::core::update();

        sprites_rotation_scene(text_generator);
        bn::core::update();

        sprites_rotation_actions_scene(text_generator);
        bn::core::update();

        sprites_scale_scene(text_generator);
        bn::core::update();

        sprites_scale_actions_scene(text_generator);
        bn::core::update();

        sprites_shear_scene(text_generator);
        bn::core::update();

        sprites_shear_actions_scene(text_generator);
        bn::core::update();

        sprites_flip_scene(text_generator);
        bn::core::update();

        sprites_flip_actions_scene(text_generator);
        bn::core::update();

        sprites_double_size_mode_scene(text_generator);
        bn::core::update();

        sprites_bg_priority_scene(text_generator);
        bn::core::update();

        sprites_z_order_scene(text_generator);
        bn::core::update();

        sprites_put_above_scene(text_generator);
        bn::core::update();

        sprites_first_attributes_scene(text_generator);
        bn::core::update();

        sprites_first_attributes_hbe_scene(text_generator);
        bn::core::update();

        sprites_regular_second_attributes_scene(text_generator);
        bn::core::update();

        sprites_regular_second_attributes_hbe_scene(text_generator);
        bn::core::update();

        sprites_affine_second_attributes_scene(text_generator);
        bn::core::update();

        sprites_affine_second_attributes_hbe_scene(text_generator);
        bn::core::update();

        sprites_third_attributes_scene(text_generator);
        bn::core::update();

        sprites_third_attributes_hbe_scene(text_generator);
        bn::core::update();

        sprite_builder_scene(text_generator);
        bn::core::update();
    }
}

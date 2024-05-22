/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"

#include "bn_sprite_items_turtle.h"
#include "bn_regular_bg_items_red.h"
#include "bn_regular_bg_items_blue.h"
#include "bn_regular_bg_items_green.h"
#include "bn_regular_bg_items_dragon.h"
#include "bn_regular_bg_items_yellow.h"
#include "bn_bg_palette_items_dragon_alt.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void regular_bgs_visibility_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "A: hide/show BG",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs visibility", info_text_lines, text_generator);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                red_bg.set_visible(! red_bg.visible());
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_visibility_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BGs visibility actions", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green.create_bg(0, 0);
        bn::regular_bg_visible_toggle_action action(green_bg, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_position_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move BG",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs position", info_text_lines, text_generator);

        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                blue_bg.set_x(blue_bg.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                blue_bg.set_x(blue_bg.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                blue_bg.set_y(blue_bg.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                blue_bg.set_y(blue_bg.y() + 1);
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_position_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BGs position actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow.create_bg(-amplitude, -amplitude);
        bn::regular_bg_move_loop_action action(yellow_bg, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_position_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BGs position H-Blank effect", info_text_lines, text_generator);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);

        bn::array<bn::fixed, bn::display::height()> horizontal_deltas;
        bn::regular_bg_position_hbe_ptr horizontal_deltas_hbe =
                bn::regular_bg_position_hbe_ptr::create_horizontal(red_bg, horizontal_deltas);

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

                bn::fixed desp = bn::degrees_lut_sin(degrees_angle) * 8;
                horizontal_deltas[(bn::display::height() / 2) + index] = desp;
                horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_deltas_hbe.reload_deltas_ref();
            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_animation_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: change dragon's pose",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs animation", info_text_lines, text_generator);

        bn::regular_bg_ptr dragon_bg = bn::regular_bg_items::dragon.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                dragon_bg.set_map(bn::regular_bg_items::dragon.map_item());
            }
            else if(bn::keypad::right_held())
            {
                dragon_bg.set_map(bn::regular_bg_items::dragon.map_item(), 1);
            }
            else if(bn::keypad::up_held())
            {
                dragon_bg.set_map(bn::regular_bg_items::dragon.map_item(), 2);
            }
            else if(bn::keypad::down_held())
            {
                dragon_bg.set_map(bn::regular_bg_items::dragon.map_item(), 3);
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_animation_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BGs animation actions", info_text_lines, text_generator);

        bn::regular_bg_ptr dragon_bg = bn::regular_bg_items::dragon.create_bg(0, 0);
        bn::regular_bg_cached_animate_action<5> action = bn::create_regular_bg_cached_animate_action_forever(
                    dragon_bg, 32, bn::regular_bg_items::dragon.map_item(), 0, 1, 2, 0, 3);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_priority_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease red BG priority",
            "RIGHT: increase red BG priority",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs priority", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green.create_bg(-64, -64);
        green_bg.set_priority(0);

        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_priority(1);

        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow.create_bg(64, 64);
        yellow_bg.set_priority(2);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
        red_bg.set_priority(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! bn::keypad::start_pressed())
        {
            int priority = red_bg.priority();

            if(bn::keypad::left_pressed())
            {
                red_bg.set_priority(bn::max(priority - 1, 0));
            }
            else if(bn::keypad::right_pressed())
            {
                red_bg.set_priority(bn::min(priority + 1, 3));
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_z_order_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease red BG Z order",
            "RIGHT: increase red BG Z order",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs Z order", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green.create_bg(-64, -64);
        green_bg.set_z_order(0);

        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_z_order(1);

        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow.create_bg(64, 64);
        yellow_bg.set_z_order(2);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
        red_bg.set_z_order(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! bn::keypad::start_pressed())
        {
            int z_order = red_bg.z_order();

            if(bn::keypad::left_pressed())
            {
                red_bg.set_z_order(bn::max(z_order - 1, 0));
            }
            else if(bn::keypad::right_pressed())
            {
                red_bg.set_z_order(bn::min(z_order + 1, 3));
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_put_above_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: put blue BG above red BG",
            "A: put yellow BG above red BG",
            "L: put green BG above red BG",
            "R: put red BG above all",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs put above", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green.create_bg(-64, -64);
        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue.create_bg(-64, 64);
        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow.create_bg(64, 64);
        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                blue_bg.put_above();
            }

            if(bn::keypad::a_pressed())
            {
                yellow_bg.put_above();
            }

            if(bn::keypad::l_pressed())
            {
                green_bg.put_above();
            }

            if(bn::keypad::r_pressed())
            {
                red_bg.put_above();
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: copy yellow attributes to red",
            "L: copy green attributes to red",
            "R: restore red attributes",
            "",
            "START: go to next scene",
        };

        common::info info("Regular BGs attributes", info_text_lines, text_generator);

        bn::regular_bg_ptr green_bg = bn::regular_bg_items::green.create_bg(-64, -64);
        bn::regular_bg_ptr blue_bg = bn::regular_bg_items::blue.create_bg(-64, 64);
        bn::regular_bg_ptr yellow_bg = bn::regular_bg_items::yellow.create_bg(64, 64);
        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
        bn::regular_bg_attributes red_bg_attributes = red_bg.attributes();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_bg.set_attributes(blue_bg.attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_bg.set_attributes(yellow_bg.attributes());
            }

            if(bn::keypad::l_pressed())
            {
                red_bg.set_attributes(green_bg.attributes());
            }

            if(bn::keypad::r_pressed())
            {
                red_bg.set_attributes(red_bg_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void regular_bgs_attributes_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BGs attributes H-Blank effect", info_text_lines, text_generator);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
        bn::regular_bg_map_ptr green_map = bn::regular_bg_items::green.create_map();
        bn::regular_bg_attributes red_attributes = red_bg.attributes();
        bn::regular_bg_attributes green_attributes = red_attributes;
        green_attributes.set_map(green_map);

        bn::vector<bn::regular_bg_attributes, bn::display::height()> attributes;

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

        bn::span<const bn::regular_bg_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::regular_bg_attributes_hbe_ptr attributes_hbe =
                bn::regular_bg_attributes_hbe_ptr::create(red_bg, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void regular_bg_builder_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Regular BG builder", info_text_lines, text_generator);

        bn::regular_bg_ptr red_bg = bn::regular_bg_items::red.create_bg(0, 0);
        bn::bgs_mosaic::set_stretch(0.2);
        bn::blending::set_transparency_alpha(0.6);

        bn::regular_bg_builder builder(bn::regular_bg_items::blue);
        builder.set_position(30, 10);
        builder.set_mosaic_enabled(true);
        builder.set_blending_enabled(true);

        bn::regular_bg_ptr blue_bg = builder.build();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
        bn::blending::set_transparency_alpha(1);
    }

    void regular_bg_different_palette(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Same BGs, different palette", info_text_lines, text_generator);

        constexpr bn::regular_bg_item dragon_alt_item(
                    bn::regular_bg_items::dragon.tiles_item(), bn::bg_palette_items::dragon_alt,
                    bn::regular_bg_items::dragon.map_item());

        bn::regular_bg_ptr dragon_bg = bn::regular_bg_items::dragon.create_bg(-48, 0);
        bn::regular_bg_ptr dragon_alt_bg = dragon_alt_item.create_bg(48, 0);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        regular_bgs_visibility_scene(text_generator);
        bn::core::update();

        regular_bgs_visibility_actions_scene(text_generator);
        bn::core::update();

        regular_bgs_position_scene(text_generator);
        bn::core::update();

        regular_bgs_position_actions_scene(text_generator);
        bn::core::update();

        regular_bgs_position_hbe_scene(text_generator);
        bn::core::update();

        regular_bgs_animation_scene(text_generator);
        bn::core::update();

        regular_bgs_animation_actions_scene(text_generator);
        bn::core::update();

        regular_bgs_priority_scene(text_generator);
        bn::core::update();

        regular_bgs_z_order_scene(text_generator);
        bn::core::update();

        regular_bgs_put_above_scene(text_generator);
        bn::core::update();

        regular_bgs_attributes_scene(text_generator);
        bn::core::update();

        regular_bgs_attributes_hbe_scene(text_generator);
        bn::core::update();

        regular_bg_builder_scene(text_generator);
        bn::core::update();

        regular_bg_different_palette(text_generator);
        bn::core::update();
    }
}

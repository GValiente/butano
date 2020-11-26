/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_affine_bg_actions.h"
#include "bn_affine_bg_ptr.h"
#include "bn_affine_bg_builder.h"
#include "bn_affine_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_position_hblank_effect_ptr.h"
#include "bn_affine_bg_attributes_hblank_effect_ptr.h"

#include "bn_sprite_items_turtle.h"
#include "bn_affine_bg_items_red.h"
#include "bn_affine_bg_items_blue.h"
#include "bn_affine_bg_items_red_small.h"
#include "bn_affine_bg_items_blue_small.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void affine_bgs_visibility_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "A: hide/show BG",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs visibility", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);

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

    void affine_bgs_visibility_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BGs visibility actions", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(0, 0);
        bn::affine_bg_visible_toggle_action action(blue_bg, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_position_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "PAD: move BG",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs position", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                red_bg.set_x(red_bg.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                red_bg.set_x(red_bg.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                red_bg.set_y(red_bg.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                red_bg.set_y(red_bg.y() + 1);
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_position_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BGs position actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(0, 0);
        bn::affine_bg_move_loop_action action(blue_bg, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_position_hblank_effect_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BGs position H-Blank effect", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);

        bn::array<bn::fixed, bn::display::height()> horizontal_deltas;

        bn::affine_bg_position_hblank_effect_ptr horizontal_hblank_effect =
                bn::affine_bg_position_hblank_effect_ptr::create_horizontal(red_bg, horizontal_deltas);

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

                bn::fixed desp = bn::degrees_sin(degrees_angle) * 8;
                horizontal_deltas[(bn::display::height() / 2) + index] = desp;
                horizontal_deltas[(bn::display::height() / 2) - index - 1] = desp;
            }

            horizontal_hblank_effect.reload_deltas_ref();
            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_wrapping_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "A: enable/disable wrapping",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs wrapping", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red_small.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                red_bg.set_wrapping_enabled(! red_bg.wrapping_enabled());
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_wrapping_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BGs wrapping actions", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue_small.create_bg(0, 0);
        bn::affine_bg_wrapping_toggle_action action(blue_bg, 60);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_priority_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "LEFT: decrease red BG priority",
            "RIGHT: increase red BG priority",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs priority", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_priority(1);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);
        red_bg.set_priority(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                red_bg.set_priority(1);
            }
            else if(bn::keypad::right_pressed())
            {
                red_bg.set_priority(2);
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_z_order_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "LEFT: decrease red BG Z order",
            "RIGHT: increase red BG Z order",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs Z order", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(-64, 64);
        blue_bg.set_z_order(1);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);
        red_bg.set_z_order(2);

        bn::sprite_ptr turtle_sprite = bn::sprite_items::turtle.create_sprite(64, -32);
        turtle_sprite.set_bg_priority(2);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                red_bg.set_z_order(1);
            }
            else if(bn::keypad::right_pressed())
            {
                red_bg.set_z_order(2);
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_put_above_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "B: put blue BG above red BG",
            "A: put red BG above blue BG",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs put above", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(-64, 64);
        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                blue_bg.put_above();
            }

            if(bn::keypad::a_pressed())
            {
                red_bg.put_above();
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_attributes_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "B: copy blue attributes to red",
            "A: restore red attributes",
            "",
            "START: go to next scene",
        };

        info info("Affine BGs attributes", info_text_lines, text_generator);

        bn::affine_bg_ptr blue_bg = bn::affine_bg_items::blue.create_bg(-64, 64);
        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);
        bn::affine_bg_attributes red_bg_attributes = red_bg.attributes();

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::b_pressed())
            {
                red_bg.set_attributes(blue_bg.attributes());
            }

            if(bn::keypad::a_pressed())
            {
                red_bg.set_attributes(red_bg_attributes);
            }

            info.update();
            bn::core::update();
        }
    }

    void affine_bgs_attributes_hblank_effect_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BGs attributes H-Blank effect", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);
        bn::affine_bg_map_ptr blue_map = bn::affine_bg_items::blue.create_map();
        bn::affine_bg_attributes red_attributes = red_bg.attributes();
        bn::affine_bg_attributes blue_attributes = red_attributes;
        blue_attributes.set_map(blue_map);

        bn::vector<bn::affine_bg_attributes, bn::display::height()> attributes;

        for(int index = 0; index < bn::display::height(); ++index)
        {
            if(index % 2)
            {
                attributes.push_back(blue_attributes);
            }
            else
            {
                attributes.push_back(red_attributes);
            }
        }

        bn::span<const bn::affine_bg_attributes> attributes_ref(attributes.data(), attributes.size());
        bn::affine_bg_attributes_hblank_effect_ptr hblank_effect =
                bn::affine_bg_attributes_hblank_effect_ptr::create(red_bg, attributes_ref);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void affine_bg_builder_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr const bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Affine BG builder", info_text_lines, text_generator);

        bn::affine_bg_ptr red_bg = bn::affine_bg_items::red.create_bg(0, 0);
        bn::bgs_mosaic::set_stretch(0.2);
        bn::blending::set_transparency_alpha(0.6);

        bn::affine_bg_builder builder(bn::affine_bg_items::blue);
        builder.set_position(30, 10);
        builder.set_mosaic_enabled(true);
        builder.set_blending_enabled(true);

        bn::affine_bg_ptr blue_bg = builder.build();

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
        bn::blending::set_transparency_alpha(1);
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    while(true)
    {
        affine_bgs_visibility_scene(text_generator);
        bn::core::update();

        affine_bgs_visibility_actions_scene(text_generator);
        bn::core::update();

        affine_bgs_position_scene(text_generator);
        bn::core::update();

        affine_bgs_position_actions_scene(text_generator);
        bn::core::update();

        affine_bgs_position_hblank_effect_scene(text_generator);
        bn::core::update();

        affine_bgs_wrapping_scene(text_generator);
        bn::core::update();

        affine_bgs_wrapping_actions_scene(text_generator);
        bn::core::update();

        affine_bgs_priority_scene(text_generator);
        bn::core::update();

        affine_bgs_z_order_scene(text_generator);
        bn::core::update();

        affine_bgs_put_above_scene(text_generator);
        bn::core::update();

        affine_bgs_attributes_scene(text_generator);
        bn::core::update();

        affine_bgs_attributes_hblank_effect_scene(text_generator);
        bn::core::update();

        affine_bg_builder_scene(text_generator);
        bn::core::update();
    }
}

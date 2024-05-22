/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_ptr.h"
#include "bn_mosaic_attributes.h"
#include "bn_bgs_mosaic_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprites_mosaic_actions.h"
#include "bn_mosaic_attributes_hbe_ptr.h"

#include "bn_sprite_items_blonde.h"
#include "bn_regular_bg_items_land.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void sprites_mosaic_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal stretch",
            "RIGHT: increase horizontal stretch",
            "DOWN: decrease vertical stretch",
            "UP: increase vertical stretch",
            "",
            "START: go to next scene",
        };

        common::info info("Sprites mosaic", info_text_lines, text_generator);

        bn::sprite_ptr blonde_sprite = bn::sprite_items::blonde.create_sprite(0, 0);
        blonde_sprite.set_mosaic_enabled(true);

        bn::sprites_mosaic::set_stretch(0.1);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_stretch = bn::sprites_mosaic::horizontal_stretch();
            bn::fixed vertical_stretch = bn::sprites_mosaic::vertical_stretch();

            if(bn::keypad::left_held())
            {
                bn::sprites_mosaic::set_horizontal_stretch(bn::max(horizontal_stretch - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::sprites_mosaic::set_horizontal_stretch(bn::min(horizontal_stretch + 0.01, bn::fixed(1)));
            }

            if(bn::keypad::down_held())
            {
                bn::sprites_mosaic::set_vertical_stretch(bn::max(vertical_stretch - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::up_held())
            {
                bn::sprites_mosaic::set_vertical_stretch(bn::min(vertical_stretch + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::sprites_mosaic::set_stretch(0);
    }

    void bgs_mosaic_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal stretch",
            "RIGHT: increase horizontal stretch",
            "DOWN: decrease vertical stretch",
            "UP: increase vertical stretch",
            "",
            "START: go to next scene",
        };

        common::info info("Backgrounds mosaic", info_text_lines, text_generator);

        bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);
        land_bg.set_mosaic_enabled(true);

        bn::bgs_mosaic::set_stretch(0.1);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_stretch = bn::bgs_mosaic::horizontal_stretch();
            bn::fixed vertical_stretch = bn::bgs_mosaic::vertical_stretch();

            if(bn::keypad::left_held())
            {
                bn::bgs_mosaic::set_horizontal_stretch(bn::max(horizontal_stretch - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::bgs_mosaic::set_horizontal_stretch(bn::min(horizontal_stretch + 0.01, bn::fixed(1)));
            }

            if(bn::keypad::down_held())
            {
                bn::bgs_mosaic::set_vertical_stretch(bn::max(vertical_stretch - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::up_held())
            {
                bn::bgs_mosaic::set_vertical_stretch(bn::min(vertical_stretch + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
    }

    void sprites_mosaic_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprites mosaic actions", info_text_lines, text_generator);

        bn::sprite_ptr blonde_sprite = bn::sprite_items::blonde.create_sprite(0, 0);
        blonde_sprite.set_mosaic_enabled(true);

        bn::sprites_mosaic_stretch_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::sprites_mosaic::set_stretch(0);
    }

    void bgs_mosaic_actions_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Backgrounds mosaic actions", info_text_lines, text_generator);

        bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);
        land_bg.set_mosaic_enabled(true);

        bn::bgs_mosaic_stretch_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::bgs_mosaic::set_stretch(0);
    }

    void mosaic_hbe_scene(bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Mosaic H-Blank effect", info_text_lines, text_generator);

        bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);
        land_bg.set_mosaic_enabled(true);

        bn::sprite_ptr blonde_sprite = bn::sprite_items::blonde.create_sprite(0, 0);
        blonde_sprite.set_mosaic_enabled(true);

        bn::array<bn::mosaic_attributes, bn::display::height()> mosaic_attributes;
        bn::fixed max_strech(0.5);

        for(int index = 0, amplitude = 32; index < amplitude; ++index)
        {
            bn::fixed stretch = max_strech - ((index * max_strech) / amplitude);
            mosaic_attributes[(bn::display::height() / 2) + index].set_horizontal_stretch(stretch);
            mosaic_attributes[(bn::display::height() / 2) - index - 1].set_horizontal_stretch(stretch);
        }

        bn::mosaic_attributes_hbe_ptr mosaic_attributes_hbe = bn::mosaic_attributes_hbe_ptr::create(mosaic_attributes);

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
        sprites_mosaic_scene(text_generator);
        bn::core::update();

        bgs_mosaic_scene(text_generator);
        bn::core::update();

        sprites_mosaic_actions_scene(text_generator);
        bn::core::update();

        bgs_mosaic_actions_scene(text_generator);
        bn::core::update();

        mosaic_hbe_scene(text_generator);
        bn::core::update();
    }
}

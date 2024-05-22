/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h"
#include "bn_blending_actions.h"
#include "bn_blending_fade_alpha.h"
#include "bn_sprite_text_generator.h"
#include "bn_blending_fade_alpha_hbe_ptr.h"
#include "bn_blending_transparency_attributes.h"
#include "bn_blending_transparency_attributes_hbe_ptr.h"

#include "bn_regular_bg_items_tree.h"
#include "bn_sprite_items_dinosaur.h"
#include "bn_regular_bg_items_mountain.h"
#include "bn_sprite_items_variable_8x16_font_yellow.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void transparency_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                            bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease transparency alpha",
            "RIGHT: increase transparency alpha",
            "",
            "START: go to next scene",
        };

        common::info info("Transparency", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending::set_transparency_alpha(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed transparency_alpha = bn::blending::transparency_alpha();

            if(bn::keypad::left_held())
            {
                bn::blending::set_transparency_alpha(bn::max(transparency_alpha - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::blending::set_transparency_alpha(bn::min(transparency_alpha + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::blending::set_transparency_alpha(1);
    }

    void transparency_bottom_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                   bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move dinosaur",
            "",
            "",
            "START: go to next scene",
        };

        common::info info("Transparency bottom", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(false);

        bn::regular_bg_ptr tree_bg = bn::regular_bg_items::tree.create_bg(0, 0);
        tree_bg.set_priority(2);
        tree_bg.set_blending_enabled(true);

        bn::blending::set_transparency_alpha(0.75);
        mountain_bg.set_blending_bottom_enabled(false);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                dinosaur_sprite.set_x(bn::max(dinosaur_sprite.x() - 1, bn::fixed(-60)));
            }
            else if(bn::keypad::right_held())
            {
                dinosaur_sprite.set_x(bn::min(dinosaur_sprite.x() + 1, bn::fixed(60)));
            }

            if(bn::keypad::up_held())
            {
                dinosaur_sprite.set_y(bn::max(dinosaur_sprite.y() - 1, bn::fixed(-30)));
            }
            else if(bn::keypad::down_held())
            {
                dinosaur_sprite.set_y(bn::min(dinosaur_sprite.y() + 1, bn::fixed(30)));
            }

            info.update();
            bn::core::update();
        }

        bn::blending::set_transparency_alpha(1);
        mountain_bg.set_blending_bottom_enabled(true);
        dinosaur_sprite.set_position(0, 0);
    }

    void transparency_actions_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                      bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Transparency actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending_transparency_alpha_loop_action action(120, 0);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::blending::set_transparency_alpha(1);
    }

    void intensity_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                         bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease intensity alpha",
            "RIGHT: increase intensity alpha",
            "",
            "START: go to next scene",
        };

        common::info info("Intensity", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending::set_intensity_alpha(0.5);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed intensity_alpha = bn::blending::intensity_alpha();

            if(bn::keypad::left_held())
            {
                bn::blending::set_intensity_alpha(bn::max(intensity_alpha - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::blending::set_intensity_alpha(bn::min(intensity_alpha + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::blending::set_intensity_alpha(0);
    }

    void intensity_actions_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                 bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("intensity actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending_intensity_alpha_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::blending::set_intensity_alpha(0);
    }

    void transparency_weights_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                    bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "UP: decrease top weight",
            "DOWN: increase top weight",
            "LEFT: decrease bottom weight",
            "RIGHT: increase bottom weight",
            "",
            "START: go to next scene",
        };

        common::info info("Transparency weights", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending::set_transparency_weights(0.1, 0.6);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed transparency_top_weight = bn::blending::transparency_top_weight();
            bn::fixed transparency_bottom_weight = bn::blending::transparency_bottom_weight();

            if(bn::keypad::down_held())
            {
                bn::blending::set_transparency_top_weight(bn::max(transparency_top_weight - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::up_held())
            {
                bn::blending::set_transparency_top_weight(bn::min(transparency_top_weight + 0.01, bn::fixed(1)));
            }

            if(bn::keypad::left_held())
            {
                bn::blending::set_transparency_bottom_weight(bn::max(transparency_bottom_weight - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::blending::set_transparency_bottom_weight(bn::min(transparency_bottom_weight + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::blending::set_transparency_weights(1, 0);
    }

    void transparency_weights_actions_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                            bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Transparency weights actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending::set_transparency_weights(0, 0.5);

        bn::blending_transparency_top_weight_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::blending::set_transparency_weights(1, 0);
    }

    void transparency_hbe_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                                bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Transparency H-Blank effect", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(false);
        dinosaur_sprite.set_blending_enabled(true);

        bn::array<bn::blending_transparency_attributes, bn::display::height()> transparency_attributes;

        for(int index = 0, amplitude = 32; index < amplitude; ++index)
        {
            bn::fixed transparency_alpha = bn::fixed(index) / amplitude;
            transparency_attributes[(bn::display::height() / 2) + index].set_transparency_alpha(transparency_alpha);
            transparency_attributes[(bn::display::height() / 2) - index - 1].set_transparency_alpha(transparency_alpha);
        }

        bn::blending_transparency_attributes_hbe_ptr transparency_attributes_hbe =
                bn::blending_transparency_attributes_hbe_ptr::create(transparency_attributes);

        while(! bn::keypad::start_pressed())
        {
            info.update();
            bn::core::update();
        }
    }

    void fade_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                    bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease fade alpha",
            "RIGHT: increase fade alpha",
            "A: toggle fade color",
            "",
            "START: go to next scene",
        };

        common::info info("Fade", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending::set_fade_alpha(0.5);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::a_pressed())
            {
                if(bn::blending::fade_color() == bn::blending::fade_color_type::BLACK)
                {
                    bn::blending::set_white_fade_color();
                }
                else
                {
                    bn::blending::set_black_fade_color();
                }
            }

            bn::fixed fade_alpha = bn::blending::fade_alpha();

            if(bn::keypad::left_held())
            {
                bn::blending::set_fade_alpha(bn::max(fade_alpha - 0.01, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                bn::blending::set_fade_alpha(bn::min(fade_alpha + 0.01, bn::fixed(1)));
            }

            info.update();
            bn::core::update();
        }

        bn::blending::set_black_fade_color();
        bn::blending::set_fade_alpha(0);
    }

    void fade_actions_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                            bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Fade actions", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        bn::blending_fade_alpha_loop_action action(120, 1);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        bn::blending::set_fade_alpha(0);
    }

    void fade_hbe_scene(bn::regular_bg_ptr& mountain_bg, bn::sprite_ptr& dinosaur_sprite,
                        bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Fade H-Blank effect", info_text_lines, text_generator);

        mountain_bg.set_blending_enabled(true);
        dinosaur_sprite.set_blending_enabled(true);

        bn::array<bn::blending_fade_alpha, bn::display::height()> fade_alphas;

        for(int index = 0, amplitude = bn::display::height() / 2; index < amplitude; ++index)
        {
            bn::fixed fade_alpha = bn::fixed(index) / amplitude;
            fade_alphas[(bn::display::height() / 2) + index].set_value(fade_alpha);
            fade_alphas[(bn::display::height() / 2) - index - 1].set_value(fade_alpha);
        }

        bn::blending_fade_alpha_hbe_ptr fade_alphas_hbe = bn::blending_fade_alpha_hbe_ptr::create(fade_alphas);

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

    bn::regular_bg_ptr mountain_bg = bn::regular_bg_items::mountain.create_bg(0, 0);
    bn::sprite_ptr dinosaur_sprite = bn::sprite_items::dinosaur.create_sprite(0, 0);

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    text_generator.set_palette_item(bn::sprite_items::variable_8x16_font_yellow.palette_item());

    while(true)
    {
        transparency_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        transparency_bottom_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        transparency_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        intensity_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        intensity_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        transparency_weights_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        transparency_weights_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        transparency_hbe_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        fade_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        fade_actions_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();

        fade_hbe_scene(mountain_bg, dinosaur_sprite, text_generator);
        bn::core::update();
    }
}

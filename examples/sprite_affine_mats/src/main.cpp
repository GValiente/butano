/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_bg_palettes.h"
#include "bn_affine_mat_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_double_size_mode.h"
#include "bn_sprite_affine_mat_actions.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"

#include "bn_sprite_items_red_sprite.h"
#include "bn_sprite_items_blue_sprite.h"
#include "bn_sprite_items_green_sprite.h"
#include "bn_sprite_items_yellow_sprite.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void sprite_affine_mats_rotation_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                           bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease rotation angle",
            "RIGHT: increase rotation angle",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite affine mats rotation", info_text_lines, text_generator);

        affine_mat.set_rotation_angle(45);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed rotation_angle = affine_mat.rotation_angle();

            if(bn::keypad::left_held())
            {
                affine_mat.set_rotation_angle(bn::max(rotation_angle - 1, bn::fixed(0)));
            }
            else if(bn::keypad::right_held())
            {
                affine_mat.set_rotation_angle(bn::min(rotation_angle + 1, bn::fixed(360)));
            }

            info.update();
            bn::core::update();
        }

        affine_mat.set_rotation_angle(0);
    }

    void sprite_affine_mats_rotation_actions_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                                   bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite affine mats rotation actions", info_text_lines, text_generator);

        bn::sprite_affine_mat_rotate_loop_action action(affine_mat, 180, 360);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        affine_mat.set_rotation_angle(0);
    }

    void sprite_affine_mats_scale_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                        bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal scale",
            "RIGHT: increase horizontal scale",
            "DOWN: decrease vertical scale",
            "UP: increase vertical scale",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite affine mats scale", info_text_lines, text_generator);

        affine_mat.set_horizontal_scale(1.5);
        affine_mat.set_vertical_scale(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_scale = affine_mat.horizontal_scale();
            bn::fixed vertical_scale = affine_mat.vertical_scale();

            if(bn::keypad::left_held())
            {
                affine_mat.set_horizontal_scale(bn::max(horizontal_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::right_held())
            {
                affine_mat.set_horizontal_scale(bn::min(horizontal_scale + 0.01, bn::fixed(2)));
            }

            if(bn::keypad::down_held())
            {
                affine_mat.set_vertical_scale(bn::max(vertical_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::up_held())
            {
                affine_mat.set_vertical_scale(bn::min(vertical_scale + 0.01, bn::fixed(2)));
            }

            info.update();
            bn::core::update();
        }

        affine_mat.set_scale(1);
    }

    void sprite_affine_mats_scale_actions_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                                bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite affine mats scale actions", info_text_lines, text_generator);

        affine_mat.set_scale(0.01);

        bn::sprite_affine_mat_scale_loop_action action(affine_mat, 120, 2);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        affine_mat.set_scale(1);
    }

    void sprite_affine_mats_shear_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                        bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: decrease horizontal shear",
            "RIGHT: increase horizontal shear",
            "DOWN: decrease vertical shear",
            "UP: increase vertical shear",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite affine mats shear", info_text_lines, text_generator);

        affine_mat.set_horizontal_shear(0.5);
        affine_mat.set_vertical_shear(-0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::fixed horizontal_shear = affine_mat.horizontal_shear();
            bn::fixed vertical_shear = affine_mat.vertical_shear();

            if(bn::keypad::left_held())
            {
                affine_mat.set_horizontal_shear(horizontal_shear - 0.01);
            }
            else if(bn::keypad::right_held())
            {
                affine_mat.set_horizontal_shear(horizontal_shear + 0.01);
            }

            if(bn::keypad::down_held())
            {
                affine_mat.set_vertical_shear(vertical_shear - 0.01);
            }
            else if(bn::keypad::up_held())
            {
                affine_mat.set_vertical_shear(vertical_shear + 0.01);
            }

            info.update();
            bn::core::update();
        }

        affine_mat.set_shear(0);
    }

    void sprite_affine_mats_shear_actions_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                                bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite affine mats shear actions", info_text_lines, text_generator);

        affine_mat.set_shear(-0.5);

        bn::sprite_affine_mat_shear_loop_action action(affine_mat, 120, 0.5);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        affine_mat.set_shear(0);
    }

    void sprite_affine_mats_flip_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                       bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "LEFT: disable horizontal flip",
            "RIGHT: enable horizontal flip",
            "DOWN: disable vertical flip",
            "UP: enable vertical flip",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite affine mats flip", info_text_lines, text_generator);

        affine_mat.set_horizontal_flip(true);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_pressed())
            {
                affine_mat.set_horizontal_flip(false);
            }
            else if(bn::keypad::right_pressed())
            {
                affine_mat.set_horizontal_flip(true);
            }

            if(bn::keypad::down_pressed())
            {
                affine_mat.set_vertical_flip(false);
            }
            else if(bn::keypad::up_pressed())
            {
                affine_mat.set_vertical_flip(true);
            }

            info.update();
            bn::core::update();
        }

        affine_mat.set_horizontal_flip(false);
        affine_mat.set_vertical_flip(false);
    }

    void sprite_affine_mats_flip_actions_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                               bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Sprite affine mats flip actions", info_text_lines, text_generator);

        bn::sprite_affine_mat_horizontal_flip_toggle_action action(affine_mat, 120);

        while(! bn::keypad::start_pressed())
        {
            action.update();
            info.update();
            bn::core::update();
        }

        affine_mat.set_horizontal_flip(false);
    }

    void sprite_affine_mats_attributes_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                             bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: change scale",
            "L: decrease rotation angle",
            "R: increase rotation angle",
            "",
            "START: go to next scene",
        };

        common::info info("Sprite affine mats attributes", info_text_lines, text_generator);

        affine_mat.set_rotation_angle(45);
        affine_mat.set_horizontal_scale(1.5);
        affine_mat.set_vertical_scale(0.75);

        while(! bn::keypad::start_pressed())
        {
            bn::affine_mat_attributes attributes = affine_mat.attributes();
            bn::fixed horizontal_scale = attributes.horizontal_scale();
            bn::fixed vertical_scale = attributes.vertical_scale();
            bn::fixed rotation_angle = attributes.rotation_angle();

            if(bn::keypad::left_held())
            {
                attributes.set_horizontal_scale(bn::max(horizontal_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::right_held())
            {
                attributes.set_horizontal_scale(bn::min(horizontal_scale + 0.01, bn::fixed(2)));
            }

            if(bn::keypad::down_held())
            {
                attributes.set_vertical_scale(bn::max(vertical_scale - 0.01, bn::fixed(0.01)));
            }
            else if(bn::keypad::up_held())
            {
                attributes.set_vertical_scale(bn::min(vertical_scale + 0.01, bn::fixed(2)));
            }

            if(bn::keypad::l_held())
            {
                attributes.set_rotation_angle(bn::max(rotation_angle - 1, bn::fixed(0)));
            }
            else if(bn::keypad::r_held())
            {
                attributes.set_rotation_angle(bn::min(rotation_angle + 1, bn::fixed(360)));
            }

            affine_mat.set_attributes(attributes);
            info.update();
            bn::core::update();
        }

        affine_mat.set_attributes(bn::affine_mat_attributes());
    }

    void sprite_affine_mats_attributes_hbe_scene(bn::sprite_affine_mat_ptr& affine_mat,
                                                 bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Attributes H-Blank effect", info_text_lines, text_generator);

        bn::affine_mat_attributes attributes[bn::display::height()];

        bn::sprite_affine_mat_attributes_hbe_ptr attributes_hbe =
                bn::sprite_affine_mat_attributes_hbe_ptr::create(affine_mat, attributes);

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
                degrees_angle += 4;

                if(degrees_angle >= 360)
                {
                    degrees_angle -= 360;
                }

                bn::fixed scale_inc = bn::degrees_lut_sin(degrees_angle) / 2;
                attributes[(bn::display::height() / 2) + index].set_horizontal_scale(1.5 + scale_inc);
                attributes[(bn::display::height() / 2) - index - 1].set_horizontal_scale(1.5 + scale_inc);
            }

            attributes_hbe.reload_attributes_ref();
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

    bn::sprite_affine_mat_ptr affine_mat = bn::sprite_affine_mat_ptr::create();

    bn::sprite_ptr green_sprite = bn::sprite_items::green_sprite.create_sprite(-32, -32 - 16);
    green_sprite.set_affine_mat(affine_mat);
    green_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);

    bn::sprite_ptr yellow_sprite = bn::sprite_items::yellow_sprite.create_sprite(32, -32 + 16);
    yellow_sprite.set_affine_mat(affine_mat);
    yellow_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

    bn::sprite_ptr blue_sprite = bn::sprite_items::blue_sprite.create_sprite(-32, 32 - 16);
    blue_sprite.set_affine_mat(affine_mat);
    blue_sprite.set_double_size_mode(bn::sprite_double_size_mode::DISABLED);

    bn::sprite_ptr red_sprite = bn::sprite_items::red_sprite.create_sprite(32, 32 + 16);
    red_sprite.set_affine_mat(affine_mat);
    red_sprite.set_double_size_mode(bn::sprite_double_size_mode::ENABLED);

    while(true)
    {
        sprite_affine_mats_rotation_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_rotation_actions_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_scale_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_scale_actions_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_shear_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_shear_actions_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_flip_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_flip_actions_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_attributes_scene(affine_mat, text_generator);
        bn::core::update();

        sprite_affine_mats_attributes_hbe_scene(affine_mat, text_generator);
        bn::core::update();
    }
}

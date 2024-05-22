/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_blending.h"
#include "bn_rect_window.h"
#include "bn_regular_bg_ptr.h"
#include "bn_camera_actions.h"
#include "bn_sprite_text_generator.h"

#include "bn_sprite_items_dino.h"
#include "bn_regular_bg_items_land.h"
#include "bn_regular_bg_items_clouds.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

namespace
{
    void camera_scene(bn::camera_ptr& camera, bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "PAD: move camera",
            "",
            "START: go to next scene",
        };

        common::info info("Camera position", info_text_lines, text_generator);

        while(! bn::keypad::start_pressed())
        {
            if(bn::keypad::left_held())
            {
                camera.set_x(camera.x() - 1);
            }
            else if(bn::keypad::right_held())
            {
                camera.set_x(camera.x() + 1);
            }

            if(bn::keypad::up_held())
            {
                camera.set_y(camera.y() - 1);
            }
            else if(bn::keypad::down_held())
            {
                camera.set_y(camera.y() + 1);
            }

            info.update();
            bn::core::update();
        }

        camera.set_position(0, 0);
    }

    void camera_actions_scene(bn::camera_ptr& camera, bn::sprite_text_generator& text_generator)
    {
        constexpr bn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        common::info info("Camera actions", info_text_lines, text_generator);

        bn::fixed amplitude = 30;
        camera.set_position(-amplitude, -amplitude);

        bn::camera_move_loop_action camera_action(camera, 120, amplitude, amplitude);

        while(! bn::keypad::start_pressed())
        {
            camera_action.update();
            info.update();
            bn::core::update();
        }

        camera.set_position(0, 0);
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    bn::regular_bg_ptr land_bg = bn::regular_bg_items::land.create_bg(0, 0);

    bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);
    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    bn::sprite_ptr dino_sprite = bn::sprite_items::dino.create_sprite(-60, 0);

    bn::window outside_window = bn::window::outside();
    outside_window.set_show_bg(clouds_bg, false);

    bn::rect_window internal_window = bn::rect_window::internal();
    internal_window.set_boundaries(-48, 8, 48, 104);

    bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
    land_bg.set_camera(camera);
    dino_sprite.set_camera(camera);
    internal_window.set_camera(camera);

    while(true)
    {
        camera_scene(camera, text_generator);
        bn::core::update();

        camera_actions_scene(camera, text_generator);
        bn::core::update();
    }
}

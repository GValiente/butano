/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_blending.h"
#include "btn_optional.h"
#include "btn_rect_window.h"
#include "btn_regular_bg_ptr.h"
#include "btn_camera_actions.h"
#include "btn_sprite_text_generator.h"

#include "btn_sprite_items_dino.h"
#include "btn_regular_bg_items_land.h"
#include "btn_regular_bg_items_clouds.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

namespace
{
    void camera_scene(btn::camera_ptr& camera, btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "PAD: move camera",
            "",
            "START: go to next scene",
        };

        info info("Camera", info_text_lines, text_generator);

        while(! btn::keypad::start_pressed())
        {
            if(btn::keypad::left_held())
            {
                camera.set_x(camera.x() - 1);
            }
            else if(btn::keypad::right_held())
            {
                camera.set_x(camera.x() + 1);
            }

            if(btn::keypad::up_held())
            {
                camera.set_y(camera.y() - 1);
            }
            else if(btn::keypad::down_held())
            {
                camera.set_y(camera.y() + 1);
            }

            info.update();
            btn::core::update();
        }

        camera.set_position(0, 0);
    }

    void camera_actions_scene(btn::camera_ptr& camera, btn::sprite_text_generator& text_generator)
    {
        constexpr const btn::string_view info_text_lines[] = {
            "START: go to next scene",
        };

        info info("Camera actions", info_text_lines, text_generator);

        btn::fixed amplitude = 30;
        camera.set_position(-amplitude, -amplitude);

        btn::camera_move_loop_action camera_action(camera, 120, amplitude, amplitude);

        while(! btn::keypad::start_pressed())
        {
            camera_action.update();
            info.update();
            btn::core::update();
        }

        camera.set_position(0, 0);
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);

    btn::regular_bg_ptr land_bg = btn::regular_bg_items::land.create_bg(0, 0);

    btn::regular_bg_ptr clouds_bg = btn::regular_bg_items::clouds.create_bg(0, 0);
    btn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    btn::sprite_ptr dino_sprite = btn::sprite_items::dino.create_sprite(-60, 0);

    btn::window outside_window = btn::window::outside();
    outside_window.set_show_bg(clouds_bg, false);

    btn::rect_window internal_window = btn::rect_window::internal();
    internal_window.set_boundaries(-48, 8, 48, 104);

    btn::camera_ptr camera = btn::camera_ptr::create(0, 0);
    land_bg.set_camera(camera);
    dino_sprite.set_camera(camera);
    internal_window.set_camera(camera);

    while(true)
    {
        camera_scene(camera, text_generator);
        btn::core::update();

        camera_actions_scene(camera, text_generator);
        btn::core::update();
    }
}

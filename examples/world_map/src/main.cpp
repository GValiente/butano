/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_optional.h"
#include "bn_fixed_point.h"
#include "bn_affine_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_affine_bg_pa_register_hbe_ptr.h"
#include "bn_affine_bg_pd_register_hbe_ptr.h"
#include "bn_affine_bg_dx_register_hbe_ptr.h"
#include "bn_affine_bg_dy_register_hbe_ptr.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "load_attributes.h"

#include "bn_sprite_items_ninja.h"
#include "bn_affine_bg_items_land.h"

int main()
{
    bn::core::init();

    constexpr bn::string_view info_text_lines[] = {
        "PAD: move ninja",
        "PAD+A: move ninja faster",
        "",
        "START: go to next scene",
    };

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    common::info info("World map", info_text_lines, text_generator);

    bn::affine_bg_ptr land_bg = bn::affine_bg_items::land.create_bg(0, 0);
    land_bg.set_pivot_position(1432, 874);

    int x_limit = (land_bg.dimensions().width() - bn::display::width()) / 2;
    int y_limit = (land_bg.dimensions().height() - bn::display::height()) / 2;

    bn::unique_ptr<bn::array<bn::affine_bg_mat_attributes, bn::display::height()>> land_attributes_ptr(
            new bn::array<bn::affine_bg_mat_attributes, bn::display::height()>());
    bn::array<bn::affine_bg_mat_attributes, bn::display::height()>& land_attributes = *land_attributes_ptr;
    bn::affine_bg_pa_register_hbe_ptr land_pa_hbe =
            bn::affine_bg_pa_register_hbe_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_pd_register_hbe_ptr land_pd_hbe =
            bn::affine_bg_pd_register_hbe_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_dx_register_hbe_ptr land_dx_hbe =
            bn::affine_bg_dx_register_hbe_ptr::create(land_bg, land_attributes._data);
    bn::affine_bg_dy_register_hbe_ptr land_dy_hbe =
            bn::affine_bg_dy_register_hbe_ptr::create(land_bg, land_attributes._data);

    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> ninja_animate_action = bn::create_sprite_animate_action_forever(
                ninja_sprite, 12, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    union direction
    {
       struct
       {
          unsigned up: 1;
          unsigned down: 1;
          unsigned left: 1;
          unsigned right: 1;
       } keys;
       int data = 0;
    };

    direction last_direction;
    last_direction.keys.down = true;

    bool first_frame = true;

    while(true)
    {
        direction new_direction;
        int inc = bn::keypad::a_held() ? 4 : 1;
        bool key_held = false;

        if(bn::keypad::left_held())
        {
            land_bg.set_pivot_x(bn::max(land_bg.pivot_x().right_shift_integer() - inc, 0 - x_limit));
            new_direction.keys.left = true;
            key_held = true;
        }
        else if(bn::keypad::right_held())
        {
            land_bg.set_pivot_x(bn::min(land_bg.pivot_x().right_shift_integer() + inc, x_limit - 1));
            new_direction.keys.right = true;
            key_held = true;
        }

        if(bn::keypad::up_held())
        {
            land_bg.set_pivot_y(bn::max(land_bg.pivot_y().right_shift_integer() - inc, 0 - y_limit));
            new_direction.keys.up = true;
            key_held = true;
        }
        else if(bn::keypad::down_held())
        {
            land_bg.set_pivot_y(bn::min(land_bg.pivot_y().right_shift_integer() + inc, y_limit - 1));
            new_direction.keys.down = true;
            key_held = true;
        }

        load_attributes(land_bg.mat_attributes(), land_attributes._data);

        if(first_frame)
        {
            land_pa_hbe.reload_attributes_ref();
            land_pd_hbe.reload_attributes_ref();
            first_frame = false;
        }

        land_dx_hbe.reload_attributes_ref();
        land_dy_hbe.reload_attributes_ref();

        if(key_held && last_direction.data != new_direction.data)
        {
            if(new_direction.keys.left)
            {
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 12, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
            }
            else if(new_direction.keys.right)
            {
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 12, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
            }

            if(new_direction.keys.up)
            {
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 12, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
            }
            else if(new_direction.keys.down)
            {
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 12, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
            }

            last_direction = new_direction;
        }

        for(int index = 0; index < inc; ++index)
        {
            ninja_animate_action.update();
        }

        info.update();
        bn::core::update();
    }
}

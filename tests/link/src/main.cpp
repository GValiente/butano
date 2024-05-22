/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_link.h"
#include "bn_math.h"
#include "bn_music.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_link_state.h"
#include "bn_rect_window.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_rect_window_boundaries_hbe_ptr.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_music_items.h"
#include "bn_sprite_items_ninja.h"
#include "bn_regular_bg_items_clouds.h"
#include "bn_regular_bg_items_village.h"

namespace
{
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


    [[nodiscard]] bn::optional<direction> read_keypad()
    {
        bn::optional<direction> result;

        if(bn::keypad::up_held())
        {
            if(! result)
            {
                result = direction();
            }

            result->keys.up = true;
        }

        if(bn::keypad::down_held())
        {
            if(! result)
            {
                result = direction();
            }

            result->keys.down = true;
        }

        if(bn::keypad::left_held())
        {
            if(! result)
            {
                result = direction();
            }

            result->keys.left = true;
        }

        if(bn::keypad::right_held())
        {
            if(! result)
            {
                result = direction();
            }

            result->keys.right = true;
        }

        return result;
    }

    void move_ninja(direction new_direction, direction& old_direction, bn::sprite_ptr& ninja_sprite,
                    bn::sprite_animate_action<4>& ninja_animate_action)
    {
        bool direction_changed = false;

        if(new_direction.keys.left)
        {
            ninja_sprite.set_x(ninja_sprite.x() - 1);

            if(new_direction.data != old_direction.data)
            {
                direction_changed = true;
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
            }
        }
        else if(new_direction.keys.right)
        {
            ninja_sprite.set_x(ninja_sprite.x() + 1);

            if(new_direction.data != old_direction.data)
            {
                direction_changed = true;
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
            }
        }

        if(new_direction.keys.up)
        {
            ninja_sprite.set_y(ninja_sprite.y() - 1);

            if(new_direction.data != old_direction.data)
            {
                direction_changed = true;
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
            }
        }
        else if(new_direction.keys.down)
        {
            ninja_sprite.set_y(ninja_sprite.y() + 1);

            if(new_direction.data != old_direction.data)
            {
                direction_changed = true;
                ninja_animate_action = bn::create_sprite_animate_action_forever(
                            ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
            }
        }

        if(direction_changed)
        {
            old_direction = new_direction;
        }
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);

    bn::string_view info_text_lines[] = {
        "PAD: move other player's ninja",
        "A: pause/resume music",
        "R: sleep",
        "L: wake up",
    };

    common::info info("Link communication", info_text_lines, text_generator);

    bn::vector<bn::sprite_ptr, 64> messages_per_second_sprites;
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> ninja_animate_action = bn::create_sprite_animate_action_forever(
                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    bn::regular_bg_ptr village_bg = bn::regular_bg_items::village.create_bg(0, 0);
    bn::regular_bg_ptr clouds_bg = bn::regular_bg_items::clouds.create_bg(0, 0);

    bn::window outside_window = bn::window::outside();
    outside_window.set_show_bg(clouds_bg, false);

    bn::blending::set_transparency_alpha(0.5);
    clouds_bg.set_blending_enabled(true);

    bn::rect_window internal_window = bn::rect_window::internal();
    int amplitude = 56;
    internal_window.set_top(-amplitude);
    internal_window.set_bottom(amplitude);

    bn::array<bn::pair<bn::fixed, bn::fixed>, bn::display::height()> horizontal_boundaries;
    bn::rect_window_boundaries_hbe_ptr horizontal_hblank_effect =
            bn::rect_window_boundaries_hbe_ptr::create_horizontal(internal_window, horizontal_boundaries);
    bn::fixed base_degrees_angle;

    bn::music_items::soda7_xcopy_ohc.play();

    direction old_direction;
    old_direction.keys.down = true;

    int frames_counter = 0;
    int messages_counter = 0;

    while(true)
    {
        if(bn::keypad::a_pressed())
        {
            if(bn::music::paused())
            {
                bn::music::resume();
            }
            else
            {
                bn::music::pause();
            }
        }

        if(bn::keypad::r_pressed())
        {
            bn::core::sleep(bn::keypad::key_type::L);
        }

        if(bn::optional<direction> direction_to_send = read_keypad())
        {
            bn::link::send(direction_to_send->data);
        }

        int max_failed_retries = 5;
        int failed_retries = 0;

        while(failed_retries <= max_failed_retries)
        {
            if(bn::optional<bn::link_state> link_state = bn::link::receive())
            {
                const bn::link_player& first_other_player = link_state->other_players().front();
                direction new_direction;
                new_direction.data = first_other_player.data();
                move_ninja(new_direction, old_direction, ninja_sprite, ninja_animate_action);
                failed_retries = 0;
                ++messages_counter;
            }
            else
            {
                ++failed_retries;
            }
        }

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

        horizontal_hblank_effect.reload_deltas_ref();
        clouds_bg.set_position(clouds_bg.x() + 0.5, clouds_bg.y() + 0.5);

        if(++frames_counter == 60)
        {
            bn::string<64> messages_per_second = "Messages per second: ";
            messages_per_second += bn::to_string<18>(messages_counter);
            messages_per_second_sprites.clear();
            text_generator.generate(0, 44, messages_per_second, messages_per_second_sprites);
            frames_counter = 0;
            messages_counter = 0;
        }

        ninja_animate_action.update();
        info.update();
        bn::core::update();
    }
}

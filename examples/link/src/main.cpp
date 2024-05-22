/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_link.h"
#include "bn_keypad.h"
#include "bn_string.h"
#include "bn_link_state.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_ninja.h"

#include "common_info.h"
#include "common_variable_8x16_sprite_font.h"

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
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    bn::string_view info_text_lines[] = {
        "PAD: move other player's ninja",
    };

    common::info info("Link communication", info_text_lines, text_generator);

    bn::vector<bn::sprite_ptr, 64> messages_per_second_sprites;
    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> ninja_animate_action = bn::create_sprite_animate_action_forever(
                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    direction old_direction;
    old_direction.keys.down = true;

    int frames_counter = 0;
    int messages_counter = 0;

    while(true)
    {
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

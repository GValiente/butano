/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_core.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_link_state.h"
#include "btn_bg_palettes.h"
#include "btn_sprite_text_generator.h"
#include "btn_sprite_animate_actions.h"

#include "btn_sprite_items_ninja.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

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


    [[nodiscard]] direction read_keypad()
    {
        direction result;

        if(btn::keypad::up_held())
        {
            result.keys.up = true;
        }

        if(btn::keypad::down_held())
        {
            result.keys.down = true;
        }

        if(btn::keypad::left_held())
        {
            result.keys.left = true;
        }

        if(btn::keypad::right_held())
        {
            result.keys.right = true;
        }

        return result;
    }
}

int main()
{
    btn::core::init();

    btn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    btn::bg_palettes::set_transparent_color(btn::color(16, 16, 16));

    constexpr const btn::string_view info_text_lines[] = {
        "PAD: move other player's ninja",
    };

    info info("Link communication", info_text_lines, text_generator);

    btn::sprite_ptr ninja_sprite = btn::sprite_items::ninja.create_sprite(0, 0);
    btn::sprite_animate_action<4> action = btn::create_sprite_animate_action_forever(
                ninja_sprite, 16, btn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    direction old_direction;
    old_direction.keys.down = true;

    while(true)
    {
        direction direction_to_send = read_keypad();

        if(btn::optional<btn::link_state> link_state = btn::link_state::get(direction_to_send.data))
        {
            const btn::link_player& first_other_player = link_state->other_players().front();
            direction new_direction;
            new_direction.data = first_other_player.data();

            if(new_direction.keys.left)
            {
                ninja_sprite.set_x(ninja_sprite.x() - 1);

                if(new_direction.data != old_direction.data)
                {
                    action = btn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, btn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
                }
            }
            else if(new_direction.keys.right)
            {
                ninja_sprite.set_x(ninja_sprite.x() + 1);

                if(new_direction.data != old_direction.data)
                {
                    action = btn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, btn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
                }
            }

            if(new_direction.keys.up)
            {
                ninja_sprite.set_y(ninja_sprite.y() - 1);

                if(new_direction.data != old_direction.data)
                {
                    action = btn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, btn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
                }
            }
            else if(new_direction.keys.down)
            {
                ninja_sprite.set_y(ninja_sprite.y() + 1);

                if(new_direction.data != old_direction.data)
                {
                    action = btn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, btn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
                }
            }

            old_direction = new_direction;
        }

        action.update();
        info.update();
        btn::core::update();
    }
}

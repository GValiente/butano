/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_keypad.h"
#include "bn_optional.h"
#include "bn_link_state.h"
#include "bn_bg_palettes.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_ninja.h"

#include "info.h"
#include "variable_8x16_sprite_font.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "bn_log.h"
#include "../../butano/hw/3rd_party/gba-link-connection/include/LinkConnection.h"

#pragma GCC diagnostic pop

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
        bn::optional<direction> result = direction();

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

    bool isBitHigh(u8 bit)
    {
        return (REG_SIOCNT >> bit) & 1;
    }
}

int main()
{
    bn::core::init();

    bn::sprite_text_generator text_generator(variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(16, 16, 16));

    constexpr const bn::string_view info_text_lines[] = {
        "PAD: move other player's ninja",
    };

    info info("Link communication", info_text_lines, text_generator);

    bn::sprite_ptr ninja_sprite = bn::sprite_items::ninja.create_sprite(0, 0);
    bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(
                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);

    direction old_direction;
    old_direction.keys.down = true;

    while(true)
    {
        if(bn::optional<direction> direction_to_send = read_keypad())
        {
            bn::link_state::send(direction_to_send->data + 1);
        }

        bn::optional<bn::link_state> link_state;
        int total_retries = 5;
        int retries = total_retries;

        while(retries)
        {
            if(bn::optional<bn::link_state> new_link_state = bn::link_state::get())
            {
                link_state = new_link_state;
                retries = total_retries;
            }
            else
            {
                --retries;
            }
        }

        if(link_state)
        {
            const bn::link_player& first_other_player = link_state->other_players().front();
            bool direction_changed = false;
            direction new_direction;
            new_direction.data = first_other_player.data() - 1;

            if(new_direction.keys.left)
            {
                ninja_sprite.set_x(ninja_sprite.x() - 1);

                if(new_direction.data != old_direction.data)
                {
                    direction_changed = true;
                    action = bn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 8, 9, 10, 11);
                }
            }
            else if(new_direction.keys.right)
            {
                ninja_sprite.set_x(ninja_sprite.x() + 1);

                if(new_direction.data != old_direction.data)
                {
                    direction_changed = true;
                    action = bn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 12, 13, 14, 15);
                }
            }

            if(new_direction.keys.up)
            {
                ninja_sprite.set_y(ninja_sprite.y() - 1);

                if(new_direction.data != old_direction.data)
                {
                    direction_changed = true;
                    action = bn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 4, 5, 6, 7);
                }
            }
            else if(new_direction.keys.down)
            {
                ninja_sprite.set_y(ninja_sprite.y() + 1);

                if(new_direction.data != old_direction.data)
                {
                    direction_changed = true;
                    action = bn::create_sprite_animate_action_forever(
                                ninja_sprite, 16, bn::sprite_items::ninja.tiles_item(), 0, 1, 2, 3);
                }
            }

            if(direction_changed)
            {
                old_direction = new_direction;
            }

            BN_LOG("received");
        }
        else
        {
            BN_LOG("nothing");
        }

        /*// log player id/count and important flags
        BN_LOG("P", linkConnection->linkState.currentPlayerId, "/",
               linkConnection->linkState.playerCount, "-R",
               isBitHigh(LINK_BIT_READY), "-S",
               isBitHigh(LINK_BIT_START), "-E",
               isBitHigh(LINK_BIT_ERROR), "-I",
               linkConnection->linkState._IRQFlag);*/

        action.update();
        info.update();
        bn::core::update();
    }
}

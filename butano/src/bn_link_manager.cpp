/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_link_manager.h"

#include "../hw/include/bn_hw_link.h"

#include "bn_link.cpp.h"

namespace bn::link_manager
{

void init()
{
    hw::link::init();
}

bool active()
{
    return hw::link::active();
}

void send(int data_to_send)
{
    hw::link::send(data_to_send + 1);
}

optional<link_state> receive()
{
    lc::LinkResponse response;
    optional<link_state> result;

    if(hw::link::receive(response))
    {
        vector<link_player, 3> other_players;

        for(int player_id = 0; player_id < 4; ++player_id)
        {
            int player_data = response.incomingMessages[player_id];

            if(player_data != LINK_NO_DATA)
            {
                other_players.emplace_back(player_id, player_data - 1);
            }
        }

        if(! other_players.empty())
        {
            result.emplace(response.currentPlayerId, response.playerCount, other_players);
        }
    }

    return result;
}

void deactivate()
{
    hw::link::deactivate();
}

void enable()
{
    if(hw::link::active())
    {
        hw::link::enable();
    }
}

void disable()
{
    if(hw::link::active())
    {
        hw::link::disable();
    }
}

}

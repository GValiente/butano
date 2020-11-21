/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_link_manager.h"

#include "../hw/include/bn_hw_link.h"

#include "bn_link.cpp.h"

namespace bn::link_manager
{

namespace
{
    class static_data
    {

    public:
        hw::link::connection connection;
        bool activated = false;
    };

    BN_DATA_EWRAM static_data data;


    void _check_activated()
    {
        if(! data.activated)
        {
            hw::link::enable();
            data.activated = true;
        }
    }
}

void init()
{
    hw::link::init(data.connection);
}

void send(int data_to_send)
{
    _check_activated();

    hw::link::send(data_to_send + 1);
}

optional<link_state> receive()
{
    int current_player_id = 0;
    vector<link_player, 3> other_players;
    _check_activated();

    hw::link::block();

    if(hw::link::state* link_state = hw::link::current_state())
    {
        current_player_id = int(link_state->currentPlayerId);

        for(int player_id = 0, players_count = link_state->playerCount; player_id < players_count; ++player_id)
        {
            if(player_id != current_player_id)
            {
                int player_data = link_state->readMessage(u8(player_id));

                if(player_data != LINK_NO_DATA && player_data != LINK_DISCONNECTED)
                {
                    BN_ASSERT(! other_players.full(), "Too much players");

                    other_players.emplace_back(player_id, player_data - 1);
                }
            }
        }
    }

    hw::link::unblock();

    optional<link_state> result;

    if(! other_players.empty())
    {
        result.emplace(current_player_id, other_players);
    }

    return result;
}

void deactivate()
{
    if(data.activated)
    {
        data.activated = false;
        hw::link::disable();
    }
}

void enable()
{
    if(data.activated)
    {
        hw::link::enable();
    }
}

void disable()
{
    if(data.activated)
    {
        hw::link::disable();
    }
}

void commit()
{
    if(data.activated)
    {
        hw::link::commit();
    }
}

}

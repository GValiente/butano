/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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
        LinkConnection connection;
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
    LinkConnection::Response response;
    optional<link_state> result;
    _check_activated();

    if(hw::link::receive(response))
    {
        vector<link_player, 3> other_players;

        for(int player_id = 0; player_id < 4; ++player_id)
        {
            int player_data = response.incomingMessages[player_id];

            if(player_data != LINK_NO_DATA)
            {
                BN_ASSERT(! other_players.full(), "Too much players");

                other_players.emplace_back(player_id, player_data - 1);
            }
        }

        if(! other_players.empty())
        {
            result.emplace(response.currentPlayerId, other_players);
        }
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

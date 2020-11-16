/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_link_manager.h"

#include "bn_vector.h"
#include "bn_link_player.h"
#include "bn_config_link.h"
#include "../hw/include/bn_hw_link.h"

namespace bn::link_manager
{

namespace
{
    constexpr const int baud_rate = BN_CFG_LINK_BAUD_RATE;

    static_assert(baud_rate == BN_LINK_BAUD_RATE_9600_BPS || baud_rate == BN_LINK_BAUD_RATE_38400_BPS ||
            baud_rate == BN_LINK_BAUD_RATE_57600_BPS || baud_rate == BN_LINK_BAUD_RATE_115200_BPS);


    class static_data
    {

    public:
        hw::link::connection connection;
    };

    BN_DATA_EWRAM static_data data;
}

void init()
{
    hw::link::init(baud_rate, data.connection);
}

void send(int data_to_send)
{
    data.connection.send(u16(data_to_send));
}

bool get(int& current_player_id, vector<link_player, 3>& other_players)
{
    hw::link::state& link_state = data.connection.linkState;

    if(! link_state.isConnected())
    {
        return false;
    }

    auto this_player_id = int(link_state.currentPlayerId);

    for(int player_id = 0; player_id < 4; ++player_id)
    {
        if(player_id != this_player_id)
        {
            int player_data = link_state.readMessage(u8(player_id));

            if(player_data != LINK_NO_DATA && player_data != LINK_DISCONNECTED)
            {
                BN_ASSERT(! other_players.full(), "Too much players");

                other_players.emplace_back(player_id, player_data);
            }
        }
    }

    if(other_players.empty())
    {
        return false;
    }

    current_player_id = this_player_id;
    return true;
}

void enable()
{
    hw::link::enable();
}

void disable()
{
    hw::link::disable();
}

void update()
{
    int current_player_id = 0;
    vector<link_player, 3> other_players;

    while(get(current_player_id, other_players))
    {
        other_players.clear();
    }
}

}

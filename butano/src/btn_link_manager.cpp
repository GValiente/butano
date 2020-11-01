/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_link_manager.h"

#include "btn_vector.h"
#include "btn_link_player.h"
#include "btn_config_link.h"
#include "../hw/include/btn_hw_link.h"

namespace btn::link_manager
{

namespace
{
    constexpr const int baud_rate = BTN_CFG_LINK_BAUD_RATE;

    static_assert(baud_rate == BTN_LINK_BAUD_RATE_9600_BPS || baud_rate == BTN_LINK_BAUD_RATE_38400_BPS ||
            baud_rate == BTN_LINK_BAUD_RATE_57600_BPS || baud_rate == BTN_LINK_BAUD_RATE_115200_BPS);


    class static_data
    {

    public:
        hw::link::connection connection;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    hw::link::init(baud_rate, data.connection);
}

bool tick(int data_to_send, int& current_player_id, vector<link_player, 3>& other_players)
{
    hw::link::state new_state = hw::link::tick(data_to_send);

    if(! new_state.isConnected())
    {
        return false;
    }

    auto this_player_id = int(new_state.currentPlayerId);

    for(int player_id = 0; player_id < 4; ++player_id)
    {
        if(player_id != this_player_id)
        {
            int player_data = new_state.data[player_id];

            if(player_data != LINK_NO_DATA)
            {
                BTN_ASSERT(! other_players.full(), "Too much players");

                other_players.emplace_back(player_id, player_data);
            }
        }
    }

    BTN_ASSERT(other_players.size() == int(new_state.playerCount) - 1, "Invalid players data");

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

}

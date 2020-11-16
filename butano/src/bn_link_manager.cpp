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
        bool running = false;
    };

    BN_DATA_EWRAM static_data data;
}

bool tick(int data_to_send, int& current_player_id, vector<link_player, 3>& other_players)
{
    if(! data.running)
    {
        hw::link::init(baud_rate, data.connection);
        data.running = true;
    }

    hw::link::state link_state = data.connection.tick(uint16_t(data_to_send));
    auto this_player_id = int(link_state.currentPlayerId);

    for(int player_id = 0; player_id < 4; ++player_id)
    {
        if(player_id != this_player_id)
        {
            int player_data = link_state.data[player_id];

            if(player_data > 0 && player_data < 0xFFFF)
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
    if(data.running)
    {
        hw::link::enable();
    }
}

void disable()
{
    if(data.running)
    {
        hw::link::disable();
    }
}

}

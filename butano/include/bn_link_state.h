/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_STATE_H
#define BN_LINK_STATE_H

#include "bn_vector.h"
#include "bn_link_player.h"

namespace bn
{

class link_state
{

public:
    link_state(int current_player_id, const vector<link_player, 3>& other_players) :
        _other_players(other_players),
        _current_player_id(current_player_id)
    {
        BN_ASSERT(current_player_id >= 0 && current_player_id <= 3, "Invalid current player id: ", current_player_id);
    }

    [[nodiscard]] int current_player_id() const
    {
        return _current_player_id;
    }

    [[nodiscard]] const ivector<link_player>& other_players() const
    {
        return _other_players;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const link_state& a, const link_state& b) = default;

private:
    vector<link_player, 3> _other_players;
    int _current_player_id = 0;

    link_state() = default;
};

}

#endif

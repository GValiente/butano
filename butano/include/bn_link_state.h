/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_STATE_H
#define BN_LINK_STATE_H

#include "bn_vector.h"
#include "bn_link_player.h"
#include "bn_optional_fwd.h"

namespace bn
{

class link_state
{

public:
    [[nodiscard]] static optional<link_state> get(int data_to_send);

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

/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_STATE_H
#define BN_LINK_STATE_H

/**
 * @file
 * bn::link_state header file.
 *
 * @ingroup link
 */

#include "bn_vector.h"
#include "bn_link_player.h"

namespace bn
{

/**
 * @brief Stores the state of a communication through the link cable.
 *
 * @ingroup link
 */
class link_state
{

public:
    /**
     * @brief Constructor.
     * @param current_player_id ID of this player, in the range [0..3].
     * @param other_players Data provided by the other players.
     */
    link_state(int current_player_id, const vector<link_player, 3>& other_players) :
        _other_players(other_players),
        _current_player_id(current_player_id)
    {
        BN_ASSERT(current_player_id >= 0 && current_player_id <= 3, "Invalid current player id: ", current_player_id);
    }

    /**
     * @brief Returns the ID of this player.
     */
    [[nodiscard]] int current_player_id() const
    {
        return _current_player_id;
    }

    /**
     * @brief Returns the data provided by the other players.
     */
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
};

}

#endif

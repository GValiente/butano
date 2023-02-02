/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
        _current_player_id(uint16_t(current_player_id)),
        _player_count(uint16_t(other_players.size() + 1))
    {
        BN_ASSERT(current_player_id >= 0 && current_player_id <= 3,
                  "Invalid current player id: ", current_player_id);
    }

    /**
     * @brief Constructor.
     * @param current_player_id ID of this player, in the range [0..3].
     * @param player_count Number of connected players (including this one).
     * @param other_players Data provided by the other players.
     */
    link_state(int current_player_id, int player_count, const vector<link_player, 3>& other_players) :
        _other_players(other_players),
        _current_player_id(uint16_t(current_player_id)),
        _player_count(uint16_t(player_count))
    {
        BN_ASSERT(current_player_id >= 0 && current_player_id <= 3,
                  "Invalid current player id: ", current_player_id);
        BN_ASSERT(player_count > other_players.size() && player_count <= 4,
                  "Invalid player count: ", player_count, " - ", other_players.size());
    }

    /**
     * @brief Returns the ID of this player.
     */
    [[nodiscard]] int current_player_id() const
    {
        return _current_player_id;
    }

    /**
     * @brief Returns the number of connected players (including this one).
     */
    [[nodiscard]] int player_count() const
    {
        return _player_count;
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
    uint16_t _current_player_id = 0;
    uint16_t _player_count = 0;
};

}

#endif

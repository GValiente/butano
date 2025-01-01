/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_PLAYER_H
#define BN_LINK_PLAYER_H

/**
 * @file
 * bn::link_player header file.
 *
 * @ingroup link
 */

#include "bn_assert.h"

namespace bn
{

/**
 * @brief Contains the data of a message provided by a player.
 *
 * @ingroup link
 */
class link_player
{

public:
    /**
     * @brief Constructor.
     * @param id Player ID, in the range [0..3].
     * @param data Player data, in the range [0..65533].
     */
    constexpr link_player(int id, int data) :
        _id(uint16_t(id)),
        _data(uint16_t(data))
    {
        BN_ASSERT(id >= 0 && id <= 3, "Invalid id: ", id);
        BN_ASSERT(data >= 0 && data <= 65533, "Invalid data: ", data);
    }

    /**
     * @brief Returns the ID of the player.
     */
    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

    /**
     * @brief Returns the data provided by the player.
     */
    [[nodiscard]] constexpr uint16_t data() const
    {
        return _data;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const link_player& a, const link_player& b) = default;

private:
    uint16_t _id;
    uint16_t _data;
};

}

#endif

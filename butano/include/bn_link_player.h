/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_PLAYER_H
#define BN_LINK_PLAYER_H

#include "bn_assert.h"

namespace bn
{

class link_player
{

public:
    constexpr link_player(int id, int data) :
        _id(uint16_t(id)),
        _data(uint16_t(data))
    {
        BN_ASSERT(id >= 0 && id <= 3, "Invalid id: ", id);
        BN_ASSERT(data > 0 && data < 0xFFFF, "Invalid data: ", data);
    }

    [[nodiscard]] constexpr int id() const
    {
        return _id;
    }

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

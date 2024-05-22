/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_RIVAL_CAR_INFO_H
#define FR_RIVAL_CAR_INFO_H

#include "bn_assert.h"

namespace bn
{
    class sprite_item;
}

namespace fr
{

class rival_car_info
{

public:
    constexpr rival_car_info(const bn::sprite_item& sprite_item, int start_checkpoint_index) :
        _sprite_item(sprite_item),
        _start_checkpoint_index(start_checkpoint_index)
    {
        BN_ASSERT(start_checkpoint_index >= 0);
    }

    [[nodiscard]] constexpr const bn::sprite_item& sprite_item() const
    {
        return _sprite_item;
    }

    [[nodiscard]] constexpr int start_checkpoint_index() const
    {
        return _start_checkpoint_index;
    }

private:
    const bn::sprite_item& _sprite_item;
    int _start_checkpoint_index;
};

}

#endif

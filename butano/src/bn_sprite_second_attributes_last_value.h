/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_SECOND_ATTRIBUTES_LAST_VALUE_H
#define BN_SPRITE_SECOND_ATTRIBUTES_LAST_VALUE_H

#include "bn_point.h"
#include "bn_sprite_shape_size.h"
#include "bn_sprites_manager.h"

namespace bn
{

class sprite_second_attributes_last_value
{

public:
    int hw_x = -1;
    sprite_size size = sprite_size::SMALL;

    sprite_second_attributes_last_value() = default;

    explicit sprite_second_attributes_last_value(void* handle) :
        hw_x(sprites_manager::hw_position(handle).x()),
        size(sprites_manager::size(handle))
    {
    }

    explicit sprite_second_attributes_last_value(intptr_t target_id) :
        sprite_second_attributes_last_value(reinterpret_cast<void*>(target_id))
    {
    }

    [[nodiscard]] friend bool operator==(const sprite_second_attributes_last_value& a,
                                         const sprite_second_attributes_last_value& b) = default;
};

}

#endif

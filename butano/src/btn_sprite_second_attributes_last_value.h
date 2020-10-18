#ifndef BTN_SPRITE_SECOND_ATTRIBUTES_LAST_VALUE_H
#define BTN_SPRITE_SECOND_ATTRIBUTES_LAST_VALUE_H

#include "btn_point.h"
#include "btn_sprite_shape_size.h"
#include "btn_sprites_manager.h"

namespace btn
{

class alignas(int) sprite_second_attributes_last_value
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

    explicit sprite_second_attributes_last_value(int target_id) :
        sprite_second_attributes_last_value(reinterpret_cast<void*>(target_id))
    {
    }

    [[nodiscard]] friend bool operator==(const sprite_second_attributes_last_value& a,
                                         const sprite_second_attributes_last_value& b) = default;
};

}

#endif

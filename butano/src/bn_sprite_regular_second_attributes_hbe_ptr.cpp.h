/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_regular_second_attributes_hbe_ptr.h"

#include "bn_display.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

sprite_regular_second_attributes_hbe_ptr sprite_regular_second_attributes_hbe_ptr::create(
        sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), intptr_t(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES);
    return sprite_regular_second_attributes_hbe_ptr(id, move(sprite));
}

optional<sprite_regular_second_attributes_hbe_ptr> sprite_regular_second_attributes_hbe_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_regular_second_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), intptr_t(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_REGULAR_SECOND_ATTRIBUTES);
    optional<sprite_regular_second_attributes_hbe_ptr> result;

    if(id >= 0)
    {
        result = sprite_regular_second_attributes_hbe_ptr(id, move(sprite));
    }

    return result;
}

span<const sprite_regular_second_attributes> sprite_regular_second_attributes_hbe_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_regular_second_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_regular_second_attributes>(values_ptr, display::height());
}

void sprite_regular_second_attributes_hbe_ptr::set_attributes_ref(
        const span<const sprite_regular_second_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_regular_second_attributes_hbe_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_regular_second_attributes_hbe_ptr::swap(sprite_regular_second_attributes_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _sprite.swap(other._sprite);
}

sprite_regular_second_attributes_hbe_ptr::sprite_regular_second_attributes_hbe_ptr(int id, sprite_ptr&& sprite) :
    hbe_ptr(id),
    _sprite(move(sprite))
{
}

}

/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_position_hbe_ptr.h"

#include "bn_display.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

sprite_position_hbe_ptr sprite_position_hbe_ptr::create_horizontal(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), intptr_t(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_HORIZONTAL_POSITION);
    return sprite_position_hbe_ptr(id, move(sprite));
}

sprite_position_hbe_ptr sprite_position_hbe_ptr::create_vertical(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), intptr_t(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_VERTICAL_POSITION);
    return sprite_position_hbe_ptr(id, move(sprite));
}

optional<sprite_position_hbe_ptr> sprite_position_hbe_ptr::create_horizontal_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), intptr_t(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_HORIZONTAL_POSITION);
    optional<sprite_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hbe_ptr(id, move(sprite));
    }

    return result;
}

optional<sprite_position_hbe_ptr> sprite_position_hbe_ptr::create_vertical_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), intptr_t(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_VERTICAL_POSITION);
    optional<sprite_position_hbe_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hbe_ptr(id, move(sprite));
    }

    return result;
}

span<const fixed> sprite_position_hbe_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void sprite_position_hbe_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void sprite_position_hbe_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_position_hbe_ptr::swap(sprite_position_hbe_ptr& other)
{
    hbe_ptr::swap(other);
    _sprite.swap(other._sprite);
}

sprite_position_hbe_ptr::sprite_position_hbe_ptr(int id, sprite_ptr&& sprite) :
    hbe_ptr(id),
    _sprite(move(sprite))
{
}

}

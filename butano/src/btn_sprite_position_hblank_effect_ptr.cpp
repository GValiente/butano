/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_position_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

sprite_position_hblank_effect_ptr sprite_position_hblank_effect_ptr::create_horizontal(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_HORIZONTAL_POSITION);
    return sprite_position_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_position_hblank_effect_ptr> sprite_position_hblank_effect_ptr::create_horizontal_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_HORIZONTAL_POSITION);
    optional<sprite_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

sprite_position_hblank_effect_ptr sprite_position_hblank_effect_ptr::create_vertical(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_VERTICAL_POSITION);
    return sprite_position_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_position_hblank_effect_ptr> sprite_position_hblank_effect_ptr::create_vertical_optional(
        sprite_ptr sprite, const span<const fixed>& deltas_ref)
{
    int id = hblank_effects_manager::create_optional(deltas_ref.data(), deltas_ref.size(), int(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_VERTICAL_POSITION);
    optional<sprite_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_position_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

span<const fixed> sprite_position_hblank_effect_ptr::deltas_ref() const
{
    auto values_ptr = reinterpret_cast<const fixed*>(hblank_effects_manager::values_ref(id()));
    return span<const fixed>(values_ptr, display::height());
}

void sprite_position_hblank_effect_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void sprite_position_hblank_effect_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_position_hblank_effect_ptr::swap(sprite_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    _sprite.swap(other._sprite);
}

sprite_position_hblank_effect_ptr::sprite_position_hblank_effect_ptr(int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}

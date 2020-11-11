/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_first_attributes_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_first_attributes.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

sprite_first_attributes_hblank_effect_ptr sprite_first_attributes_hblank_effect_ptr::create(
        sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), int(sprite.handle()),
                                            hblank_effects_manager::handler_type::SPRITE_FIRST_ATTRIBUTES);
    return sprite_first_attributes_hblank_effect_ptr(id, move(sprite));
}

optional<sprite_first_attributes_hblank_effect_ptr> sprite_first_attributes_hblank_effect_ptr::create_optional(
        sprite_ptr sprite, const span<const sprite_first_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), int(sprite.handle()),
                                                     hblank_effects_manager::handler_type::SPRITE_FIRST_ATTRIBUTES);
    optional<sprite_first_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_first_attributes_hblank_effect_ptr(id, move(sprite));
    }

    return result;
}

span<const sprite_first_attributes> sprite_first_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const sprite_first_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_first_attributes>(values_ptr, display::height());
}

void sprite_first_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_first_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_first_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_first_attributes_hblank_effect_ptr::swap(sprite_first_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    _sprite.swap(other._sprite);
}

sprite_first_attributes_hblank_effect_ptr::sprite_first_attributes_hblank_effect_ptr(int id, sprite_ptr&& sprite) :
    hblank_effect_ptr(id),
    _sprite(move(sprite))
{
}

}

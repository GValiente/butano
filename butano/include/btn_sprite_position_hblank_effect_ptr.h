/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_POSITION_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_POSITION_HBLANK_EFFECT_PTR_H

#include "btn_sprite_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class sprite_position_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_position_hblank_effect_ptr create_horizontal(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    [[nodiscard]] static optional<sprite_position_hblank_effect_ptr> create_horizontal_optional(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    [[nodiscard]] static sprite_position_hblank_effect_ptr create_vertical(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    [[nodiscard]] static optional<sprite_position_hblank_effect_ptr> create_vertical_optional(
            sprite_ptr sprite, const span<const fixed>& deltas_ref);

    sprite_position_hblank_effect_ptr(const sprite_position_hblank_effect_ptr& other) = default;

    sprite_position_hblank_effect_ptr& operator=(const sprite_position_hblank_effect_ptr& other) = default;

    sprite_position_hblank_effect_ptr(sprite_position_hblank_effect_ptr&& other) noexcept;

    sprite_position_hblank_effect_ptr& operator=(sprite_position_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] span<const fixed> deltas_ref() const;

    void set_deltas_ref(const span<const fixed>& deltas_ref);

    void reload_deltas_ref();

    void swap(sprite_position_hblank_effect_ptr& other);

    friend void swap(sprite_position_hblank_effect_ptr& a, sprite_position_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    sprite_ptr _sprite;

    sprite_position_hblank_effect_ptr(int id, sprite_ptr&& sprite);
};

}

#endif

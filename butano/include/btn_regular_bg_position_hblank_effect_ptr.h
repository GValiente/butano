/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_REGULAR_BG_POSITION_HBLANK_EFFECT_PTR_H
#define BTN_REGULAR_BG_POSITION_HBLANK_EFFECT_PTR_H

#include "btn_regular_bg_ptr.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class regular_bg_position_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static regular_bg_position_hblank_effect_ptr create_horizontal(
            regular_bg_ptr bg, const span<const fixed>& deltas_ref);

    [[nodiscard]] static optional<regular_bg_position_hblank_effect_ptr> create_horizontal_optional(
            regular_bg_ptr bg, const span<const fixed>& deltas_ref);

    [[nodiscard]] static regular_bg_position_hblank_effect_ptr create_vertical(
            regular_bg_ptr bg, const span<const fixed>& deltas_ref);

    [[nodiscard]] static optional<regular_bg_position_hblank_effect_ptr> create_vertical_optional(
            regular_bg_ptr bg, const span<const fixed>& deltas_ref);

    regular_bg_position_hblank_effect_ptr(const regular_bg_position_hblank_effect_ptr& other) = default;

    regular_bg_position_hblank_effect_ptr& operator=(const regular_bg_position_hblank_effect_ptr& other) = default;

    regular_bg_position_hblank_effect_ptr(regular_bg_position_hblank_effect_ptr&& other) noexcept;

    regular_bg_position_hblank_effect_ptr& operator=(regular_bg_position_hblank_effect_ptr&& other) noexcept;

    [[nodiscard]] const regular_bg_ptr& bg() const
    {
        return _bg;
    }

    [[nodiscard]] span<const fixed> deltas_ref() const;

    void set_deltas_ref(const span<const fixed>& deltas_ref);

    void reload_deltas_ref();

    void swap(regular_bg_position_hblank_effect_ptr& other);

    friend void swap(regular_bg_position_hblank_effect_ptr& a, regular_bg_position_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    regular_bg_ptr _bg;

    regular_bg_position_hblank_effect_ptr(int id, regular_bg_ptr&& bg);
};

}

#endif

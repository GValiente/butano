/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_PTR_H
#define BTN_BG_PALETTES_TRANSPARENT_COLOR_HBLANK_EFFECT_PTR_H

#include "btn_optional_fwd.h"
#include "btn_hblank_effect_ptr.h"

namespace btn
{

class color;

class bg_palettes_transparent_color_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static bg_palettes_transparent_color_hblank_effect_ptr create(
            const span<const color>& colors_ref);

    [[nodiscard]] static optional<bg_palettes_transparent_color_hblank_effect_ptr> create_optional(
            const span<const color>& colors_ref);

    [[nodiscard]] span<const color> colors_ref() const;

    void set_colors_ref(const span<const color>& colors_ref);

    void reload_colors_ref();

    void swap(bg_palettes_transparent_color_hblank_effect_ptr& other);

    friend void swap(bg_palettes_transparent_color_hblank_effect_ptr& a,
                     bg_palettes_transparent_color_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bg_palettes_transparent_color_hblank_effect_ptr(int id);
};

}

#endif

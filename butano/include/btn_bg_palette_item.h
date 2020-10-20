/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTE_ITEM_H
#define BTN_BG_PALETTE_ITEM_H

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional_fwd.h"
#include "btn_palette_bpp_mode.h"

namespace btn
{

class bg_palette_ptr;

class bg_palette_item
{

public:
    constexpr bg_palette_item(const span<const color>& colors, palette_bpp_mode bpp_mode) :
        _colors(colors),
        _bpp_mode(bpp_mode)
    {
        BTN_ASSERT(_colors.size() >= 16 && _colors.size() <= 256 && _colors.size() % 16 == 0,
                   "Invalid colors count: ", _colors.size());
    }

    [[nodiscard]] constexpr const span<const color>& colors() const
    {
        return _colors;
    }

    [[nodiscard]] constexpr palette_bpp_mode bpp_mode() const
    {
        return _bpp_mode;
    }

    [[nodiscard]] optional<bg_palette_ptr> find_palette() const;

    [[nodiscard]] bg_palette_ptr create_palette() const;

    [[nodiscard]] bg_palette_ptr create_new_palette() const;

    [[nodiscard]] optional<bg_palette_ptr> create_palette_optional() const;

    [[nodiscard]] optional<bg_palette_ptr> create_new_palette_optional() const;

    [[nodiscard]] constexpr friend bool operator==(const bg_palette_item& a, const bg_palette_item& b) = default;

private:
    span<const color> _colors;
    palette_bpp_mode _bpp_mode;
};

}

#endif


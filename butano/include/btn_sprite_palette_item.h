/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PALETTE_ITEM_H
#define BTN_SPRITE_PALETTE_ITEM_H

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional_fwd.h"
#include "btn_palette_bpp_mode.h"

namespace btn
{

class sprite_palette_ptr;

class sprite_palette_item
{

public:
    constexpr sprite_palette_item(const span<const color>& colors, palette_bpp_mode bpp_mode) :
        _colors(colors),
        _bpp_mode(bpp_mode)
    {
        BTN_ASSERT((bpp_mode == palette_bpp_mode::BPP_4 && _colors.size() == 16) ||
                   (bpp_mode == palette_bpp_mode::BPP_8 && _colors.size() >= 16 && _colors.size() <= 256 &&
                            _colors.size() % 16 == 0),
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

    [[nodiscard]] optional<sprite_palette_ptr> find_palette() const;

    [[nodiscard]] sprite_palette_ptr create_palette() const;

    [[nodiscard]] sprite_palette_ptr create_new_palette() const;

    [[nodiscard]] optional<sprite_palette_ptr> create_palette_optional() const;

    [[nodiscard]] optional<sprite_palette_ptr> create_new_palette_optional() const;

    [[nodiscard]] constexpr friend bool operator==(const sprite_palette_item& a,
                                                   const sprite_palette_item& b) = default;

private:
    span<const color> _colors;
    palette_bpp_mode _bpp_mode;
};

}

#endif


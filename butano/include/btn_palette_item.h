#ifndef BTN_PALETTE_ITEM_H
#define BTN_PALETTE_ITEM_H

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional_fwd.h"

namespace btn
{

class bg_palette_ptr;
class sprite_palette_ptr;
enum class create_mode;

class palette_item
{

public:
    constexpr explicit palette_item(const span<const color>& palette) :
        _palette(palette)
    {
        BTN_CONSTEXPR_ASSERT(colors() >= 16 && colors() <= 256 && colors() % 16 == 0, "Invalid colors count");
    }

    [[nodiscard]] constexpr const span<const color>& palette() const
    {
        return _palette;
    }

    [[nodiscard]] constexpr int colors() const
    {
        return int(_palette.size());
    }

    [[nodiscard]] constexpr bool eight_bits_per_pixel() const
    {
        return _palette.size() > 16;
    }

    [[nodiscard]] optional<sprite_palette_ptr> create_sprite_palette(create_mode create_mode) const;

    [[nodiscard]] optional<bg_palette_ptr> create_bg_palette(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const palette_item& a, const palette_item& b)
    {
        return a._palette.data() == b._palette.data() && a._palette.size() == b._palette.size();
    }

    [[nodiscard]] constexpr friend bool operator!=(const palette_item& a, const palette_item& b)
    {
        return ! (a == b);
    }

private:
    span<const color> _palette;
};

}

#endif


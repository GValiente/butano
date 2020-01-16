#ifndef BTN_SPRITE_PALETTE_ITEM_H
#define BTN_SPRITE_PALETTE_ITEM_H

#include "btn_span.h"
#include "btn_color.h"

namespace btn
{

class sprite_palette_ptr;
enum class create_mode;

class sprite_palette_item
{

public:
    constexpr explicit sprite_palette_item(const span<const color>& palette) :
        _palette(palette)
    {
        BTN_CONSTEXPR_ASSERT(colors() >= 16 && colors() <= 256, "Invalid colors count");
    }

    [[nodiscard]] constexpr const span<const color>& palette() const
    {
        return _palette;
    }

    [[nodiscard]] constexpr int colors() const
    {
        return int(_palette.size());
    }

    [[nodiscard]] sprite_palette_ptr palette_ptr(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const sprite_palette_item& a, const sprite_palette_item& b)
    {
        return a._palette.data() == b._palette.data() && a._palette.size() == b._palette.size();
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_palette_item& a, const sprite_palette_item& b)
    {
        return ! (a == b);
    }

private:
    span<const color> _palette;
};

}

#endif


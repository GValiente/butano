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
        BTN_CONSTEXPR_ASSERT(palette.size() >= 16 && palette.size() <= 256, "Invalid palette size");
    }

    [[nodiscard]] constexpr const span<const color>& palette() const
    {
        return _palette;
    }

    [[nodiscard]] sprite_palette_ptr palette_ptr(create_mode create_mode) const;

    [[nodiscard]] constexpr bool operator==(sprite_palette_item other) const
    {
        return _palette.data() == other._palette.data() && _palette.size() == other._palette.size();
    }

    [[nodiscard]] constexpr bool operator!=(sprite_palette_item other) const
    {
        return ! (*this == other);
    }

private:
    span<const color> _palette;
};

}

#endif


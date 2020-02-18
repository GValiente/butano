#ifndef BTN_BG_PALETTE_ITEM_H
#define BTN_BG_PALETTE_ITEM_H

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional_fwd.h"
#include "btn_palette_bpp_mode.h"

namespace btn
{

class bg_palette_ptr;
enum class create_mode;

class bg_palette_item
{

public:
    constexpr bg_palette_item(const span<const color>& palette_ref, palette_bpp_mode bpp_mode) :
        _palette_ref(palette_ref),
        _bpp_mode(bpp_mode)
    {
        BTN_CONSTEXPR_ASSERT(colors_count() >= 16 && colors_count() <= 256 && colors_count() % 16 == 0,
                             "Invalid colors count");
    }

    [[nodiscard]] constexpr const span<const color>& palette_ref() const
    {
        return _palette_ref;
    }

    [[nodiscard]] constexpr int colors_count() const
    {
        return _palette_ref.size();
    }

    [[nodiscard]] constexpr palette_bpp_mode bpp_mode() const
    {
        return _bpp_mode;
    }

    [[nodiscard]] optional<bg_palette_ptr> create_palette(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const bg_palette_item& a, const bg_palette_item& b)
    {
        return a._palette_ref.data() == b._palette_ref.data() && a._palette_ref.size() == b._palette_ref.size() &&
                a._bpp_mode == b._bpp_mode;
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_palette_item& a, const bg_palette_item& b)
    {
        return ! (a == b);
    }

private:
    span<const color> _palette_ref;
    palette_bpp_mode _bpp_mode;
};

}

#endif


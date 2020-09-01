#ifndef BTN_SPRITE_FIRST_ATTRIBUTES_H
#define BTN_SPRITE_FIRST_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

class sprite_first_attributes
{

public:
    constexpr sprite_first_attributes() = default;

    constexpr sprite_first_attributes(fixed y, bool mosaic_enabled, bool blending_enabled, bool window_enabled,
                                      bool visible) :
        _y(y),
        _mosaic_enabled(mosaic_enabled),
        _blending_enabled(blending_enabled),
        _window_enabled(window_enabled),
        _visible(visible)
    {
        BTN_CONSTEXPR_ASSERT(! blending_enabled || ! window_enabled,
                             "Blending and window can't be enabled at the same time");
    }

    [[nodiscard]] constexpr fixed y() const
    {
        return _y;
    }

    void constexpr set_y(fixed y)
    {
        _y = y;
    }

    [[nodiscard]] constexpr bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    constexpr void set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
    }

    [[nodiscard]] constexpr bool blending_enabled() const
    {
        return _blending_enabled;
    }

    constexpr void set_blending_enabled(bool blending_enabled)
    {
        BTN_CONSTEXPR_ASSERT(! blending_enabled || ! _window_enabled,
                             "Blending and window can't be enabled at the same time");

        _blending_enabled = blending_enabled;
    }

    [[nodiscard]] constexpr bool window_enabled() const
    {
        return _window_enabled;
    }

    constexpr void set_window_enabled(bool window_enabled)
    {
        BTN_CONSTEXPR_ASSERT(! _blending_enabled || ! window_enabled,
                             "Blending and window can't be enabled at the same time");

        _window_enabled = window_enabled;
    }

    [[nodiscard]] constexpr bool visible() const
    {
        return _visible;
    }

    constexpr void set_visible(bool visible)
    {
        _visible = visible;
    }

    [[nodiscard]] constexpr friend bool operator==(const sprite_first_attributes& a,
                                                   const sprite_first_attributes& b) = default;

private:
    fixed _y = 0;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _window_enabled = false;
    bool _visible = false;
};

}

#endif


#ifndef BTN_MOSAIC_ATTRIBUTES_H
#define BTN_MOSAIC_ATTRIBUTES_H

#include "btn_fixed.h"

namespace btn
{

class mosaic_attributes
{

public:
    constexpr mosaic_attributes() = default;

    constexpr mosaic_attributes(fixed sprites_horizontal_stretch, fixed sprites_vertical_stretch,
                                fixed bgs_horizontal_stretch, fixed bgs_vertical_stretch) :
        _sprites_horizontal_stretch(sprites_horizontal_stretch),
        _sprites_vertical_stretch(sprites_vertical_stretch),
        _bgs_horizontal_stretch(bgs_horizontal_stretch),
        _bgs_vertical_stretch(bgs_vertical_stretch)
    {
        BTN_ASSERT(sprites_horizontal_stretch >= 0 && sprites_horizontal_stretch <= 1,
                   "Invalid sprites horizontal stretch: ", sprites_horizontal_stretch);
        BTN_ASSERT(sprites_vertical_stretch >= 0 && sprites_vertical_stretch <= 1,
                   "Invalid sprites vertical stretch: ", sprites_vertical_stretch);
        BTN_ASSERT(bgs_horizontal_stretch >= 0 && bgs_horizontal_stretch <= 1,
                   "Invalid bgs horizontal stretch: ", bgs_horizontal_stretch);
        BTN_ASSERT(bgs_vertical_stretch >= 0 && bgs_vertical_stretch <= 1,
                   "Invalid bgs vertical stretch: ", bgs_vertical_stretch);
    }

    [[nodiscard]] constexpr fixed sprites_horizontal_stretch() const
    {
        return _sprites_horizontal_stretch;
    }

    constexpr void set_sprites_horizontal_stretch(fixed sprites_horizontal_stretch)
    {
        BTN_ASSERT(sprites_horizontal_stretch >= 0 && sprites_horizontal_stretch <= 1,
                   "Invalid sprites horizontal stretch: ", sprites_horizontal_stretch);

        _sprites_horizontal_stretch = sprites_horizontal_stretch;
    }

    [[nodiscard]] constexpr fixed sprites_vertical_stretch() const
    {
        return _sprites_vertical_stretch;
    }

    constexpr void set_sprites_vertical_stretch(fixed sprites_vertical_stretch)
    {
        BTN_ASSERT(sprites_vertical_stretch >= 0 && sprites_vertical_stretch <= 1,
                   "Invalid sprites vertical stretch: ", sprites_vertical_stretch);

        _sprites_vertical_stretch = sprites_vertical_stretch;
    }

    constexpr void set_sprites_stretch(fixed sprites_stretch)
    {
        BTN_ASSERT(sprites_stretch >= 0 && sprites_stretch <= 1, "Invalid sprites stretch: ", sprites_stretch);

        _sprites_horizontal_stretch = sprites_stretch;
        _sprites_vertical_stretch = sprites_stretch;
    }

    [[nodiscard]] constexpr fixed bgs_horizontal_stretch() const
    {
        return _bgs_horizontal_stretch;
    }

    constexpr void set_bgs_horizontal_stretch(fixed bgs_horizontal_stretch)
    {
        BTN_ASSERT(bgs_horizontal_stretch >= 0 && bgs_horizontal_stretch <= 1,
                   "Invalid bgs horizontal stretch: ", bgs_horizontal_stretch);

        _bgs_horizontal_stretch = bgs_horizontal_stretch;
    }

    [[nodiscard]] constexpr fixed bgs_vertical_stretch() const
    {
        return _bgs_vertical_stretch;
    }

    constexpr void set_bgs_vertical_stretch(fixed bgs_vertical_stretch)
    {
        BTN_ASSERT(bgs_vertical_stretch >= 0 && bgs_vertical_stretch <= 1,
                   "Invalid bgs vertical stretch: ", bgs_vertical_stretch);

        _bgs_vertical_stretch = bgs_vertical_stretch;
    }

    constexpr void set_bgs_stretch(fixed bgs_stretch)
    {
        BTN_ASSERT(bgs_stretch >= 0 && bgs_stretch <= 1, "Invalid bgs stretch: ", bgs_stretch);

        _bgs_horizontal_stretch = bgs_stretch;
        _bgs_vertical_stretch = bgs_stretch;
    }

    constexpr void set_horizontal_stretch(fixed horizontal_stretch)
    {
        BTN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);

        _sprites_horizontal_stretch = horizontal_stretch;
        _bgs_horizontal_stretch = horizontal_stretch;
    }

    constexpr void set_vertical_stretch(fixed vertical_stretch)
    {
        BTN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _sprites_vertical_stretch = vertical_stretch;
        _bgs_vertical_stretch = vertical_stretch;
    }

    constexpr void set_stretch(fixed stretch)
    {
        BTN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

        _sprites_horizontal_stretch = stretch;
        _sprites_vertical_stretch = stretch;
        _bgs_horizontal_stretch = stretch;
        _bgs_vertical_stretch = stretch;
    }

    [[nodiscard]] constexpr friend bool operator==(const mosaic_attributes& a, const mosaic_attributes& b) = default;

private:
    fixed _sprites_horizontal_stretch;
    fixed _sprites_vertical_stretch;
    fixed _bgs_horizontal_stretch;
    fixed _bgs_vertical_stretch;
};

}

#endif


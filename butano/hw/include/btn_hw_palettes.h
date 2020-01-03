#ifndef BTN_HW_PALETTES_H
#define BTN_HW_PALETTES_H

#include "btn_common.h"

namespace btn
{

class color;

namespace hw::palettes
{
    [[nodiscard]] constexpr int count()
    {
        return 16;
    }

    [[nodiscard]] constexpr int colors_per_palette()
    {
        return 16;
    }

    [[nodiscard]] constexpr int colors()
    {
        return count() * colors_per_palette();
    }

    void brightness(int value, int count, color* colors_ptr);

    void contrast(int value, int count, color* colors_ptr);

    void intensity(int value, int count, color* colors_ptr);

    void inverse(int intensity, int count, color* colors_ptr);

    void grayscale(int intensity, int count, color* colors_ptr);

    void fade(color fade_color, int intensity, int count, color* colors_ptr);

    void rotate(int rotate_count, int colors_count, color* colors_ptr);

    void commit_sprites(const color* colors_ptr, int offset, int count);

    void commit_bgs(const color* colors_ptr, int offset, int count);
}
}

#endif

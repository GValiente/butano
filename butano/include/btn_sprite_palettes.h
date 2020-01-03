#ifndef BTN_SPRITE_PALETTES_H
#define BTN_SPRITE_PALETTES_H

#include "btn_fixed_fwd.h"

namespace btn
{

class color;

namespace sprite_palettes
{
    [[nodiscard]] fixed brightness();

    void set_brightness(fixed brightness);

    [[nodiscard]] fixed contrast();

    void set_contrast(fixed contrast);

    [[nodiscard]] fixed intensity();

    void set_intensity(fixed intensity);

    [[nodiscard]] fixed inverse_intensity();

    void set_inverse_intensity(fixed intensity);

    [[nodiscard]] fixed grayscale_intensity();

    void set_grayscale_intensity(fixed intensity);

    [[nodiscard]] color fade_color();

    [[nodiscard]] fixed fade_intensity();

    void set_fade(color color, fixed intensity);
}
}

#endif

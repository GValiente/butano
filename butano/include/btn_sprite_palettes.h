/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_PALETTES_H
#define BTN_SPRITE_PALETTES_H

#include "btn_fixed_fwd.h"

namespace btn
{

class color;

namespace sprite_palettes
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] fixed brightness();

    void set_brightness(fixed brightness);

    [[nodiscard]] fixed contrast();

    void set_contrast(fixed contrast);

    [[nodiscard]] fixed intensity();

    void set_intensity(fixed intensity);

    [[nodiscard]] bool inverted();

    void set_inverted(bool inverted);

    [[nodiscard]] fixed grayscale_intensity();

    void set_grayscale_intensity(fixed intensity);

    [[nodiscard]] color fade_color();

    void set_fade_color(color color);

    [[nodiscard]] fixed fade_intensity();

    void set_fade_intensity(fixed intensity);

    void set_fade(color color, fixed intensity);
}
}

#endif

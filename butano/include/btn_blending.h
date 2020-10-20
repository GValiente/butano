/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BLENDING_H
#define BTN_BLENDING_H

#include "btn_fixed_fwd.h"

namespace btn
{
    class blending_fade_alpha;
    class blending_transparency_attributes;
}

namespace btn::blending
{
    enum class fade_color_type
    {
        BLACK,
        WHITE
    };

    [[nodiscard]] fixed transparency_alpha();

    void set_transparency_alpha(fixed transparency_alpha);

    [[nodiscard]] fixed intensity_alpha();

    void set_intensity_alpha(fixed intensity_alpha);

    [[nodiscard]] blending_transparency_attributes transparency_attributes();

    void set_transparency_attributes(blending_transparency_attributes transparency_attributes);

    [[nodiscard]] fade_color_type fade_color();

    void set_fade_color(fade_color_type fade_color);

    [[nodiscard]] fixed fade_alpha();

    void set_fade_alpha(fixed fade_alpha);

    void set_fade_alpha(blending_fade_alpha fade_alpha);
}

#endif

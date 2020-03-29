#ifndef BTN_BG_PALETTES_H
#define BTN_BG_PALETTES_H

#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{

class color;

namespace bg_palettes
{
    [[nodiscard]] int used_count();

    [[nodiscard]] int available_count();

    [[nodiscard]] const optional<color>& transparent_color();

    void set_transparent_color(const optional<color>& transparent_color);

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

    [[nodiscard]] fixed fade_intensity();

    void set_fade(color color, fixed intensity);
}
}

#endif

#ifndef BTN_BLENDING_H
#define BTN_BLENDING_H

#include "btn_fixed_fwd.h"

namespace btn::blending
{
    [[nodiscard]] fixed transparency_alpha();

    void set_transparency_alpha(fixed transparency_alpha);

    [[nodiscard]] fixed intensity_alpha();

    void set_intensity_alpha(fixed intensity_alpha);
}

#endif

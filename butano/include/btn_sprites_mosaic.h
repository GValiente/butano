#ifndef BTN_SPRITES_MOSAIC_H
#define BTN_SPRITES_MOSAIC_H

#include "btn_fixed_fwd.h"

namespace btn::sprites_mosaic
{
    [[nodiscard]] fixed horizontal_stretch();

    void set_horizontal_stretch(fixed stretch);

    [[nodiscard]] fixed vertical_stretch();

    void set_vertical_stretch(fixed stretch);

    void set_stretch(fixed stretch);

    void set_stretch(fixed horizontal_stretch, fixed vertical_stretch);
}

#endif

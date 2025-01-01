/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bgs_mosaic.h"

#include "bn_assert.h"
#include "bn_display_manager.h"

namespace bn::bgs_mosaic
{

fixed horizontal_stretch()
{
    return display_manager::bgs_mosaic_horizontal_stretch();
}

void set_horizontal_stretch(fixed horizontal_stretch)
{
    BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1, "Invalid horizontal stretch: ", horizontal_stretch);

    display_manager::set_bgs_mosaic_horizontal_stretch(horizontal_stretch);
}

fixed vertical_stretch()
{
    return display_manager::bgs_mosaic_vertical_stretch();
}

void set_vertical_stretch(fixed vertical_stretch)
{
    BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

    display_manager::set_bgs_mosaic_vertical_stretch(vertical_stretch);
}

void set_stretch(fixed stretch)
{
    BN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

    display_manager::set_bgs_mosaic_stretch(stretch);
}

void set_stretch(fixed horizontal_stretch, fixed vertical_stretch)
{
    BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1, "Invalid horizontal stretch: ", horizontal_stretch);
    BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

    display_manager::set_bgs_mosaic_stretch(horizontal_stretch, vertical_stretch);
}

}

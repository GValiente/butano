#include "btn_bgs_mosaic.h"

#include "btn_fixed.h"
#include "btn_display_manager.h"

namespace btn::bgs_mosaic
{

fixed horizontal_stretch()
{
    return display_manager::bgs_mosaic_horizontal_stretch();
}

void set_horizontal_stretch(fixed stretch)
{
    BTN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

    display_manager::set_bgs_mosaic_horizontal_stretch(stretch);
}

fixed vertical_stretch()
{
    return display_manager::bgs_mosaic_vertical_stretch();
}

void set_vertical_stretch(fixed stretch)
{
    BTN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

    display_manager::set_bgs_mosaic_vertical_stretch(stretch);
}

}

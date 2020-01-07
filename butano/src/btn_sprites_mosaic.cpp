#include "btn_sprites_mosaic.h"

#include "btn_fixed.h"
#include "btn_display_manager.h"

namespace btn::sprites_mosaic
{

fixed horizontal_stretch()
{
    return display_manager::sprites_mosaic_horizontal_stretch();
}

void set_horizontal_stretch(fixed stretch)
{
    BTN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

    display_manager::set_sprites_mosaic_horizontal_stretch(stretch);
}

fixed vertical_stretch()
{
    return display_manager::sprites_mosaic_vertical_stretch();
}

void set_vertical_stretch(fixed stretch)
{
    BTN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

    display_manager::set_sprites_mosaic_vertical_stretch(stretch);
}

}

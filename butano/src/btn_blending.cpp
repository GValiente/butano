#include "btn_blending.h"

#include "btn_fixed.h"
#include "btn_regular_bg_ptr.h"
#include "btn_display_manager.h"

namespace btn::blending
{

fixed transparency_alpha()
{
    return display_manager::blending_transparency_alpha();
}

void set_transparency_alpha(fixed transparency_alpha)
{
    BTN_ASSERT(transparency_alpha >= 0 && transparency_alpha <= 1, "Invalid transparency alpha: ", transparency_alpha);

    display_manager::set_blending_transparency_alpha(transparency_alpha);
}

fixed intensity_alpha()
{
    return display_manager::blending_intensity_alpha();
}

void set_intensity_alpha(fixed intensity_alpha)
{
    BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);

    display_manager::set_blending_intensity_alpha(intensity_alpha);
}

}

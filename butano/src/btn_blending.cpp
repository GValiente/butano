#include "btn_blending.h"

#include "btn_fixed.h"
#include "btn_blending_fade_alpha.h"
#include "btn_blending_transparency_attributes.h"
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
    BTN_ASSERT(transparency_alpha == 1 || ! display_manager::blending_fade_enabled(),
               "Blending transparency and fade can't be enabled at the same time: ",
               transparency_alpha, " - ", display_manager::blending_fade_alpha());

    display_manager::set_blending_transparency_alpha(transparency_alpha);
}

fixed intensity_alpha()
{
    return display_manager::blending_intensity_alpha();
}

void set_intensity_alpha(fixed intensity_alpha)
{
    BTN_ASSERT(intensity_alpha >= 0 && intensity_alpha <= 1, "Invalid intensity alpha: ", intensity_alpha);
    BTN_ASSERT(intensity_alpha == 0 || ! display_manager::blending_fade_enabled(),
               "Blending intensity and fade can't be enabled at the same time: ",
               intensity_alpha, " - ", display_manager::blending_fade_alpha());

    display_manager::set_blending_intensity_alpha(intensity_alpha);
}

blending_transparency_attributes transparency_attributes()
{
    return blending_transparency_attributes(display_manager::blending_transparency_alpha(),
                                            display_manager::blending_intensity_alpha());
}

void set_transparency_attributes(blending_transparency_attributes transparency_attributes)
{
    BTN_ASSERT(transparency_attributes.transparency_alpha() == 1 || ! display_manager::blending_fade_enabled(),
               "Blending transparency and fade can't be enabled at the same time: ",
               transparency_attributes.transparency_alpha(), " - ", display_manager::blending_fade_alpha());
    BTN_ASSERT(transparency_attributes.intensity_alpha() == 0 || ! display_manager::blending_fade_enabled(),
               "Blending intensity and fade can't be enabled at the same time: ",
               transparency_attributes.intensity_alpha(), " - ", display_manager::blending_fade_alpha());

    display_manager::set_blending_transparency_alpha(transparency_attributes.transparency_alpha());
    display_manager::set_blending_intensity_alpha(transparency_attributes.intensity_alpha());
}

fade_color_type fade_color()
{
    return display_manager::blending_fade_to_black() ? fade_color_type::BLACK : fade_color_type::WHITE;
}

void set_fade_color(fade_color_type fade_color)
{
    display_manager::set_blending_fade_to_black(fade_color == fade_color_type::BLACK);
}

fixed fade_alpha()
{
    return display_manager::blending_fade_alpha();
}

void set_fade_alpha(fixed fade_alpha)
{
    BTN_ASSERT(fade_alpha >= 0 && fade_alpha <= 1, "Invalid fade alpha: ", fade_alpha);

    display_manager::set_blending_fade_alpha(fade_alpha);

    BTN_ASSERT(! display_manager::blending_fade_enabled() || display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time: ",
               fade_alpha, " - ", display_manager::blending_transparency_alpha());
    BTN_ASSERT(! display_manager::blending_fade_enabled() || display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time: ",
               fade_alpha, " - ", display_manager::blending_intensity_alpha());
}

void set_fade_alpha(blending_fade_alpha fade_alpha)
{
    display_manager::set_blending_fade_alpha(fade_alpha.value());

    BTN_ASSERT(! display_manager::blending_fade_enabled() || display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time: ",
               fade_alpha.value(), " - ", display_manager::blending_transparency_alpha());
    BTN_ASSERT(! display_manager::blending_fade_enabled() || display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time: ",
               fade_alpha.value(), " - ", display_manager::blending_intensity_alpha());
}

}

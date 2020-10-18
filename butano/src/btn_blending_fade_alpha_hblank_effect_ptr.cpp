#include "btn_blending_fade_alpha_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_display_manager.h"
#include "btn_blending_fade_alpha.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

blending_fade_alpha_hblank_effect_ptr blending_fade_alpha_hblank_effect_ptr::create(
        const span<const blending_fade_alpha>& values_ref)
{
    BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time");
    BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), 0,
                                            hblank_effects_manager::handler_type::BLENDING_FADE_ALPHA);
    return blending_fade_alpha_hblank_effect_ptr(id);
}

optional<blending_fade_alpha_hblank_effect_ptr> blending_fade_alpha_hblank_effect_ptr::create_optional(
        const span<const blending_fade_alpha>& values_ref)
{
    BTN_ASSERT(display_manager::blending_transparency_alpha() == 1,
               "Blending transparency and fade can't be enabled at the same time");
    BTN_ASSERT(display_manager::blending_intensity_alpha() == 0,
               "Blending intensity and fade can't be enabled at the same time");

    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::BLENDING_FADE_ALPHA);
    optional<blending_fade_alpha_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = blending_fade_alpha_hblank_effect_ptr(id);
    }

    return result;
}

span<const blending_fade_alpha> blending_fade_alpha_hblank_effect_ptr::values_ref() const
{
    auto values_ptr = reinterpret_cast<const blending_fade_alpha*>(hblank_effects_manager::values_ref(id()));
    return span<const blending_fade_alpha>(values_ptr, display::height());
}

void blending_fade_alpha_hblank_effect_ptr::set_values_ref(const span<const blending_fade_alpha>& values_ref)
{
    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void blending_fade_alpha_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

blending_fade_alpha_hblank_effect_ptr::blending_fade_alpha_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}

#include "btn_bg_palettes_hblank_effects.h"

#include "btn_span.h"
#include "btn_color.h"
#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

bg_palettes_transparent_color_hblank_effect_ptr bg_palettes_transparent_color_hblank_effect_ptr::create(
        const span<const color>& colors_ref)
{
    auto target_type = hblank_effects_manager::target_type::BG_PALETTES_TRANSPARENT_COLOR;
    int id = hblank_effects_manager::create(colors_ref, target_type, 0);
    return bg_palettes_transparent_color_hblank_effect_ptr(id);
}

optional<bg_palettes_transparent_color_hblank_effect_ptr> bg_palettes_transparent_color_hblank_effect_ptr::optional_create(
        const span<const color>& colors_ref)
{
    auto target_type = hblank_effects_manager::target_type::BG_PALETTES_TRANSPARENT_COLOR;
    int id = hblank_effects_manager::optional_create(colors_ref, target_type, 0);
    optional<bg_palettes_transparent_color_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = bg_palettes_transparent_color_hblank_effect_ptr(id);
    }

    return result;
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(
        const bg_palettes_transparent_color_hblank_effect_ptr& other) :
    hblank_effect_ptr(other)
{
}

bg_palettes_transparent_color_hblank_effect_ptr& bg_palettes_transparent_color_hblank_effect_ptr::operator=(
        const bg_palettes_transparent_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    return *this;
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(
        bg_palettes_transparent_color_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other))
{
}

bg_palettes_transparent_color_hblank_effect_ptr& bg_palettes_transparent_color_hblank_effect_ptr::operator=(
        bg_palettes_transparent_color_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const color> bg_palettes_transparent_color_hblank_effect_ptr::colors_ref() const
{
    return hblank_effects_manager::colors_ref(id());
}

void bg_palettes_transparent_color_hblank_effect_ptr::set_colors_ref(
        const span<const color>& colors_ref)
{
    hblank_effects_manager::set_values_ref(id(), colors_ref.data(), colors_ref.size());
}

void bg_palettes_transparent_color_hblank_effect_ptr::reload_colors_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void bg_palettes_transparent_color_hblank_effect_ptr::swap(bg_palettes_transparent_color_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
}

bg_palettes_transparent_color_hblank_effect_ptr::bg_palettes_transparent_color_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}

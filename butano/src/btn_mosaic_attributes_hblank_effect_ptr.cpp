#include "btn_mosaic_attributes_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_mosaic_attributes.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

mosaic_attributes_hblank_effect_ptr mosaic_attributes_hblank_effect_ptr::create(
        const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), 0,
                                            hblank_effects_manager::handler_type::MOSAIC_ATTRIBUTES);
    return mosaic_attributes_hblank_effect_ptr(id);
}

optional<mosaic_attributes_hblank_effect_ptr> mosaic_attributes_hblank_effect_ptr::create_optional(
        const span<const mosaic_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), 0,
                                                     hblank_effects_manager::handler_type::MOSAIC_ATTRIBUTES);
    optional<mosaic_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = mosaic_attributes_hblank_effect_ptr(id);
    }

    return result;
}

span<const mosaic_attributes> mosaic_attributes_hblank_effect_ptr::attributes_ref() const
{
    auto values_ptr = reinterpret_cast<const mosaic_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const mosaic_attributes>(values_ptr, display::height());
}

void mosaic_attributes_hblank_effect_ptr::set_attributes_ref(const span<const mosaic_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void mosaic_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

mosaic_attributes_hblank_effect_ptr::mosaic_attributes_hblank_effect_ptr(int id) :
    hblank_effect_ptr(id)
{
}

}

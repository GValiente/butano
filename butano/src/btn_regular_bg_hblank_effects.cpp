#include "btn_regular_bg_hblank_effects.h"

#include "btn_optional.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

namespace
{
    hblank_effects_manager::target_type manager_target_type(regular_bg_hblank_effect_type type)
    {
        switch(type)
        {

        case regular_bg_hblank_effect_type::HORIZONTAL_POSITION_ABSOLUTE:
            return hblank_effects_manager::target_type::REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE;

        case regular_bg_hblank_effect_type::HORIZONTAL_POSITION_RELATIVE:
            return hblank_effects_manager::target_type::REGULAR_BG_HORIZONTAL_POSITION_RELATIVE;
        }

        BTN_ERROR("Invalid type: ", int(type));
        return hblank_effects_manager::target_type::REGULAR_BG_HORIZONTAL_POSITION_ABSOLUTE;
    }
}

regular_bg_hblank_effect_ptr regular_bg_hblank_effect_ptr::create(
        regular_bg_ptr bg_ptr, regular_bg_hblank_effect_type type, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref, manager_target_type(type), bg_ptr.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return regular_bg_hblank_effect_ptr(id, move(bg_ptr));
}

optional<regular_bg_hblank_effect_ptr> regular_bg_hblank_effect_ptr::optional_create(
        regular_bg_ptr bg_ptr, regular_bg_hblank_effect_type type, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref, manager_target_type(type), bg_ptr.id());
    optional<regular_bg_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_hblank_effect_ptr(id, move(bg_ptr));
    }

    return result;
}

regular_bg_hblank_effect_ptr::regular_bg_hblank_effect_ptr(int id, regular_bg_ptr&& bg_ptr) :
    hblank_effect_ptr(id),
    _bg_ptr(move(bg_ptr))
{
}

}

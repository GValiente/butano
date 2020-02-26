#include "btn_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

hblank_effect_ptr::hblank_effect_ptr(const hblank_effect_ptr& other) :
    hblank_effect_ptr(other._id)
{
    hblank_effects_manager::increase_usages(_id);
}

hblank_effect_ptr& hblank_effect_ptr::operator=(const hblank_effect_ptr& other)
{
    if(_id != other._id)
    {
        if(_id >= 0)
        {
            hblank_effects_manager::decrease_usages(_id);
        }

        _id = other._id;
        hblank_effects_manager::increase_usages(_id);
    }

    return *this;
}

const span<const int16_t>& hblank_effect_ptr::values_ref() const
{
    return hblank_effects_manager::values_ref(_id);
}

void hblank_effect_ptr::set_values_ref(const span<const int16_t>& values_ref)
{
    hblank_effects_manager::set_values_ref(_id, values_ref);
}

void hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(_id);
}

void hblank_effect_ptr::_destroy()
{
    hblank_effects_manager::decrease_usages(_id);
}

}

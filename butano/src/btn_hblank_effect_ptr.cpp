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

[[nodiscard]] bool hblank_effect_ptr::visible() const
{
    return hblank_effects_manager::visible(_id);
}

void hblank_effect_ptr::set_visible(bool visible)
{
    hblank_effects_manager::set_visible(_id, visible);
}

void hblank_effect_ptr::_destroy()
{
    hblank_effects_manager::decrease_usages(_id);
}

}

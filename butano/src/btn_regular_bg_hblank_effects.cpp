#include "btn_regular_bg_hblank_effects.h"

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_regular_bg_attributes.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

regular_bg_position_hblank_effect_ptr regular_bg_position_hblank_effect_ptr::create_horizontal(
        regular_bg_ptr bg_ptr, const span<const fixed>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::REGULAR_BG_HORIZONTAL_POSITION;
    int id = hblank_effects_manager::create(deltas_ref, target_type, bg_ptr.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return regular_bg_position_hblank_effect_ptr(id, move(bg_ptr));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::optional_create_horizontal(
        regular_bg_ptr bg_ptr, const span<const fixed>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::REGULAR_BG_HORIZONTAL_POSITION;
    int id = hblank_effects_manager::create(deltas_ref, target_type, bg_ptr.id());
    optional<regular_bg_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hblank_effect_ptr(id, move(bg_ptr));
    }

    return result;
}

regular_bg_position_hblank_effect_ptr regular_bg_position_hblank_effect_ptr::create_vertical(
        regular_bg_ptr bg_ptr, const span<const fixed>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::REGULAR_BG_VERTICAL_POSITION;
    int id = hblank_effects_manager::create(deltas_ref, target_type, bg_ptr.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return regular_bg_position_hblank_effect_ptr(id, move(bg_ptr));
}

optional<regular_bg_position_hblank_effect_ptr> regular_bg_position_hblank_effect_ptr::optional_create_vertical(
        regular_bg_ptr bg_ptr, const span<const fixed>& deltas_ref)
{
    auto target_type = hblank_effects_manager::target_type::REGULAR_BG_VERTICAL_POSITION;
    int id = hblank_effects_manager::create(deltas_ref, target_type, bg_ptr.id());
    optional<regular_bg_position_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_position_hblank_effect_ptr(id, move(bg_ptr));
    }

    return result;
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(
        const regular_bg_position_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _bg_ptr(other._bg_ptr)
{
}

regular_bg_position_hblank_effect_ptr& regular_bg_position_hblank_effect_ptr::operator=(
        const regular_bg_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _bg_ptr = other._bg_ptr;
    return *this;
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(
        regular_bg_position_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _bg_ptr(move(other._bg_ptr))
{
}

regular_bg_position_hblank_effect_ptr& regular_bg_position_hblank_effect_ptr::operator=(
        regular_bg_position_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _bg_ptr = move(other._bg_ptr);
    return *this;
}

span<const fixed> regular_bg_position_hblank_effect_ptr::deltas_ref() const
{
    return hblank_effects_manager::fixed_values_ref(id());
}

void regular_bg_position_hblank_effect_ptr::set_deltas_ref(const span<const fixed>& deltas_ref)
{
    hblank_effects_manager::set_values_ref(id(), deltas_ref.data(), deltas_ref.size());
}

void regular_bg_position_hblank_effect_ptr::reload_deltas_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_position_hblank_effect_ptr::swap(regular_bg_position_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_bg_ptr, other._bg_ptr);
}

regular_bg_position_hblank_effect_ptr::regular_bg_position_hblank_effect_ptr(int id, regular_bg_ptr&& bg_ptr) :
    hblank_effect_ptr(id),
    _bg_ptr(move(bg_ptr))
{
}

regular_bg_attributes_hblank_effect_ptr regular_bg_attributes_hblank_effect_ptr::create(
        regular_bg_ptr bg_ptr, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref, bg_ptr.id());
    BTN_ASSERT(id >= 0, "HBlank effect create failed");

    return regular_bg_attributes_hblank_effect_ptr(id, move(bg_ptr));
}

optional<regular_bg_attributes_hblank_effect_ptr> regular_bg_attributes_hblank_effect_ptr::optional_create(
        regular_bg_ptr bg_ptr, const span<const regular_bg_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref, bg_ptr.id());
    optional<regular_bg_attributes_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = regular_bg_attributes_hblank_effect_ptr(id, move(bg_ptr));
    }

    return result;
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(
        const regular_bg_attributes_hblank_effect_ptr& other) :
    hblank_effect_ptr(other),
    _bg_ptr(other._bg_ptr)
{
}

regular_bg_attributes_hblank_effect_ptr& regular_bg_attributes_hblank_effect_ptr::operator=(
        const regular_bg_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::operator=(other);
    _bg_ptr = other._bg_ptr;
    return *this;
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(
        regular_bg_attributes_hblank_effect_ptr&& other) :
    hblank_effect_ptr(move(other)),
    _bg_ptr(move(other._bg_ptr))
{
}

regular_bg_attributes_hblank_effect_ptr& regular_bg_attributes_hblank_effect_ptr::operator=(
        regular_bg_attributes_hblank_effect_ptr&& other)
{
    hblank_effect_ptr::operator=(move(other));
    _bg_ptr = move(other._bg_ptr);
    return *this;
}

span<const regular_bg_attributes> regular_bg_attributes_hblank_effect_ptr::attributes_ref() const
{
    return hblank_effects_manager::regular_bg_attributes_ref(id());
}

void regular_bg_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const regular_bg_attributes>& attributes_ref)
{
    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void regular_bg_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void regular_bg_attributes_hblank_effect_ptr::swap(regular_bg_attributes_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_bg_ptr, other._bg_ptr);
}

regular_bg_attributes_hblank_effect_ptr::regular_bg_attributes_hblank_effect_ptr(int id, regular_bg_ptr&& bg_ptr) :
    hblank_effect_ptr(id),
    _bg_ptr(move(bg_ptr))
{
}

}

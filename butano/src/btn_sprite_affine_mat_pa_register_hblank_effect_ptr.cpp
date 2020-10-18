#include "btn_sprite_affine_mat_pa_register_hblank_effect_ptr.h"

#include "btn_span.h"
#include "btn_display.h"
#include "btn_optional.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "btn_hblank_effects_manager.h"

namespace btn
{

sprite_affine_mat_pa_register_hblank_effect_ptr sprite_affine_mat_pa_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                            hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES);
    return sprite_affine_mat_pa_register_hblank_effect_ptr(id, true, move(affine_mat));
}

sprite_affine_mat_pa_register_hblank_effect_ptr sprite_affine_mat_pa_register_hblank_effect_ptr::create(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create(values_ref.data(), values_ref.size(), affine_mat.id(),
                                            hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES);
    return sprite_affine_mat_pa_register_hblank_effect_ptr(id, false, move(affine_mat));
}

optional<sprite_affine_mat_pa_register_hblank_effect_ptr> sprite_affine_mat_pa_register_hblank_effect_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    int id = hblank_effects_manager::create_optional(attributes_ref.data(), attributes_ref.size(), affine_mat.id(),
                                                     hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_ATTRIBUTES);
    optional<sprite_affine_mat_pa_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pa_register_hblank_effect_ptr(id, true, move(affine_mat));
    }

    return result;
}

optional<sprite_affine_mat_pa_register_hblank_effect_ptr> sprite_affine_mat_pa_register_hblank_effect_ptr::create_optional(
        sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref)
{
    int id = hblank_effects_manager::create_optional(values_ref.data(), values_ref.size(), affine_mat.id(),
                                                     hblank_effects_manager::handler_type::SPRITE_AFFINE_MAT_PA_REGISTER_VALUES);
    optional<sprite_affine_mat_pa_register_hblank_effect_ptr> result;

    if(id >= 0)
    {
        result = sprite_affine_mat_pa_register_hblank_effect_ptr(id, false, move(affine_mat));
    }

    return result;
}

sprite_affine_mat_pa_register_hblank_effect_ptr::sprite_affine_mat_pa_register_hblank_effect_ptr(
        sprite_affine_mat_pa_register_hblank_effect_ptr&& other) noexcept :
    hblank_effect_ptr(move(other)),
    _from_attributes(other._from_attributes),
    _affine_mat(move(other._affine_mat))
{
}

sprite_affine_mat_pa_register_hblank_effect_ptr& sprite_affine_mat_pa_register_hblank_effect_ptr::operator=(
        sprite_affine_mat_pa_register_hblank_effect_ptr&& other) noexcept
{
    _from_attributes = other._from_attributes;
    _affine_mat = move(other._affine_mat);
    hblank_effect_ptr::operator=(move(other));
    return *this;
}

span<const sprite_affine_mat_attributes> sprite_affine_mat_pa_register_hblank_effect_ptr::attributes_ref() const
{
    BTN_ASSERT(_from_attributes, "Built from values");

    auto attributes_ptr = reinterpret_cast<const sprite_affine_mat_attributes*>(hblank_effects_manager::values_ref(id()));
    return span<const sprite_affine_mat_attributes>(attributes_ptr, display::height());
}

span<const int16_t> sprite_affine_mat_pa_register_hblank_effect_ptr::values_ref() const
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    auto values_ptr = reinterpret_cast<const int16_t*>(hblank_effects_manager::values_ref(id()));
    return span<const int16_t>(values_ptr, display::height());
}

void sprite_affine_mat_pa_register_hblank_effect_ptr::set_attributes_ref(
        const span<const sprite_affine_mat_attributes>& attributes_ref)
{
    BTN_ASSERT(_from_attributes, "Built from values");

    hblank_effects_manager::set_values_ref(id(), attributes_ref.data(), attributes_ref.size());
}

void sprite_affine_mat_pa_register_hblank_effect_ptr::set_values_ref(
        const span<const int16_t>& values_ref)
{
    BTN_ASSERT(! _from_attributes, "Built from attributes");

    hblank_effects_manager::set_values_ref(id(), values_ref.data(), values_ref.size());
}

void sprite_affine_mat_pa_register_hblank_effect_ptr::reload_attributes_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pa_register_hblank_effect_ptr::reload_values_ref()
{
    hblank_effects_manager::reload_values_ref(id());
}

void sprite_affine_mat_pa_register_hblank_effect_ptr::swap(
        sprite_affine_mat_pa_register_hblank_effect_ptr& other)
{
    hblank_effect_ptr::swap(other);
    btn::swap(_from_attributes, other._from_attributes);
    btn::swap(_affine_mat, other._affine_mat);
}

sprite_affine_mat_pa_register_hblank_effect_ptr::sprite_affine_mat_pa_register_hblank_effect_ptr(
        int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat) :
    hblank_effect_ptr(id),
    _from_attributes(from_attributes),
    _affine_mat(move(affine_mat))
{
}

}

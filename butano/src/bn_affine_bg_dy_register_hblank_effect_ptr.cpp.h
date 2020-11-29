/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_dy_register_hblank_effect_ptr.h"

#include "bn_span.h"
#include "bn_display.h"
#include "bn_optional.h"
#include "bn_affine_bg_mat_attributes.h"
#include "bn_hblank_effects_manager.h"

namespace bn
{

affine_bg_dy_register_hblank_effect_ptr affine_bg_dy_register_hblank_effect_ptr::create(
        affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    return affine_bg_dy_register_hblank_effect_ptr(
                affine_bg_dy_high_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_dy_low_register_hblank_effect_ptr::create(bg, attributes_ref));
}

affine_bg_dy_register_hblank_effect_ptr affine_bg_dy_register_hblank_effect_ptr::create(
        affine_bg_ptr bg, const span<const int>& values_ref)
{
    return affine_bg_dy_register_hblank_effect_ptr(
                affine_bg_dy_high_register_hblank_effect_ptr::create(bg, values_ref),
                affine_bg_dy_low_register_hblank_effect_ptr::create(bg, values_ref));
}

optional<affine_bg_dy_register_hblank_effect_ptr> affine_bg_dy_register_hblank_effect_ptr::create_optional(
        affine_bg_ptr bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    optional<affine_bg_dy_register_hblank_effect_ptr> result;

    if(auto high = affine_bg_dy_high_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
    {
        if(auto low = affine_bg_dy_low_register_hblank_effect_ptr::create_optional(move(bg), attributes_ref))
        {
            result = affine_bg_dy_register_hblank_effect_ptr(move(*high), move(*low));
        }
    }

    return result;
}

optional<affine_bg_dy_register_hblank_effect_ptr> affine_bg_dy_register_hblank_effect_ptr::create_optional(
        affine_bg_ptr bg, const span<const int>& values_ref)
{
    optional<affine_bg_dy_register_hblank_effect_ptr> result;

    if(auto high = affine_bg_dy_high_register_hblank_effect_ptr::create_optional(bg, values_ref))
    {
        if(auto low = affine_bg_dy_low_register_hblank_effect_ptr::create_optional(move(bg), values_ref))
        {
            result = affine_bg_dy_register_hblank_effect_ptr(move(*high), move(*low));
        }
    }

    return result;
}

void affine_bg_dy_register_hblank_effect_ptr::set_visible(bool visible)
{
    _high_hblank_effect_ptr.set_visible(visible);
    _low_hblank_effect_ptr.set_visible(visible);
}

span<const affine_bg_mat_attributes> affine_bg_dy_register_hblank_effect_ptr::attributes_ref() const
{
    return _high_hblank_effect_ptr.attributes_ref();
}

span<const int> affine_bg_dy_register_hblank_effect_ptr::values_ref() const
{
    return _high_hblank_effect_ptr.values_ref();
}

void affine_bg_dy_register_hblank_effect_ptr::set_attributes_ref(
        const span<const affine_bg_mat_attributes>& attributes_ref)
{
    _high_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _low_hblank_effect_ptr.set_attributes_ref(attributes_ref);
}

void affine_bg_dy_register_hblank_effect_ptr::set_values_ref(const span<const int>& values_ref)
{
    _high_hblank_effect_ptr.set_values_ref(values_ref);
    _low_hblank_effect_ptr.set_values_ref(values_ref);
}

void affine_bg_dy_register_hblank_effect_ptr::reload_attributes_ref()
{
    _high_hblank_effect_ptr.reload_attributes_ref();
    _low_hblank_effect_ptr.reload_attributes_ref();
}

void affine_bg_dy_register_hblank_effect_ptr::reload_values_ref()
{
    _high_hblank_effect_ptr.reload_values_ref();
    _low_hblank_effect_ptr.reload_values_ref();
}

void affine_bg_dy_register_hblank_effect_ptr::swap(affine_bg_dy_register_hblank_effect_ptr& other)
{
    _high_hblank_effect_ptr.swap(other._high_hblank_effect_ptr);
    _low_hblank_effect_ptr.swap(other._low_hblank_effect_ptr);
}

affine_bg_dy_register_hblank_effect_ptr::affine_bg_dy_register_hblank_effect_ptr(
        affine_bg_dy_high_register_hblank_effect_ptr&& high_hblank_effect_ptr,
        affine_bg_dy_low_register_hblank_effect_ptr&& low_hblank_effect_ptr) :
    _high_hblank_effect_ptr(move(high_hblank_effect_ptr)),
    _low_hblank_effect_ptr(move(low_hblank_effect_ptr))
{
}

}

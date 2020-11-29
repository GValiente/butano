/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_mat_attributes_hblank_effect_ptr.h"

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

affine_bg_mat_attributes_hblank_effect_ptr affine_bg_mat_attributes_hblank_effect_ptr::create(
        const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    return affine_bg_mat_attributes_hblank_effect_ptr(
                affine_bg_pa_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_pb_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_pc_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_pd_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_dx_register_hblank_effect_ptr::create(bg, attributes_ref),
                affine_bg_dy_register_hblank_effect_ptr::create(bg, attributes_ref));
}

optional<affine_bg_mat_attributes_hblank_effect_ptr> affine_bg_mat_attributes_hblank_effect_ptr::create_optional(
        const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    optional<affine_bg_mat_attributes_hblank_effect_ptr> result;

    if(auto pa = affine_bg_pa_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
    {
        if(auto pb = affine_bg_pb_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
        {
            if(auto pc = affine_bg_pc_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
            {
                if(auto pd = affine_bg_pd_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
                {
                    if(auto dx = affine_bg_dx_register_hblank_effect_ptr::create_optional(bg, attributes_ref))
                    {
                        if(auto dy = affine_bg_dy_register_hblank_effect_ptr::create_optional(move(bg), attributes_ref))
                        {
                            result = affine_bg_mat_attributes_hblank_effect_ptr(
                                        move(*pa), move(*pb), move(*pc), move(*pd), move(*dx), move(*dy));
                        }
                    }
                }
            }
        }
    }

    return result;
}

void affine_bg_mat_attributes_hblank_effect_ptr::set_visible(bool visible)
{
    _pa_hblank_effect_ptr.set_visible(visible);
    _pb_hblank_effect_ptr.set_visible(visible);
    _pc_hblank_effect_ptr.set_visible(visible);
    _pd_hblank_effect_ptr.set_visible(visible);
    _dx_hblank_effect_ptr.set_visible(visible);
    _dy_hblank_effect_ptr.set_visible(visible);
}

span<const affine_bg_mat_attributes> affine_bg_mat_attributes_hblank_effect_ptr::attributes_ref() const
{
    return _pa_hblank_effect_ptr.attributes_ref();
}

void affine_bg_mat_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const affine_bg_mat_attributes>& attributes_ref)
{
    _pa_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pb_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pc_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pd_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _dx_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _dy_hblank_effect_ptr.set_attributes_ref(attributes_ref);
}

void affine_bg_mat_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    _pa_hblank_effect_ptr.reload_attributes_ref();
    _pb_hblank_effect_ptr.reload_attributes_ref();
    _pc_hblank_effect_ptr.reload_attributes_ref();
    _pd_hblank_effect_ptr.reload_attributes_ref();
    _dx_hblank_effect_ptr.reload_attributes_ref();
    _dy_hblank_effect_ptr.reload_attributes_ref();
}

void affine_bg_mat_attributes_hblank_effect_ptr::swap(affine_bg_mat_attributes_hblank_effect_ptr& other)
{
    _pa_hblank_effect_ptr.swap(other._pa_hblank_effect_ptr);
    _pb_hblank_effect_ptr.swap(other._pb_hblank_effect_ptr);
    _pc_hblank_effect_ptr.swap(other._pc_hblank_effect_ptr);
    _pd_hblank_effect_ptr.swap(other._pd_hblank_effect_ptr);
    _dx_hblank_effect_ptr.swap(other._dx_hblank_effect_ptr);
    _dy_hblank_effect_ptr.swap(other._dy_hblank_effect_ptr);
}

affine_bg_mat_attributes_hblank_effect_ptr::affine_bg_mat_attributes_hblank_effect_ptr(
        affine_bg_pa_register_hblank_effect_ptr&& pa_hblank_effect_ptr,
        affine_bg_pb_register_hblank_effect_ptr&& pb_hblank_effect_ptr,
        affine_bg_pc_register_hblank_effect_ptr&& pc_hblank_effect_ptr,
        affine_bg_pd_register_hblank_effect_ptr&& pd_hblank_effect_ptr,
        affine_bg_dx_register_hblank_effect_ptr&& dx_hblank_effect_ptr,
        affine_bg_dy_register_hblank_effect_ptr&& dy_hblank_effect_ptr) :
    _pa_hblank_effect_ptr(move(pa_hblank_effect_ptr)),
    _pb_hblank_effect_ptr(move(pb_hblank_effect_ptr)),
    _pc_hblank_effect_ptr(move(pc_hblank_effect_ptr)),
    _pd_hblank_effect_ptr(move(pd_hblank_effect_ptr)),
    _dx_hblank_effect_ptr(move(dx_hblank_effect_ptr)),
    _dy_hblank_effect_ptr(move(dy_hblank_effect_ptr))
{
}

}

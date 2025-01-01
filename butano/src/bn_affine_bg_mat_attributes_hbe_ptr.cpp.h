/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_affine_bg_mat_attributes_hbe_ptr.h"

namespace bn
{

affine_bg_mat_attributes_hbe_ptr affine_bg_mat_attributes_hbe_ptr::create(
        const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    return affine_bg_mat_attributes_hbe_ptr(
                affine_bg_pa_register_hbe_ptr::create(bg, attributes_ref),
                affine_bg_pb_register_hbe_ptr::create(bg, attributes_ref),
                affine_bg_pc_register_hbe_ptr::create(bg, attributes_ref),
                affine_bg_pd_register_hbe_ptr::create(bg, attributes_ref),
                affine_bg_dx_register_hbe_ptr::create(bg, attributes_ref),
                affine_bg_dy_register_hbe_ptr::create(bg, attributes_ref));
}

optional<affine_bg_mat_attributes_hbe_ptr> affine_bg_mat_attributes_hbe_ptr::create_optional(
        const affine_bg_ptr& bg, const span<const affine_bg_mat_attributes>& attributes_ref)
{
    optional<affine_bg_mat_attributes_hbe_ptr> result;

    if(auto pa = affine_bg_pa_register_hbe_ptr::create_optional(bg, attributes_ref))
    {
        if(auto pb = affine_bg_pb_register_hbe_ptr::create_optional(bg, attributes_ref))
        {
            if(auto pc = affine_bg_pc_register_hbe_ptr::create_optional(bg, attributes_ref))
            {
                if(auto pd = affine_bg_pd_register_hbe_ptr::create_optional(bg, attributes_ref))
                {
                    if(auto dx = affine_bg_dx_register_hbe_ptr::create_optional(bg, attributes_ref))
                    {
                        if(auto dy = affine_bg_dy_register_hbe_ptr::create_optional(bg, attributes_ref))
                        {
                            result = affine_bg_mat_attributes_hbe_ptr(
                                        move(*pa), move(*pb), move(*pc), move(*pd), move(*dx), move(*dy));
                        }
                    }
                }
            }
        }
    }

    return result;
}

void affine_bg_mat_attributes_hbe_ptr::set_visible(bool visible)
{
    _pa_hbe_ptr.set_visible(visible);
    _pb_hbe_ptr.set_visible(visible);
    _pc_hbe_ptr.set_visible(visible);
    _pd_hbe_ptr.set_visible(visible);
    _dx_hbe_ptr.set_visible(visible);
    _dy_hbe_ptr.set_visible(visible);
}

span<const affine_bg_mat_attributes> affine_bg_mat_attributes_hbe_ptr::attributes_ref() const
{
    return _pa_hbe_ptr.attributes_ref();
}

void affine_bg_mat_attributes_hbe_ptr::set_attributes_ref(const span<const affine_bg_mat_attributes>& attributes_ref)
{
    _pa_hbe_ptr.set_attributes_ref(attributes_ref);
    _pb_hbe_ptr.set_attributes_ref(attributes_ref);
    _pc_hbe_ptr.set_attributes_ref(attributes_ref);
    _pd_hbe_ptr.set_attributes_ref(attributes_ref);
    _dx_hbe_ptr.set_attributes_ref(attributes_ref);
    _dy_hbe_ptr.set_attributes_ref(attributes_ref);
}

void affine_bg_mat_attributes_hbe_ptr::reload_attributes_ref()
{
    _pa_hbe_ptr.reload_attributes_ref();
    _pb_hbe_ptr.reload_attributes_ref();
    _pc_hbe_ptr.reload_attributes_ref();
    _pd_hbe_ptr.reload_attributes_ref();
    _dx_hbe_ptr.reload_attributes_ref();
    _dy_hbe_ptr.reload_attributes_ref();
}

void affine_bg_mat_attributes_hbe_ptr::swap(affine_bg_mat_attributes_hbe_ptr& other)
{
    _pa_hbe_ptr.swap(other._pa_hbe_ptr);
    _pb_hbe_ptr.swap(other._pb_hbe_ptr);
    _pc_hbe_ptr.swap(other._pc_hbe_ptr);
    _pd_hbe_ptr.swap(other._pd_hbe_ptr);
    _dx_hbe_ptr.swap(other._dx_hbe_ptr);
    _dy_hbe_ptr.swap(other._dy_hbe_ptr);
}

affine_bg_mat_attributes_hbe_ptr::affine_bg_mat_attributes_hbe_ptr(
        affine_bg_pa_register_hbe_ptr&& pa_hbe_ptr,
        affine_bg_pb_register_hbe_ptr&& pb_hbe_ptr,
        affine_bg_pc_register_hbe_ptr&& pc_hbe_ptr,
        affine_bg_pd_register_hbe_ptr&& pd_hbe_ptr,
        affine_bg_dx_register_hbe_ptr&& dx_hbe_ptr,
        affine_bg_dy_register_hbe_ptr&& dy_hbe_ptr) :
    _pa_hbe_ptr(move(pa_hbe_ptr)),
    _pb_hbe_ptr(move(pb_hbe_ptr)),
    _pc_hbe_ptr(move(pc_hbe_ptr)),
    _pd_hbe_ptr(move(pd_hbe_ptr)),
    _dx_hbe_ptr(move(dx_hbe_ptr)),
    _dy_hbe_ptr(move(dy_hbe_ptr))
{
}

}

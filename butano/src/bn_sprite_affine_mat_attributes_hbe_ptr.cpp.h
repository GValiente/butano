/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"

namespace bn
{

sprite_affine_mat_attributes_hbe_ptr sprite_affine_mat_attributes_hbe_ptr::create(
        const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    return sprite_affine_mat_attributes_hbe_ptr(
                sprite_affine_mat_pa_register_hbe_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pb_register_hbe_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pc_register_hbe_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pd_register_hbe_ptr::create(affine_mat, attributes_ref));
}

optional<sprite_affine_mat_attributes_hbe_ptr> sprite_affine_mat_attributes_hbe_ptr::create_optional(
        const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    optional<sprite_affine_mat_attributes_hbe_ptr> result;

    if(auto pa = sprite_affine_mat_pa_register_hbe_ptr::create_optional(affine_mat, attributes_ref))
    {
        if(auto pb = sprite_affine_mat_pb_register_hbe_ptr::create_optional(affine_mat, attributes_ref))
        {
            if(auto pc = sprite_affine_mat_pc_register_hbe_ptr::create_optional(affine_mat, attributes_ref))
            {
                if(auto pd = sprite_affine_mat_pd_register_hbe_ptr::create_optional(affine_mat, attributes_ref))
                {
                    result = sprite_affine_mat_attributes_hbe_ptr(move(*pa), move(*pb), move(*pc), move(*pd));
                }
            }
        }
    }

    return result;
}

void sprite_affine_mat_attributes_hbe_ptr::set_visible(bool visible)
{
    _pa_hbe_ptr.set_visible(visible);
    _pb_hbe_ptr.set_visible(visible);
    _pc_hbe_ptr.set_visible(visible);
    _pd_hbe_ptr.set_visible(visible);
}

span<const affine_mat_attributes> sprite_affine_mat_attributes_hbe_ptr::attributes_ref() const
{
    return _pa_hbe_ptr.attributes_ref();
}

void sprite_affine_mat_attributes_hbe_ptr::set_attributes_ref(const span<const affine_mat_attributes>& attributes_ref)
{
    _pa_hbe_ptr.set_attributes_ref(attributes_ref);
    _pb_hbe_ptr.set_attributes_ref(attributes_ref);
    _pc_hbe_ptr.set_attributes_ref(attributes_ref);
    _pd_hbe_ptr.set_attributes_ref(attributes_ref);
}

void sprite_affine_mat_attributes_hbe_ptr::reload_attributes_ref()
{
    _pa_hbe_ptr.reload_attributes_ref();
    _pb_hbe_ptr.reload_attributes_ref();
    _pc_hbe_ptr.reload_attributes_ref();
    _pd_hbe_ptr.reload_attributes_ref();
}

void sprite_affine_mat_attributes_hbe_ptr::swap(sprite_affine_mat_attributes_hbe_ptr& other)
{
    _pa_hbe_ptr.swap(other._pa_hbe_ptr);
    _pb_hbe_ptr.swap(other._pb_hbe_ptr);
    _pc_hbe_ptr.swap(other._pc_hbe_ptr);
    _pd_hbe_ptr.swap(other._pd_hbe_ptr);
}

sprite_affine_mat_attributes_hbe_ptr::sprite_affine_mat_attributes_hbe_ptr(
        sprite_affine_mat_pa_register_hbe_ptr&& pa_hbe_ptr,
        sprite_affine_mat_pb_register_hbe_ptr&& pb_hbe_ptr,
        sprite_affine_mat_pc_register_hbe_ptr&& pc_hbe_ptr,
        sprite_affine_mat_pd_register_hbe_ptr&& pd_hbe_ptr) :
    _pa_hbe_ptr(move(pa_hbe_ptr)),
    _pb_hbe_ptr(move(pb_hbe_ptr)),
    _pc_hbe_ptr(move(pc_hbe_ptr)),
    _pd_hbe_ptr(move(pd_hbe_ptr))
{
}

}

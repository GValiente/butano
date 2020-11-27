/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mat_attributes_hblank_effect_ptr.h"

#include "bn_span.h"
#include "bn_optional.h"

namespace bn
{

sprite_affine_mat_attributes_hblank_effect_ptr sprite_affine_mat_attributes_hblank_effect_ptr::create(
        const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    return sprite_affine_mat_attributes_hblank_effect_ptr(
                sprite_affine_mat_pa_register_hblank_effect_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pb_register_hblank_effect_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pc_register_hblank_effect_ptr::create(affine_mat, attributes_ref),
                sprite_affine_mat_pd_register_hblank_effect_ptr::create(affine_mat, attributes_ref));
}

optional<sprite_affine_mat_attributes_hblank_effect_ptr> sprite_affine_mat_attributes_hblank_effect_ptr::create_optional(
        const sprite_affine_mat_ptr& affine_mat, const span<const affine_mat_attributes>& attributes_ref)
{
    optional<sprite_affine_mat_attributes_hblank_effect_ptr> result;

    if(auto pa = sprite_affine_mat_pa_register_hblank_effect_ptr::create_optional(affine_mat, attributes_ref))
    {
        if(auto pb = sprite_affine_mat_pb_register_hblank_effect_ptr::create_optional(affine_mat, attributes_ref))
        {
            if(auto pc = sprite_affine_mat_pc_register_hblank_effect_ptr::create_optional(affine_mat, attributes_ref))
            {
                if(auto pd = sprite_affine_mat_pd_register_hblank_effect_ptr::create_optional(affine_mat, attributes_ref))
                {
                    result = sprite_affine_mat_attributes_hblank_effect_ptr(move(*pa), move(*pb), move(*pc), move(*pd));
                }
            }
        }
    }

    return result;
}

void sprite_affine_mat_attributes_hblank_effect_ptr::set_visible(bool visible)
{
    _pa_hblank_effect_ptr.set_visible(visible);
    _pb_hblank_effect_ptr.set_visible(visible);
    _pc_hblank_effect_ptr.set_visible(visible);
    _pd_hblank_effect_ptr.set_visible(visible);
}

span<const affine_mat_attributes> sprite_affine_mat_attributes_hblank_effect_ptr::attributes_ref() const
{
    return _pa_hblank_effect_ptr.attributes_ref();
}

void sprite_affine_mat_attributes_hblank_effect_ptr::set_attributes_ref(
        const span<const affine_mat_attributes>& attributes_ref)
{
    _pa_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pb_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pc_hblank_effect_ptr.set_attributes_ref(attributes_ref);
    _pd_hblank_effect_ptr.set_attributes_ref(attributes_ref);
}

void sprite_affine_mat_attributes_hblank_effect_ptr::reload_attributes_ref()
{
    _pa_hblank_effect_ptr.reload_attributes_ref();
    _pb_hblank_effect_ptr.reload_attributes_ref();
    _pc_hblank_effect_ptr.reload_attributes_ref();
    _pd_hblank_effect_ptr.reload_attributes_ref();
}

void sprite_affine_mat_attributes_hblank_effect_ptr::swap(sprite_affine_mat_attributes_hblank_effect_ptr& other)
{
    _pa_hblank_effect_ptr.swap(other._pa_hblank_effect_ptr);
    _pb_hblank_effect_ptr.swap(other._pb_hblank_effect_ptr);
    _pc_hblank_effect_ptr.swap(other._pc_hblank_effect_ptr);
    _pd_hblank_effect_ptr.swap(other._pd_hblank_effect_ptr);
}

sprite_affine_mat_attributes_hblank_effect_ptr::sprite_affine_mat_attributes_hblank_effect_ptr(
        sprite_affine_mat_pa_register_hblank_effect_ptr&& pa_hblank_effect_ptr,
        sprite_affine_mat_pb_register_hblank_effect_ptr&& pb_hblank_effect_ptr,
        sprite_affine_mat_pc_register_hblank_effect_ptr&& pc_hblank_effect_ptr,
        sprite_affine_mat_pd_register_hblank_effect_ptr&& pd_hblank_effect_ptr) :
    _pa_hblank_effect_ptr(move(pa_hblank_effect_ptr)),
    _pb_hblank_effect_ptr(move(pb_hblank_effect_ptr)),
    _pc_hblank_effect_ptr(move(pc_hblank_effect_ptr)),
    _pd_hblank_effect_ptr(move(pd_hblank_effect_ptr))
{
}

}

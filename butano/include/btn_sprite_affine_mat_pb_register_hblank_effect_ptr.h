/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_AFFINE_MAT_PB_REGISTER_HBLANK_EFFECT_PTR_H
#define BTN_SPRITE_AFFINE_MAT_PB_REGISTER_HBLANK_EFFECT_PTR_H

#include "btn_hblank_effect_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

class sprite_affine_mat_pb_register_hblank_effect_ptr : public hblank_effect_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_pb_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    [[nodiscard]] static sprite_affine_mat_pb_register_hblank_effect_ptr create(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    [[nodiscard]] static optional<sprite_affine_mat_pb_register_hblank_effect_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const sprite_affine_mat_attributes>& attributes_ref);

    [[nodiscard]] static optional<sprite_affine_mat_pb_register_hblank_effect_ptr> create_optional(
            sprite_affine_mat_ptr affine_mat, const span<const int16_t>& values_ref);

    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat;
    }

    [[nodiscard]] span<const sprite_affine_mat_attributes> attributes_ref() const;

    [[nodiscard]] span<const int16_t> values_ref() const;

    void set_attributes_ref(const span<const sprite_affine_mat_attributes>& attributes_ref);

    void set_values_ref(const span<const int16_t>& values_ref);

    void reload_attributes_ref();

    void reload_values_ref();

    void swap(sprite_affine_mat_pb_register_hblank_effect_ptr& other);

    friend void swap(sprite_affine_mat_pb_register_hblank_effect_ptr& a,
                     sprite_affine_mat_pb_register_hblank_effect_ptr& b)
    {
        a.swap(b);
    }

private:
    bool _from_attributes;
    sprite_affine_mat_ptr _affine_mat;

    sprite_affine_mat_pb_register_hblank_effect_ptr(int id, bool from_attributes, sprite_affine_mat_ptr&& affine_mat);
};

}

#endif

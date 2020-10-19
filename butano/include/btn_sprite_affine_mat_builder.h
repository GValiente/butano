#ifndef BTN_SPRITE_AFFINE_MAT_BUILDER_H
#define BTN_SPRITE_AFFINE_MAT_BUILDER_H

#include "btn_optional_fwd.h"
#include "btn_sprite_affine_mat_attributes.h"

namespace btn
{

class sprite_affine_mat_ptr;

class sprite_affine_mat_builder
{

public:
    [[nodiscard]] constexpr fixed rotation_angle() const
    {
        return _attributes.rotation_angle();
    }

    constexpr sprite_affine_mat_builder& set_rotation_angle(fixed rotation_angle)
    {
        _attributes.set_rotation_angle(rotation_angle);
        return *this;
    }

    [[nodiscard]] constexpr fixed horizontal_scale() const
    {
        return _attributes.horizontal_scale();
    }

    constexpr sprite_affine_mat_builder& set_horizontal_scale(fixed horizontal_scale)
    {
        _attributes.set_horizontal_scale(horizontal_scale);
        return *this;
    }

    [[nodiscard]] constexpr fixed vertical_scale() const
    {
        return _attributes.horizontal_scale();
    }

    constexpr sprite_affine_mat_builder& set_vertical_scale(fixed vertical_scale)
    {
        _attributes.set_vertical_scale(vertical_scale);
        return *this;
    }

    constexpr sprite_affine_mat_builder& set_scale(fixed scale)
    {
        _attributes.set_scale(scale);
        return *this;
    }

    constexpr sprite_affine_mat_builder& set_scale(fixed horizontal_scale, fixed vertical_scale)
    {
        _attributes.set_scale(horizontal_scale, vertical_scale);
        return *this;
    }

    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _attributes.horizontal_flip();
    }

    constexpr sprite_affine_mat_builder& set_horizontal_flip(bool horizontal_flip)
    {
        _attributes.set_horizontal_flip(horizontal_flip);
        return *this;
    }

    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _attributes.vertical_flip();
    }

    constexpr sprite_affine_mat_builder& set_vertical_flip(bool vertical_flip)
    {
        _attributes.set_vertical_flip(vertical_flip);
        return *this;
    }

    [[nodiscard]] constexpr const sprite_affine_mat_attributes& attributes() const
    {
        return _attributes;
    }

    constexpr sprite_affine_mat_builder& set_attributes(const sprite_affine_mat_attributes& attributes)
    {
        _attributes = attributes;
        return *this;
    }

    [[nodiscard]] sprite_affine_mat_ptr build() const;

    [[nodiscard]] optional<sprite_affine_mat_ptr> build_optional() const;

private:
    sprite_affine_mat_attributes _attributes;
};

}

#endif


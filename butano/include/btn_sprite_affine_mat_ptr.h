#ifndef BTN_SPRITE_AFFINE_MAT_PTR_H
#define BTN_SPRITE_AFFINE_MAT_PTR_H

#include "btn_fixed_fwd.h"
#include "btn_functional.h"

namespace btn
{

class sprite_affine_mat_builder;

class sprite_affine_mat_ptr
{

public:
    [[nodiscard]] static sprite_affine_mat_ptr create();

    [[nodiscard]] static sprite_affine_mat_ptr create(const sprite_affine_mat_builder& builder);

    sprite_affine_mat_ptr(const sprite_affine_mat_ptr& other);

    sprite_affine_mat_ptr& operator=(const sprite_affine_mat_ptr& other);

    sprite_affine_mat_ptr(sprite_affine_mat_ptr&& other);

    sprite_affine_mat_ptr& operator=(sprite_affine_mat_ptr&& other);

    ~sprite_affine_mat_ptr()
    {
        _destroy();
    }

    [[nodiscard]] int id() const
    {
        return _id;
    }

    [[nodiscard]] fixed rotation_angle() const;

    void set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed scale_x() const;

    void set_scale_x(fixed scale_x);

    [[nodiscard]] fixed scale_y() const;

    void set_scale_y(fixed scale_y);

    [[nodiscard]] bool horizontal_flip() const;

    void set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const;

    void set_vertical_flip(bool vertical_flip);

    [[nodiscard]] bool is_identity() const;

    [[nodiscard]] friend bool operator==(const sprite_affine_mat_ptr& a, const sprite_affine_mat_ptr& b)
    {
        return a._id == b._id;
    }

    [[nodiscard]] friend bool operator!=(const sprite_affine_mat_ptr& a, const sprite_affine_mat_ptr& b)
    {
        return ! (a == b);
    }

private:
    int8_t _id;

    explicit sprite_affine_mat_ptr(int id) :
        _id(int8_t(id))
    {
    }

    void _destroy();
};


template<>
struct hash<sprite_affine_mat_ptr>
{
    [[nodiscard]] size_t operator()(const sprite_affine_mat_ptr& value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

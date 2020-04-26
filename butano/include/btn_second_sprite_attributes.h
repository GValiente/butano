#ifndef BTN_SECOND_SPRITE_ATTRIBUTES_H
#define BTN_SECOND_SPRITE_ATTRIBUTES_H

#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

enum class sprite_size;

class alignas(alignof(int)) second_sprite_attributes
{

public:
    second_sprite_attributes(fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip,
                             const optional<sprite_affine_mat_ptr>& affine_mat);

    second_sprite_attributes(fixed x, sprite_size size, bool horizontal_flip, bool vertical_flip,
                             optional<sprite_affine_mat_ptr>&& affine_mat);

    [[nodiscard]] fixed x() const
    {
        return _x;
    }

    void set_x(fixed x)
    {
        _x = x;
    }

    [[nodiscard]] sprite_size size() const;

    void set_size(sprite_size size);

    [[nodiscard]] bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    void set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const
    {
        return _vertical_flip;
    }

    void set_vertical_flip(bool vertical_flip);

    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const
    {
        return _affine_mat;
    }

    void set_affine_mat(const optional<sprite_affine_mat_ptr>& affine_mat);

    void set_affine_mat(optional<sprite_affine_mat_ptr>&& affine_mat);

    [[nodiscard]] friend bool operator==(const second_sprite_attributes& a, const second_sprite_attributes& b)
    {
        return a._x.integer() == b._x.integer() && a._size == b._size && a._horizontal_flip == b._horizontal_flip &&
                a._vertical_flip == b._vertical_flip && a._affine_mat == b._affine_mat;
    }

    [[nodiscard]] friend bool operator!=(const second_sprite_attributes& a, const second_sprite_attributes& b)
    {
        return ! (a == b);
    }

private:
    fixed _x;
    optional<sprite_affine_mat_ptr> _affine_mat;
    uint8_t _size;
    unsigned _horizontal_flip: 1;
    unsigned _vertical_flip: 1;
};

}

#endif


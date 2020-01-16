#ifndef BTN_SPRITE_BUILDER_H
#define BTN_SPRITE_BUILDER_H

#include "btn_optional.h"
#include "btn_sprite_item.h"
#include "btn_fixed_point.h"
#include "btn_create_mode.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_shape_size.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"

namespace btn
{

class sprite_ptr;
class sprite_tiles_ptr;
class sprite_palette_ptr;

class sprite_builder
{

public:
    sprite_builder(const sprite_item& item, int graphics_index = 0);

    sprite_builder(sprite_shape shape, sprite_size size, sprite_tiles_ptr tiles_ptr, sprite_palette_ptr palette_ptr);

    [[nodiscard]] const optional<sprite_item>& item() const
    {
        return _item;
    }

    [[nodiscard]] int graphics_index() const
    {
        return _graphics_index;
    }

    [[nodiscard]] sprite_shape shape() const
    {
        return _shape;
    }

    [[nodiscard]] sprite_size size() const
    {
        return _size;
    }

    [[nodiscard]] sprite_shape_size shape_size() const
    {
        return sprite_shape_size(_shape, _size);
    }

    [[nodiscard]] create_mode tiles_create_mode() const
    {
        return _tiles_create_mode;
    }

    sprite_builder& set_tiles_create_mode(create_mode create_mode)
    {
        _tiles_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] create_mode palette_create_mode() const
    {
        return _palette_create_mode;
    }

    sprite_builder& set_palette_create_mode(create_mode create_mode)
    {
        _palette_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] fixed x() const
    {
        return _position.x();
    }

    [[nodiscard]] fixed y() const
    {
        return _position.y();
    }

    [[nodiscard]] const fixed_point& position() const
    {
        return _position;
    }

    sprite_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    sprite_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    sprite_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    sprite_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    [[nodiscard]] fixed rotation_angle() const
    {
        return _affine_mat_ptr ? _affine_mat_ptr->rotation_angle() : 0;
    }

    sprite_builder& set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed scale_x() const
    {
        return _affine_mat_ptr ? _affine_mat_ptr->scale_x() : 1;
    }

    sprite_builder& set_scale_x(fixed scale_x);

    [[nodiscard]] fixed scale_y() const
    {
        return _affine_mat_ptr ? _affine_mat_ptr->scale_y() : 1;
    }

    sprite_builder& set_scale_y(fixed scale_y);

    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    sprite_builder& set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    sprite_builder& set_z_order(int z_order);

    [[nodiscard]] bool horizontal_flip() const
    {
        return _affine_mat_ptr ? _affine_mat_ptr->horizontal_flip() : _horizontal_flip;
    }

    sprite_builder& set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const
    {
        return _affine_mat_ptr ? _affine_mat_ptr->vertical_flip() : _vertical_flip;
    }

    sprite_builder& set_vertical_flip(bool vertical_flip);

    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    sprite_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    [[nodiscard]] bool double_size() const
    {
        return _double_size;
    }

    sprite_builder& set_double_size(bool double_size)
    {
        _double_size = double_size;
        return *this;
    }

    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    sprite_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    [[nodiscard]] bool ignore_camera() const
    {
        return _ignore_camera;
    }

    sprite_builder& set_ignore_camera(bool ignore_camera)
    {
        _ignore_camera = ignore_camera;
        return *this;
    }

    [[nodiscard]] sprite_ptr build() const;

    [[nodiscard]] sprite_ptr build_and_release();

    [[nodiscard]] optional<sprite_ptr> optional_build() const;

    [[nodiscard]] optional<sprite_ptr> optional_build_and_release();

    [[nodiscard]] optional<sprite_tiles_ptr> tiles() const;

    [[nodiscard]] optional<sprite_palette_ptr> palette() const;

    [[nodiscard]] optional<sprite_tiles_ptr> release_tiles();

    [[nodiscard]] optional<sprite_palette_ptr> release_palette();

    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const
    {
        return _affine_mat_ptr;
    }

    sprite_builder& set_affine_mat(sprite_affine_mat_ptr affine_mat)
    {
        _affine_mat_ptr = move(affine_mat);
        return *this;
    }

    [[nodiscard]] optional<sprite_affine_mat_ptr> release_affine_mat()
    {
        return move(_affine_mat_ptr);
    }

    [[nodiscard]] bool remove_affine_mat_when_not_needed() const
    {
        return _remove_affine_mat_when_not_needed;
    }

    void set_remove_affine_mat_when_not_needed(bool remove_when_not_needed)
    {
        _remove_affine_mat_when_not_needed = remove_when_not_needed;
    }

private:
    optional<sprite_item> _item;
    optional<sprite_tiles_ptr> _tiles_ptr;
    optional<sprite_palette_ptr> _palette_ptr;
    optional<sprite_affine_mat_ptr> _affine_mat_ptr;
    create_mode _tiles_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _palette_create_mode = create_mode::FIND_OR_CREATE;
    fixed_point _position;
    sprite_shape _shape;
    sprite_size _size;
    int _graphics_index;
    int _bg_priority = 2;
    int _z_order = 0;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
    bool _mosaic_enabled = false;
    bool _visible = true;
    bool _double_size = false;
    bool _ignore_camera = false;
    bool _remove_affine_mat_when_not_needed = true;
};

}

#endif


/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_BUILDER_H
#define BTN_SPRITE_BUILDER_H

#include "btn_optional.h"
#include "btn_camera_ptr.h"
#include "btn_sprite_item.h"
#include "btn_fixed_point.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_shape_size.h"
#include "btn_sprite_palette_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "btn_sprite_double_size_mode.h"

namespace btn
{

class sprite_ptr;

class sprite_builder
{

public:
    explicit sprite_builder(const sprite_item& item);

    sprite_builder(const sprite_item& item, int graphics_index);

    sprite_builder(const sprite_shape_size& shape_size, sprite_tiles_ptr tiles, sprite_palette_ptr palette);

    [[nodiscard]] const optional<sprite_item>& item() const
    {
        return _item;
    }

    [[nodiscard]] int graphics_index() const
    {
        return _graphics_index;
    }

    [[nodiscard]] const sprite_shape_size& shape_size() const
    {
        return _shape_size;
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
        return _affine_mat ? _affine_mat->rotation_angle() : 0;
    }

    sprite_builder& set_rotation_angle(fixed rotation_angle);

    [[nodiscard]] fixed horizontal_scale() const
    {
        return _affine_mat ? _affine_mat->horizontal_scale() : 1;
    }

    sprite_builder& set_horizontal_scale(fixed horizontal_scale);

    [[nodiscard]] fixed vertical_scale() const
    {
        return _affine_mat ? _affine_mat->vertical_scale() : 1;
    }

    sprite_builder& set_vertical_scale(fixed vertical_scale);

    sprite_builder& set_scale(fixed scale);

    sprite_builder& set_scale(fixed horizontal_scale, fixed vertical_scale);

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
        return _affine_mat ? _affine_mat->horizontal_flip() : _horizontal_flip;
    }

    sprite_builder& set_horizontal_flip(bool horizontal_flip);

    [[nodiscard]] bool vertical_flip() const
    {
        return _affine_mat ? _affine_mat->vertical_flip() : _vertical_flip;
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

    [[nodiscard]] bool blending_enabled() const
    {
        return _blending_enabled;
    }

    sprite_builder& set_blending_enabled(bool blending_enabled);

    [[nodiscard]] bool window_enabled() const
    {
        return _window_enabled;
    }

    sprite_builder& set_window_enabled(bool window_enabled);

    [[nodiscard]] sprite_double_size_mode double_size_mode() const
    {
        return _double_size_mode;
    }

    sprite_builder& set_double_size_mode(sprite_double_size_mode double_size_mode)
    {
        _double_size_mode = double_size_mode;
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

    [[nodiscard]] const optional<camera_ptr>& camera() const
    {
        return _camera;
    }

    sprite_builder& set_camera(const camera_ptr& camera)
    {
        _camera = camera;
        return *this;
    }

    sprite_builder& set_camera(camera_ptr&& camera)
    {
        _camera = move(camera);
        return *this;
    }

    sprite_builder& remove_camera()
    {
        _camera.reset();
        return *this;
    }

    [[nodiscard]] optional<camera_ptr> release_camera()
    {
        return move(_camera);
    }

    [[nodiscard]] sprite_ptr build() const;

    [[nodiscard]] sprite_ptr release_build();

    [[nodiscard]] optional<sprite_ptr> build_optional() const;

    [[nodiscard]] optional<sprite_ptr> release_build_optional();

    [[nodiscard]] sprite_tiles_ptr tiles() const;

    [[nodiscard]] sprite_palette_ptr palette() const;

    [[nodiscard]] optional<sprite_tiles_ptr> tiles_optional() const;

    [[nodiscard]] optional<sprite_palette_ptr> palette_optional() const;

    [[nodiscard]] sprite_tiles_ptr release_tiles();

    [[nodiscard]] sprite_palette_ptr release_palette();

    [[nodiscard]] optional<sprite_tiles_ptr> release_tiles_optional();

    [[nodiscard]] optional<sprite_palette_ptr> release_palette_optional();

    [[nodiscard]] const optional<sprite_affine_mat_ptr>& affine_mat() const
    {
        return _affine_mat;
    }

    sprite_builder& set_affine_mat(const sprite_affine_mat_ptr& affine_mat);

    sprite_builder& set_affine_mat(sprite_affine_mat_ptr&& affine_mat);

    sprite_builder& remove_affine_mat()
    {
        _affine_mat.reset();
        return *this;
    }

    [[nodiscard]] optional<sprite_affine_mat_ptr> release_affine_mat()
    {
        return move(_affine_mat);
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
    sprite_shape_size _shape_size;
    sprite_double_size_mode _double_size_mode = sprite_double_size_mode::AUTO;
    fixed_point _position;
    int _graphics_index;
    int _bg_priority = 3;
    int _z_order = 0;
    optional<sprite_tiles_ptr> _tiles;
    optional<sprite_palette_ptr> _palette;
    optional<sprite_affine_mat_ptr> _affine_mat;
    optional<camera_ptr> _camera;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _window_enabled = false;
    bool _visible = true;
    bool _remove_affine_mat_when_not_needed = true;
};

}

#endif


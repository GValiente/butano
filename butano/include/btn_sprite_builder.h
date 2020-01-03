#ifndef BTN_SPRITE_BUILDER_H
#define BTN_SPRITE_BUILDER_H

#include "btn_fixed_point.h"
#include "btn_create_mode.h"
#include "btn_sprite_shape_size.h"

namespace btn
{

class sprite_ptr;
class sprite_item;
class sprite_tiles_ptr;
class sprite_palette_ptr;

class sprite_builder
{

public:
    sprite_builder(const sprite_item& item_ref, int graphics_id = 0);

    sprite_builder(sprite_shape shape, sprite_size size, const sprite_tiles_ptr& tiles_ptr_ref,
                   const sprite_palette_ptr& palette_ptr_ref);

    [[nodiscard]] const sprite_item* item() const
    {
        return _item_ref;
    }

    [[nodiscard]] int graphics_id() const
    {
        return _graphics_id;
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

    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    void set_bg_priority(int bg_priority);

    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    void set_z_order(int z_order);

    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    void set_visible(bool visible)
    {
        _visible = visible;
    }

    sprite_ptr build() const;

    [[nodiscard]] sprite_tiles_ptr tiles_ptr() const;

    [[nodiscard]] sprite_palette_ptr palette_ptr() const;

private:
    const sprite_item* _item_ref;
    const sprite_tiles_ptr* _tiles_ptr_ref;
    const sprite_palette_ptr* _palette_ptr_ref;
    create_mode _tiles_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _palette_create_mode = create_mode::FIND_OR_CREATE;
    fixed_point _position;
    sprite_shape _shape;
    sprite_size _size;
    int _graphics_id;
    int _bg_priority = 2;
    int _z_order = 0;
    bool _visible = true;
};

}

#endif


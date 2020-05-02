#ifndef BTN_REGULAR_BG_BUILDER_H
#define BTN_REGULAR_BG_BUILDER_H

#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_regular_bg_item.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

class regular_bg_ptr;

class regular_bg_builder
{

public:
    explicit regular_bg_builder(const regular_bg_item& item) :
        _item(item)
    {
    }

    explicit regular_bg_builder(const regular_bg_map_ptr& map) :
        _map(map)
    {
    }

    explicit regular_bg_builder(regular_bg_map_ptr&& map) :
        _map(move(map))
    {
    }

    [[nodiscard]] const optional<regular_bg_item>& item() const
    {
        return _item;
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

    regular_bg_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    regular_bg_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    regular_bg_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    regular_bg_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    regular_bg_builder& set_priority(int priority);

    [[nodiscard]] int z_order() const
    {
        return _z_order;
    }

    regular_bg_builder& set_z_order(int z_order);

    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    regular_bg_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    [[nodiscard]] bool blending_enabled() const
    {
        return _blending_enabled;
    }

    regular_bg_builder& set_blending_enabled(bool blending_enabled)
    {
        _blending_enabled = blending_enabled;
        return *this;
    }

    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    regular_bg_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    [[nodiscard]] bool ignore_camera() const
    {
        return _ignore_camera;
    }

    regular_bg_builder& set_ignore_camera(bool ignore_camera)
    {
        _ignore_camera = ignore_camera;
        return *this;
    }

    [[nodiscard]] regular_bg_ptr build() const;

    [[nodiscard]] regular_bg_ptr release_build();

    [[nodiscard]] optional<regular_bg_ptr> optional_build() const;

    [[nodiscard]] optional<regular_bg_ptr> optional_release_build();

    [[nodiscard]] regular_bg_map_ptr map() const;

    [[nodiscard]] optional<regular_bg_map_ptr> optional_map() const;

    [[nodiscard]] regular_bg_map_ptr release_map();

    [[nodiscard]] optional<regular_bg_map_ptr> optional_release_map();

private:
    optional<regular_bg_item> _item;
    optional<regular_bg_map_ptr> _map;
    fixed_point _position;
    int _priority = 3;
    int _z_order = 0;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _visible = true;
    bool _ignore_camera = false;
};

}

#endif


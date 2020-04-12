#ifndef BTN_REGULAR_BG_BUILDER_H
#define BTN_REGULAR_BG_BUILDER_H

#include "btn_optional.h"
#include "btn_fixed_point.h"
#include "btn_create_mode.h"
#include "btn_bg_tiles_ptr.h"
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

    template<class BgTilesPtr, class RegularBgMapPtr>
    regular_bg_builder(BgTilesPtr&& tiles_ptr, RegularBgMapPtr&& map_ptr) :
        _tiles_ptr(forward<BgTilesPtr>(tiles_ptr)),
        _map_ptr(forward<RegularBgMapPtr>(map_ptr))
    {
        BTN_ASSERT(_tiles_ptr->valid_tiles_count(_map_ptr->bpp_mode()), "Invalid tiles count: ",
                   _tiles_ptr->tiles_count());
    }

    [[nodiscard]] const optional<regular_bg_item>& item() const
    {
        return _item;
    }

    [[nodiscard]] create_mode tiles_create_mode() const
    {
        return _tiles_create_mode;
    }

    regular_bg_builder& set_tiles_create_mode(create_mode create_mode)
    {
        _tiles_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] create_mode map_create_mode() const
    {
        return _map_create_mode;
    }

    regular_bg_builder& set_map_create_mode(create_mode create_mode)
    {
        _map_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] create_mode palette_create_mode() const
    {
        return _palette_create_mode;
    }

    regular_bg_builder& set_palette_create_mode(create_mode create_mode)
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

    [[nodiscard]] bg_tiles_ptr tiles() const;

    [[nodiscard]] regular_bg_map_ptr map() const;

    [[nodiscard]] optional<bg_tiles_ptr> optional_tiles() const;

    [[nodiscard]] optional<regular_bg_map_ptr> optional_map() const;

    [[nodiscard]] bg_tiles_ptr release_tiles();

    [[nodiscard]] regular_bg_map_ptr release_map();

    [[nodiscard]] optional<bg_tiles_ptr> optional_release_tiles();

    [[nodiscard]] optional<regular_bg_map_ptr> optional_release_map();

private:
    optional<regular_bg_item> _item;
    optional<bg_tiles_ptr> _tiles_ptr;
    optional<regular_bg_map_ptr> _map_ptr;
    create_mode _tiles_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _map_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _palette_create_mode = create_mode::FIND_OR_CREATE;
    fixed_point _position;
    int _priority = 3;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _visible = true;
    bool _ignore_camera = false;
};

}

#endif


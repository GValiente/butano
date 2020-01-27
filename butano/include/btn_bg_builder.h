#ifndef BTN_BG_BUILDER_H
#define BTN_BG_BUILDER_H

#include "btn_bg_item.h"
#include "btn_optional.h"
#include "btn_bg_map_ptr.h"
#include "btn_fixed_point.h"
#include "btn_create_mode.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"

namespace btn
{

class bg_ptr;

class bg_builder
{

public:
    explicit bg_builder(const bg_item& item) :
        _item(item)
    {
    }

    template<class BgTilesPtr, class BgMapPtr, class BgPalettePtr>
    bg_builder(BgTilesPtr&& tiles_ptr, BgMapPtr&& map_ptr, BgPalettePtr&& palette_ptr) :
        _tiles_ptr(forward<BgTilesPtr>(tiles_ptr)),
        _map_ptr(forward<BgMapPtr>(map_ptr)),
        _palette_ptr(forward<BgPalettePtr>(palette_ptr))
    {
        BTN_ASSERT(_tiles_ptr->valid_tiles_count(_palette_ptr->eight_bits_per_pixel()), "Invalid tiles count: ",
                   _tiles_ptr->tiles_count());
    }

    [[nodiscard]] const optional<bg_item>& item() const
    {
        return _item;
    }

    [[nodiscard]] create_mode tiles_create_mode() const
    {
        return _tiles_create_mode;
    }

    bg_builder& set_tiles_create_mode(create_mode create_mode)
    {
        _tiles_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] create_mode map_create_mode() const
    {
        return _map_create_mode;
    }

    bg_builder& set_map_create_mode(create_mode create_mode)
    {
        _map_create_mode = create_mode;
        return *this;
    }

    [[nodiscard]] create_mode palette_create_mode() const
    {
        return _palette_create_mode;
    }

    bg_builder& set_palette_create_mode(create_mode create_mode)
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

    bg_builder& set_x(fixed x)
    {
        _position.set_x(x);
        return *this;
    }

    bg_builder& set_y(fixed y)
    {
        _position.set_y(y);
        return *this;
    }

    bg_builder& set_position(fixed x, fixed y)
    {
        _position = fixed_point(x, y);
        return *this;
    }

    bg_builder& set_position(const fixed_point& position)
    {
        _position = position;
        return *this;
    }

    [[nodiscard]] int priority() const
    {
        return _priority;
    }

    bg_builder& set_priority(int priority);

    [[nodiscard]] bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    bg_builder& set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
        return *this;
    }

    [[nodiscard]] bool visible() const
    {
        return _visible;
    }

    bg_builder& set_visible(bool visible)
    {
        _visible = visible;
        return *this;
    }

    [[nodiscard]] bool ignore_camera() const
    {
        return _ignore_camera;
    }

    bg_builder& set_ignore_camera(bool ignore_camera)
    {
        _ignore_camera = ignore_camera;
        return *this;
    }

    [[nodiscard]] bg_ptr build() const;

    [[nodiscard]] bg_ptr build_and_release();

    [[nodiscard]] optional<bg_ptr> optional_build() const;

    [[nodiscard]] optional<bg_ptr> optional_build_and_release();

    [[nodiscard]] optional<bg_tiles_ptr> tiles() const;

    [[nodiscard]] optional<bg_map_ptr> map() const;

    [[nodiscard]] optional<bg_palette_ptr> palette() const;

    [[nodiscard]] optional<bg_tiles_ptr> release_tiles();

    [[nodiscard]] optional<bg_map_ptr> release_map();

    [[nodiscard]] optional<bg_palette_ptr> release_palette();

private:
    optional<bg_item> _item;
    optional<bg_tiles_ptr> _tiles_ptr;
    optional<bg_map_ptr> _map_ptr;
    optional<bg_palette_ptr> _palette_ptr;
    create_mode _tiles_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _map_create_mode = create_mode::FIND_OR_CREATE;
    create_mode _palette_create_mode = create_mode::FIND_OR_CREATE;
    fixed_point _position;
    int _priority = 3;
    bool _mosaic_enabled = false;
    bool _visible = true;
    bool _ignore_camera = false;
};

}

#endif


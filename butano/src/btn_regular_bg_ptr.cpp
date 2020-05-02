#include "btn_regular_bg_ptr.h"

#include "btn_size.h"
#include "btn_window.h"
#include "btn_sprite_ptr.h"
#include "btn_bgs_manager.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_builder.h"
#include "btn_regular_bg_attributes.h"

namespace btn
{

regular_bg_ptr regular_bg_ptr::create(fixed x, fixed y, const regular_bg_item& item)
{
    regular_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const fixed_point& position, const regular_bg_item& item)
{
    regular_bg_builder builder(item);
    builder.set_position(position);
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

regular_bg_ptr regular_bg_ptr::create(const regular_bg_builder& builder)
{
    return regular_bg_ptr(bgs_manager::create(regular_bg_builder(builder)));
}

regular_bg_ptr regular_bg_ptr::create(regular_bg_builder&& builder)
{
    return regular_bg_ptr(bgs_manager::create(move(builder)));
}

optional<regular_bg_ptr> regular_bg_ptr::optional_create(fixed x, fixed y, const regular_bg_item& item)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item);
    builder.set_position(fixed_point(x, y));

    if(handle_type handle = bgs_manager::optional_create(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::optional_create(const fixed_point& position, const regular_bg_item& item)
{
    optional<regular_bg_ptr> result;
    regular_bg_builder builder(item);
    builder.set_position(position);

    if(handle_type handle = bgs_manager::optional_create(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::optional_create(const regular_bg_builder& builder)
{
    optional<regular_bg_ptr> result;

    if(handle_type handle = bgs_manager::optional_create(regular_bg_builder(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

optional<regular_bg_ptr> regular_bg_ptr::optional_create(regular_bg_builder&& builder)
{
    optional<regular_bg_ptr> result;

    if(handle_type handle = bgs_manager::optional_create(move(builder)))
    {
        result = regular_bg_ptr(handle);
    }

    return result;
}

regular_bg_ptr::regular_bg_ptr(const regular_bg_ptr& other) :
    regular_bg_ptr(other._handle)
{
    bgs_manager::increase_usages(_handle);
}

regular_bg_ptr& regular_bg_ptr::operator=(const regular_bg_ptr& other)
{
    if(_handle != other._handle)
    {
        if(_handle)
        {
            bgs_manager::decrease_usages(_handle);
        }

        _handle = other._handle;
        bgs_manager::increase_usages(_handle);
    }

    return *this;
}

size regular_bg_ptr::dimensions() const
{
    return bgs_manager::dimensions(_handle);
}

const bg_tiles_ptr& regular_bg_ptr::tiles() const
{
    return bgs_manager::map(_handle).tiles();
}

void regular_bg_ptr::set_tiles(const bg_tiles_ptr& tiles)
{
    regular_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles(tiles);
}

void regular_bg_ptr::set_tiles(bg_tiles_ptr&& tiles)
{
    regular_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles(move(tiles));
}

void regular_bg_ptr::set_tiles(const regular_bg_item& item)
{
    set_tiles(item.tiles_item());
}

void regular_bg_ptr::set_tiles(const bg_tiles_item& tiles_item)
{
    set_tiles(tiles_item.create_tiles());
}

const bg_palette_ptr& regular_bg_ptr::palette() const
{
    return bgs_manager::map(_handle).palette();
}

void regular_bg_ptr::set_palette(const bg_palette_ptr& palette)
{
    regular_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_palette(palette);
}

void regular_bg_ptr::set_palette(bg_palette_ptr&& palette)
{
    regular_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_palette(move(palette));
}

void regular_bg_ptr::set_palette(const regular_bg_item& item)
{
    set_palette(item.palette_item());
}

void regular_bg_ptr::set_palette(const bg_palette_item& palette_item)
{
    set_palette(palette_item.create_palette());
}

void regular_bg_ptr::set_tiles_and_palette(bg_tiles_ptr tiles, bg_palette_ptr palette)
{
    regular_bg_map_ptr map = bgs_manager::map(_handle);
    map.set_tiles_and_palette(move(tiles), move(palette));
}

const regular_bg_map_ptr& regular_bg_ptr::map() const
{
    return bgs_manager::map(_handle);
}

void regular_bg_ptr::set_map(const regular_bg_map_ptr& map)
{
    bgs_manager::set_map(_handle, map);
}

void regular_bg_ptr::set_map(regular_bg_map_ptr&& map)
{
    bgs_manager::set_map(_handle, move(map));
}

void regular_bg_ptr::set_map(const regular_bg_item& item)
{
    set_map(item.map_item());
}

void regular_bg_ptr::set_map(const regular_bg_map_item& map_item)
{
    set_map(map_item.create_map(bg_tiles_ptr(tiles()), bg_palette_ptr(palette())));
}

void regular_bg_ptr::set_item(const regular_bg_item& item)
{
    bgs_manager::set_map(_handle,
                         item.map_item().create_map(item.tiles_item().create_tiles(),
                                                    item.palette_item().create_palette()));
}

fixed regular_bg_ptr::x() const
{
    return position().x();
}

fixed regular_bg_ptr::y() const
{
    return position().y();
}

const fixed_point& regular_bg_ptr::position() const
{
    return bgs_manager::position(_handle);
}

void regular_bg_ptr::set_x(fixed x)
{
    set_position(fixed_point(x, position().y()));
}

void regular_bg_ptr::set_y(fixed y)
{
    set_position(fixed_point(position().x(), y));
}

void regular_bg_ptr::set_position(fixed x, fixed y)
{
    set_position(fixed_point(x, y));
}

void regular_bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_position(_handle, position);
}

int regular_bg_ptr::priority() const
{
    return bgs_manager::priority(_handle);
}

void regular_bg_ptr::set_priority(int priority)
{
    bgs_manager::set_priority(_handle, priority);
}

int regular_bg_ptr::z_order() const
{
    return bgs_manager::z_order(_handle);
}

void regular_bg_ptr::set_z_order(int z_order)
{
    bgs_manager::set_z_order(_handle, z_order);
}

bool regular_bg_ptr::above(const regular_bg_ptr& other) const
{
    if(*this == other)
    {
        return false;
    }

    return bgs_manager::above(_handle, other._handle);
}

bool regular_bg_ptr::above(const sprite_ptr& sprite_ptr) const
{
    return priority() < sprite_ptr.bg_priority();
}

void regular_bg_ptr::put_above(const regular_bg_ptr& other)
{
    if(*this == other)
    {
        return;
    }

    int this_priority = priority();
    int other_priority = other.priority();

    if(this_priority < other_priority)
    {
        return;
    }

    if(this_priority > other_priority)
    {
        set_priority(other_priority);
    }

    int this_z_order = z_order();
    int other_z_order = other.z_order();

    if(this_z_order < other_z_order)
    {
        return;
    }

    if(this_z_order > other_z_order)
    {
        set_z_order(other_z_order);
    }

    bgs_manager::swap_order(_handle, other._handle);
}

void regular_bg_ptr::put_above(const sprite_ptr& sprite_ptr)
{
    int this_priority = priority();
    int sprite_priority = sprite_ptr.bg_priority();

    if(this_priority >= sprite_priority)
    {
        if(sprite_priority > 0)
        {
            set_priority(sprite_priority - 1);
        }
    }
}

bool regular_bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_handle);
}

void regular_bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_handle, mosaic_enabled);
}

bool regular_bg_ptr::blending_enabled() const
{
    return bgs_manager::blending_enabled(_handle);
}

void regular_bg_ptr::set_blending_enabled(bool blending_enabled)
{
    bgs_manager::set_blending_enabled(_handle, blending_enabled);
}

bool regular_bg_ptr::visible() const
{
    return bgs_manager::visible(_handle);
}

void regular_bg_ptr::set_visible(bool visible)
{
    bgs_manager::set_visible(_handle, visible);
}

bool regular_bg_ptr::visible_in_window(const window& window) const
{
    return window.show_bg(*this);
}

void regular_bg_ptr::set_visible_in_window(bool visible, window& window)
{
    window.set_show_bg(*this, visible);
}

bool regular_bg_ptr::ignore_camera() const
{
    return bgs_manager::ignore_camera(_handle);
}

void regular_bg_ptr::set_ignore_camera(bool ignore_camera)
{
    bgs_manager::set_ignore_camera(_handle, ignore_camera);
}

regular_bg_attributes regular_bg_ptr::attributes() const
{
    return bgs_manager::regular_attributes(_handle);
}

void regular_bg_ptr::set_attributes(const regular_bg_attributes& attributes)
{
    bgs_manager::set_regular_attributes(_handle, attributes);
}

void regular_bg_ptr::_destroy()
{
    bgs_manager::decrease_usages(_handle);
}

}

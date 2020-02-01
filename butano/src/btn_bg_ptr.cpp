#include "btn_bg_ptr.h"

#include "btn_size.h"
#include "btn_bg_builder.h"
#include "btn_sprite_ptr.h"
#include "btn_bgs_manager.h"
#include "btn_bg_palette_ptr.h"

namespace btn
{

bg_ptr bg_ptr::create(fixed x, fixed y, const bg_item& item)
{
    bg_builder builder(item);
    builder.set_position(fixed_point(x, y));

    optional<int> id = bgs_manager::create(move(builder));
    BTN_ASSERT(id, "BG create failed");

    return bg_ptr(*id);
}

bg_ptr bg_ptr::create(const fixed_point& position, const bg_item& item)
{
    bg_builder builder(item);
    builder.set_position(position);

    optional<int> id = bgs_manager::create(move(builder));
    BTN_ASSERT(id, "BG create failed");

    return bg_ptr(*id);
}

bg_ptr bg_ptr::create(const bg_builder& builder)
{
    optional<int> id = bgs_manager::create(bg_builder(builder));
    BTN_ASSERT(id, "BG create failed");

    return bg_ptr(*id);
}

bg_ptr bg_ptr::create(bg_builder&& builder)
{
    optional<int> id = bgs_manager::create(move(builder));
    BTN_ASSERT(id, "BG create failed");

    return bg_ptr(*id);
}

optional<bg_ptr> bg_ptr::optional_create(fixed x, fixed y, const bg_item& item)
{
    optional<bg_ptr> result;
    bg_builder builder(item);
    builder.set_position(fixed_point(x, y));

    if(optional<int> id = bgs_manager::create(move(builder)))
    {
        result = bg_ptr(*id);
    }

    return result;
}

optional<bg_ptr> bg_ptr::optional_create(const fixed_point& position, const bg_item& item)
{
    optional<bg_ptr> result;
    bg_builder builder(item);
    builder.set_position(position);

    if(optional<int> id = bgs_manager::create(move(builder)))
    {
        result = bg_ptr(*id);
    }

    return result;
}

optional<bg_ptr> bg_ptr::optional_create(const bg_builder& builder)
{
    optional<bg_ptr> result;

    if(optional<int> id = bgs_manager::create(bg_builder(builder)))
    {
        result = bg_ptr(*id);
    }

    return result;
}

optional<bg_ptr> bg_ptr::optional_create(bg_builder&& builder)
{
    optional<bg_ptr> result;

    if(optional<int> id = bgs_manager::create(move(builder)))
    {
        result = bg_ptr(*id);
    }

    return result;
}

bg_ptr::bg_ptr(const bg_ptr& other) :
    bg_ptr(other._id)
{
    bgs_manager::increase_usages(_id);
}

bg_ptr& bg_ptr::operator=(const bg_ptr& other)
{
    if(_id != other._id)
    {
        if(_id >= 0)
        {
            bgs_manager::decrease_usages(_id);
        }

        _id = other._id;
        bgs_manager::increase_usages(_id);
    }

    return *this;
}

size bg_ptr::dimensions() const
{
    return bgs_manager::dimensions(_id);
}

const bg_tiles_ptr& bg_ptr::tiles() const
{
    return bgs_manager::tiles(_id);
}

void bg_ptr::set_tiles(const bg_tiles_ptr& tiles_ptr)
{
    bgs_manager::set_tiles(_id, tiles_ptr);
}

void bg_ptr::set_tiles(bg_tiles_ptr&& tiles_ptr)
{
    bgs_manager::set_tiles(_id, move(tiles_ptr));
}

void bg_ptr::set_tiles(const bg_item& item, create_mode create_mode)
{
    set_tiles(item.tiles_item(), create_mode);
}

void bg_ptr::set_tiles(const bg_tiles_item& tiles_item, create_mode create_mode)
{
    optional<bg_tiles_ptr> tiles_ptr = tiles_item.create_tiles(create_mode);
    BTN_ASSERT(tiles_ptr, "Tiles create failed");

    set_tiles(move(*tiles_ptr));
}

const bg_map_ptr& bg_ptr::map() const
{
    return bgs_manager::map(_id);
}

void bg_ptr::set_map(const bg_map_ptr& map_ptr)
{
    bgs_manager::set_map(_id, map_ptr);
}

void bg_ptr::set_map(bg_map_ptr&& map_ptr)
{
    bgs_manager::set_map(_id, move(map_ptr));
}

void bg_ptr::set_map(const bg_item& item, create_mode create_mode)
{
    set_map(item.map_item(), create_mode);
}

void bg_ptr::set_map(const bg_map_item& map_item, create_mode create_mode)
{
    optional<bg_map_ptr> map_ptr = map_item.create_map(bg_palette_ptr(palette()), create_mode);
    BTN_ASSERT(map_ptr, "Map create failed");

    set_map(move(*map_ptr));
}

const bg_palette_ptr& bg_ptr::palette() const
{
    return bgs_manager::map(_id).palette();
}

void bg_ptr::set_palette(const bg_palette_ptr& palette_ptr)
{
    bg_map_ptr map_ptr = bgs_manager::map(_id);
    map_ptr.set_palette(palette_ptr);
    bgs_manager::set_map(_id, move(map_ptr));
}

void bg_ptr::set_palette(bg_palette_ptr&& palette_ptr)
{
    bg_map_ptr map_ptr = bgs_manager::map(_id);
    map_ptr.set_palette(move(palette_ptr));
    bgs_manager::set_map(_id, move(map_ptr));
}

void bg_ptr::set_palette(const bg_item& item, create_mode create_mode)
{
    set_palette(item.palette_item(), create_mode);
}

void bg_ptr::set_palette(const bg_palette_item& palette_item, create_mode create_mode)
{
    optional<bg_palette_ptr> palette_ptr = palette_item.create_palette(create_mode);
    BTN_ASSERT(palette_ptr, "Palette create failed");

    set_palette(move(*palette_ptr));
}

void bg_ptr::set_item(const bg_item& item, create_mode create_mode)
{
    set_tiles(item, create_mode);
    set_map(item, create_mode);
    set_palette(item, create_mode);
}

fixed bg_ptr::x() const
{
    return position().x();
}

fixed bg_ptr::y() const
{
    return position().y();
}

const fixed_point& bg_ptr::position() const
{
    return bgs_manager::position(_id);
}

void bg_ptr::set_x(fixed x)
{
    set_position(fixed_point(x, position().y()));
}

void bg_ptr::set_y(fixed y)
{
    set_position(fixed_point(position().x(), y));
}

void bg_ptr::set_position(fixed x, fixed y)
{
    set_position(fixed_point(x, y));
}

void bg_ptr::set_position(const fixed_point& position)
{
    bgs_manager::set_position(_id, position);
}

int bg_ptr::priority() const
{
    return bgs_manager::priority(_id);
}

void bg_ptr::set_priority(int priority)
{
    bgs_manager::set_priority(_id, priority);
}

void bg_ptr::put_above(const bg_ptr& other)
{
    int this_id = id();
    int other_id = other.id();

    if(this_id != other_id)
    {
        int this_priority = priority();
        int other_priority = other.priority();

        if(this_priority >= other_priority)
        {
            if(this_id > other_id)
            {
                if(other_priority > 0)
                {
                    set_priority(other_priority - 1);
                }
            }
            else
            {
                set_priority(other_priority);
            }
        }
    }
}

void bg_ptr::put_above(const sprite_ptr& sprite_ptr)
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

bool bg_ptr::mosaic_enabled() const
{
    return bgs_manager::mosaic_enabled(_id);
}

void bg_ptr::set_mosaic_enabled(bool mosaic_enabled)
{
    bgs_manager::set_mosaic_enabled(_id, mosaic_enabled);
}

bool bg_ptr::visible() const
{
    return bgs_manager::visible(_id);
}

void bg_ptr::set_visible(bool visible)
{
    bgs_manager::set_visible(_id, visible);
}

bool bg_ptr::ignore_camera() const
{
    return bgs_manager::ignore_camera(_id);
}

void bg_ptr::set_ignore_camera(bool ignore_camera)
{
    bgs_manager::set_ignore_camera(_id, ignore_camera);
}

void bg_ptr::_destroy()
{
    bgs_manager::decrease_usages(_id);
}

}

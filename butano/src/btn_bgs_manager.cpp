#include "btn_bgs_manager.h"

#include "btn_size.h"
#include "btn_color.h"
#include "btn_camera.h"
#include "btn_algorithm.h"
#include "btn_bg_builder.h"
#include "btn_display_manager.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn::bgs_manager
{

namespace
{
    class item_type
    {

    public:
        fixed_point position;
        size dimensions;
        unsigned usages = 1;
        bg_tiles_ptr tiles_ptr;
        bg_map_ptr map_ptr;
        bg_palette_ptr palette_ptr;
        unsigned ignore_camera: 1;

        item_type(bg_builder&& builder, bg_tiles_ptr&& tiles, bg_map_ptr&& map, bg_palette_ptr&& palette,
                  hw::bgs::handle& handle) :
            position(builder.position()),
            dimensions(map.width(), map.height()),
            tiles_ptr(move(tiles)),
            map_ptr(move(map)),
            palette_ptr(move(palette))
        {
            hw::bgs::setup(builder, tiles_ptr.id(), map_ptr.id(), dimensions.width(), dimensions.height(),
                           palette_ptr.eight_bits_per_pixel(), handle);

            fixed_point real_position = -position;
            ignore_camera = builder.ignore_camera();

            if(! ignore_camera)
            {
                real_position += camera::position();
            }

            hw::bgs::set_position(real_position.x().integer(), real_position.y().integer(), handle);
        }
    };

    class static_data
    {

    public:
        optional<item_type> items[hw::bgs::count()];
        hw::bgs::handle handles[hw::bgs::count()];
        bool commit = false;
    };

    BTN_DATA_EWRAM static_data data;
}

int max_priority()
{
    return hw::bgs::max_priority();
}

optional<int> create(bg_builder&& builder)
{
    int new_index = 0;

    while(new_index < hw::bgs::count())
    {
        if(data.items[new_index])
        {
            ++new_index;
        }
        else
        {
            break;
        }
    }

    if(new_index == hw::bgs::count())
    {
        return nullopt;
    }

    optional<bg_tiles_ptr> tiles = builder.release_tiles();

    if(! tiles)
    {
        return nullopt;
    }

    optional<bg_map_ptr> map = builder.release_map();

    if(! map)
    {
        return nullopt;
    }

    optional<bg_palette_ptr> palette = builder.release_palette();

    if(! palette)
    {
        return nullopt;
    }

    bool visible = builder.visible();
    data.items[new_index] = item_type(move(builder), move(*tiles), move(*map), move(*palette), data.handles[new_index]);

    if(visible)
    {
        display_manager::set_bg_enabled(new_index, true);
        data.commit = true;
    }

    return new_index;
}

void increase_usages(int id)
{
    item_type& item = *data.items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = *data.items[id];
    --item.usages;

    if(! item.usages)
    {
        data.items[id].reset();

        if(display_manager::bg_enabled(id))
        {
            display_manager::set_bg_enabled(id, false);
            data.commit = true;
        }
    }
}

size dimensions(int id)
{
    item_type& item = *data.items[id];
    return item.dimensions * 8;
}

const bg_tiles_ptr& tiles_ptr(int id)
{
    item_type& item = *data.items[id];
    return item.tiles_ptr;
}

void set_tiles_ptr(int id, bg_tiles_ptr tiles_ptr)
{
    item_type& item = *data.items[id];
    BTN_ASSERT(item.tiles_ptr.tiles_count() == tiles_ptr.tiles_count(), "Invalid tiles count: ",
               item.tiles_ptr.tiles_count(), " - ", tiles_ptr.tiles_count());

    if(tiles_ptr != item.tiles_ptr)
    {
        hw::bgs::set_tiles(tiles_ptr.id(), data.handles[id]);
        item.tiles_ptr = move(tiles_ptr);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

const bg_map_ptr& map_ptr(int id)
{
    item_type& item = *data.items[id];
    return item.map_ptr;
}

void set_map_ptr(int id, bg_map_ptr map_ptr)
{
    item_type& item = *data.items[id];
    BTN_ASSERT(item.map_ptr.width() == map_ptr.width(), "Invalid map width: ",
               item.map_ptr.width(), " - ", map_ptr.width());
    BTN_ASSERT(item.map_ptr.height() == map_ptr.height(), "Invalid map height: ",
               item.map_ptr.height(), " - ", map_ptr.height());

    if(map_ptr != item.map_ptr)
    {
        hw::bgs::set_map(map_ptr.id(), data.handles[id]);
        item.map_ptr = move(map_ptr);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

const bg_palette_ptr& palette_ptr(int id)
{
    item_type& item = *data.items[id];
    return item.palette_ptr;
}

void set_palette_ptr(int id, bg_palette_ptr palette_ptr)
{
    item_type& item = *data.items[id];
    BTN_ASSERT(item.palette_ptr.eight_bits_per_pixel() == palette_ptr.eight_bits_per_pixel(),
               "Palette colors bpp mode mismatch: ",
               item.palette_ptr.eight_bits_per_pixel(), " - ", palette_ptr.eight_bits_per_pixel());

    if(palette_ptr != item.palette_ptr)
    {
        item.palette_ptr = move(palette_ptr);
    }
}

const fixed_point& position(int id)
{
    item_type& item = *data.items[id];
    return item.position;
}

void set_position(int id, const fixed_point& position)
{
    item_type& item = *data.items[id];
    fixed_point real_position = -position;

    if(! item.ignore_camera)
    {
        real_position += camera::position();
    }

    hw::bgs::set_position(real_position.x().integer(), real_position.y().integer(), data.handles[id]);
    item.position = position;

    if(display_manager::bg_enabled(id))
    {
        data.commit = true;
    }
}

int priority(int id)
{
    return hw::bgs::priority(data.handles[id]);
}

void set_priority(int id, int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= hw::bgs::max_priority(), "Invalid priority: ", priority);

    hw::bgs::set_priority(priority, data.handles[id]);

    if(display_manager::bg_enabled(id))
    {
        data.commit = true;
    }
}

bool mosaic_enabled(int id)
{
    return hw::bgs::mosaic_enabled(data.handles[id]);
}

void set_mosaic_enabled(int id, bool mosaic_enabled)
{
    hw::bgs::set_mosaic_enabled(mosaic_enabled, data.handles[id]);

    if(display_manager::bg_enabled(id))
    {
        data.commit = true;
    }
}

bool visible(int id)
{
    return display_manager::bg_enabled(id);
}

void set_visible(int id, bool visible)
{
    display_manager::set_bg_enabled(id, visible);

    if(visible)
    {
        data.commit = true;
    }
}

bool ignore_camera(int id)
{
    item_type& item = *data.items[id];
    return item.ignore_camera;
}

void set_ignore_camera(int id, bool ignore_camera)
{
    item_type& item = *data.items[id];
    item.ignore_camera = ignore_camera;
    set_position(id, item.position);
}

void update_camera()
{
    for(int index = 0; index < hw::bgs::count(); ++index)
    {
        if(optional<item_type>& item_cnt = data.items[index])
        {
            item_type& item = *item_cnt;

            if(! item.ignore_camera)
            {
                set_position(index, item.position);
            }
        }
    }
}

void commit()
{
    if(data.commit)
    {
        hw::bgs::commit(data.handles[0]);
        data.commit = false;
    }
}

}

#include "btn_bgs_manager.h"

#include "btn_color.h"
#include "btn_camera.h"
#include "btn_display.h"
#include "btn_algorithm.h"
#include "btn_display_manager.h"
#include "btn_regular_bg_attributes.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn::bgs_manager
{

namespace
{
    class item_type
    {

    public:
        fixed_point position;
        size quarter_dimensions;
        unsigned usages = 1;
        bg_tiles_ptr tiles_ptr;
        regular_bg_map_ptr map_ptr;
        unsigned ignore_camera: 1;

        item_type(regular_bg_builder&& builder, bg_tiles_ptr&& tiles, regular_bg_map_ptr&& map,
                  hw::bgs::handle& handle) :
            position(builder.position()),
            quarter_dimensions(map.dimensions()),
            tiles_ptr(move(tiles)),
            map_ptr(move(map)),
            ignore_camera(builder.ignore_camera())
        {
            hw::bgs::setup_regular(builder, handle);
            hw::bgs::set_tiles(tiles_ptr.id(), handle);
            hw::bgs::set_map(map_ptr.id(), quarter_dimensions, map_ptr.bpp_mode(), handle);
            update_quarter_dimensions(quarter_dimensions, handle);
        }

        void update_quarter_dimensions(const size& map_dimensions, hw::bgs::handle& handle)
        {
            quarter_dimensions = map_dimensions * 2;
            update_hw_position(handle);
        }

        fixed_point hw_position() const
        {
            fixed_point result = -position;

            if(! ignore_camera)
            {
                result += camera::position();
            }

            result.set_x(result.x() + quarter_dimensions.width() + 8);
            result.set_y(result.y() + quarter_dimensions.height() + 8);
            return result;
        }

        fixed_point hw_position(const fixed_point& camera_position) const
        {
            fixed_point result = -position;

            if(! ignore_camera)
            {
                result += camera_position;
            }

            result.set_x(result.x() + quarter_dimensions.width() + 8);
            result.set_y(result.y() + quarter_dimensions.height() + 8);
            return result;
        }

        void update_hw_position(hw::bgs::handle& handle)
        {
            fixed_point real_position = hw_position();
            hw::bgs::set_position(real_position.x().integer(), real_position.y().integer(), handle);
        }

        void update_hw_position(const fixed_point& camera_position, hw::bgs::handle& handle)
        {
            fixed_point real_position = hw_position(camera_position);
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

int used_count()
{
    int result = 0;

    for(const optional<item_type>& item : data.items)
    {
        if(item)
        {
            ++result;
        }
    }

    return result;
}

int available_count()
{
    return hw::bgs::count() - used_count();
}

int create(regular_bg_builder&& builder)
{
    int new_index = hw::bgs::count() - 1;

    while(new_index >= 0)
    {
        if(data.items[new_index])
        {
            --new_index;
        }
        else
        {
            break;
        }
    }

    if(new_index < 0)
    {
        return -1;
    }

    optional<bg_tiles_ptr> tiles = builder.release_tiles();

    if(! tiles)
    {
        return -1;
    }

    optional<regular_bg_map_ptr> map = builder.release_map();

    if(! map)
    {
        return -1;
    }

    bool blending_enabled = builder.blending_enabled();
    bool visible = builder.visible();
    data.items[new_index] = item_type(move(builder), move(*tiles), move(*map), data.handles[new_index]);

    if(visible)
    {
        display_manager::set_bg_enabled(new_index, true);
        data.commit = true;
    }

    if(blending_enabled)
    {
        display_manager::set_blending_bg_enabled(new_index, true);
    }

    display_manager::set_show_bg_in_all_windows(new_index, true);
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
        display_manager::set_blending_bg_enabled(id, false);

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
    return item.quarter_dimensions * 4;
}

const bg_tiles_ptr& tiles(int id)
{
    item_type& item = *data.items[id];
    return item.tiles_ptr;
}

void set_tiles(int id, const bg_tiles_ptr& tiles_ptr)
{
    item_type& item = *data.items[id];

    if(tiles_ptr != item.tiles_ptr)
    {
        BTN_ASSERT(tiles_ptr.valid_tiles_count(item.map_ptr.bpp_mode()), "Invalid tiles count: ",
                   tiles_ptr.tiles_count());

        hw::bgs::set_tiles(tiles_ptr.id(), data.handles[id]);
        item.tiles_ptr = tiles_ptr;

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

void set_tiles(int id, bg_tiles_ptr&& tiles_ptr)
{
    item_type& item = *data.items[id];

    if(tiles_ptr != item.tiles_ptr)
    {
        BTN_ASSERT(tiles_ptr.valid_tiles_count(item.map_ptr.bpp_mode()), "Invalid tiles count: ",
                   tiles_ptr.tiles_count());

        hw::bgs::set_tiles(tiles_ptr.id(), data.handles[id]);
        item.tiles_ptr = move(tiles_ptr);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

const regular_bg_map_ptr& map(int id)
{
    item_type& item = *data.items[id];
    return item.map_ptr;
}

void set_map(int id, const regular_bg_map_ptr& map_ptr)
{
    item_type& item = *data.items[id];

    if(map_ptr != item.map_ptr)
    {
        palette_bpp_mode bpp_mode = map_ptr.bpp_mode();
        BTN_ASSERT(item.tiles_ptr.valid_tiles_count(bpp_mode), "Invalid tiles count: ", item.tiles_ptr.tiles_count());

        hw::bgs::handle& handle = data.handles[id];
        size map_dimensions = map_ptr.dimensions();
        hw::bgs::set_map(map_ptr.id(), map_dimensions, bpp_mode, handle);
        item.map_ptr = map_ptr;
        item.update_quarter_dimensions(map_dimensions, handle);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

void set_map(int id, regular_bg_map_ptr&& map_ptr)
{
    item_type& item = *data.items[id];

    if(map_ptr != item.map_ptr)
    {
        palette_bpp_mode bpp_mode = map_ptr.bpp_mode();
        BTN_ASSERT(item.tiles_ptr.valid_tiles_count(bpp_mode), "Invalid tiles count: ", item.tiles_ptr.tiles_count());

        hw::bgs::handle& handle = data.handles[id];
        size map_dimensions = map_ptr.dimensions();
        hw::bgs::set_map(map_ptr.id(), map_dimensions, bpp_mode, handle);
        item.map_ptr = move(map_ptr);
        item.update_quarter_dimensions(map_dimensions, handle);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

void set_tiles_and_map(int id, bg_tiles_ptr&& tiles_ptr, regular_bg_map_ptr&& map_ptr)
{
    item_type& item = *data.items[id];
    bool different_tiles = tiles_ptr != item.tiles_ptr;
    bool different_map = map_ptr != item.map_ptr;

    if(different_tiles || different_map)
    {
        palette_bpp_mode bpp_mode = map_ptr.bpp_mode();
        BTN_ASSERT(tiles_ptr.valid_tiles_count(bpp_mode), "Invalid tiles count: ", tiles_ptr.tiles_count());

        hw::bgs::handle& handle = data.handles[id];

        if(different_tiles)
        {
            hw::bgs::set_tiles(tiles_ptr.id(), handle);
            item.tiles_ptr = move(tiles_ptr);
        }

        if(different_map)
        {
            size map_dimensions = map_ptr.dimensions();
            hw::bgs::set_map(map_ptr.id(), map_dimensions, bpp_mode, handle);
            item.map_ptr = move(map_ptr);
            item.update_quarter_dimensions(map_dimensions, handle);
        }

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

const fixed_point& position(int id)
{
    item_type& item = *data.items[id];
    return item.position;
}

fixed_point hw_position(int id)
{
    item_type& item = *data.items[id];
    return item.hw_position();
}

void set_position(int id, const fixed_point& position)
{
    item_type& item = *data.items[id];
    item.position = position;
    item.update_hw_position(data.handles[id]);

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

regular_bg_attributes attributes(int id)
{
    return regular_bg_attributes(priority(id), mosaic_enabled(id));
}

void set_attributes(int id, const regular_bg_attributes& attributes)
{
    hw::bgs::handle& handle = data.handles[id];
    hw::bgs::set_priority(attributes.priority(), handle);
    hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), handle);

    if(display_manager::bg_enabled(id))
    {
        data.commit = true;
    }
}

bool blending_enabled(int id)
{
    return display_manager::blending_bg_enabled(id);
}

void set_blending_enabled(int id, bool blending_enabled)
{
    display_manager::set_blending_bg_enabled(id, blending_enabled);
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
    item.update_hw_position(data.handles[id]);

    if(display_manager::bg_enabled(id))
    {
        data.commit = true;
    }
}

void update_map_palette_bpp_mode(int map_id, palette_bpp_mode new_bpp_mode)
{
    for(int id = 0; id < hw::bgs::count(); ++id)
    {
        if(optional<item_type>& item_cnt = data.items[id])
        {
            item_type& item = *item_cnt;

            if(item.map_ptr.id() == map_id)
            {
                BTN_ASSERT(item.tiles_ptr.valid_tiles_count(new_bpp_mode), "Invalid tiles count: ",
                           item.tiles_ptr.tiles_count());

                hw::bgs::set_bpp_mode(new_bpp_mode, data.handles[id]);

                if(display_manager::bg_enabled(id))
                {
                    data.commit = true;
                }
            }
        }
    }
}

void update_camera()
{
    fixed_point camera_position = camera::position();

    for(int id = 0; id < hw::bgs::count(); ++id)
    {
        if(optional<item_type>& item_cnt = data.items[id])
        {
            item_type& item = *item_cnt;

            if(! item.ignore_camera)
            {
                item.update_hw_position(camera_position, data.handles[id]);

                if(display_manager::bg_enabled(id))
                {
                    data.commit = true;
                }
            }
        }
    }
}

void fill_hblank_effect_horizontal_positions(fixed base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        dest_ptr[index] = uint16_t((base_position + positions_ptr[index]).integer());
    }
}

void fill_hblank_effect_vertical_positions(fixed base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        dest_ptr[index] = uint16_t((base_position + positions_ptr[index]).integer());
    }
}

void fill_hblank_effect_attributes(int id, const regular_bg_attributes* attributes_ptr, uint16_t* dest_ptr)
{
    uint16_t bg_cnt = data.handles[id].cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const regular_bg_attributes& attributes = attributes_ptr[index];
        uint16_t& dest_cnt = dest_ptr[index];
        dest_cnt = bg_cnt;
        hw::bgs::set_priority(attributes.priority(), dest_cnt);
        hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), dest_cnt);
    }
}

void commit()
{
    if(data.commit)
    {
        hw::bgs::commit(data.handles);
        data.commit = false;
    }
}

}

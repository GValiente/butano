#include "btn_bgs_manager.h"

#include "btn_bgs.h"
#include "btn_color.h"
#include "btn_camera.h"
#include "btn_display.h"
#include "btn_algorithm.h"
#include "btn_bg_tiles_ptr.h"
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
        regular_bg_map_ptr map;
        bool ignore_camera;

        item_type(const regular_bg_builder& builder, regular_bg_map_ptr&& _map, hw::bgs::handle& handle) :
            position(builder.position()),
            map(move(_map)),
            ignore_camera(builder.ignore_camera())
        {
            hw::bgs::setup_regular(builder, handle);
            hw::bgs::set_tiles_cbb(map.tiles().cbb(), handle);
            update_map(handle);
        }

        void update_map(hw::bgs::handle& handle)
        {
            size map_dimensions = map.dimensions();
            hw::bgs::set_map_sbb(map.id(), handle);
            hw::bgs::set_bpp_mode(map.bpp_mode(), handle);
            hw::bgs::set_map_dimensions(map_dimensions, handle);
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


    void _create_finish(int new_index, bool visible, bool blending_enabled)
    {
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
    }

    void _set_regular_attributes(const regular_bg_map_ptr& current_map, const regular_bg_attributes& attributes,
                                 uint16_t& bg_cnt)
    {
        const regular_bg_map_ptr& attributes_map = attributes.map();
        BTN_ASSERT(current_map.dimensions() == attributes_map.dimensions(), "Map dimensions mismatch");

        hw::bgs::set_tiles_cbb(attributes_map.tiles().cbb(), bg_cnt);
        hw::bgs::set_map_sbb(attributes_map.id(), bg_cnt);
        hw::bgs::set_bpp_mode(attributes_map.bpp_mode(), bg_cnt);
        hw::bgs::set_priority(attributes.priority(), bg_cnt);
        hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), bg_cnt);
    }
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

    BTN_ASSERT(new_index >= 0, "No more available bgs");

    data.items[new_index] = item_type(builder, builder.release_map(), data.handles[new_index]);
    _create_finish(new_index, builder.visible(), builder.blending_enabled());
    return new_index;
}

int optional_create(regular_bg_builder&& builder)
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

    optional<regular_bg_map_ptr> map = builder.optional_release_map();

    if(! map)
    {
        return -1;
    }

    data.items[new_index] = item_type(builder, move(*map), data.handles[new_index]);
    _create_finish(new_index, builder.visible(), builder.blending_enabled());
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

const regular_bg_map_ptr& map(int id)
{
    item_type& item = *data.items[id];
    return item.map;
}

void set_map(int id, const regular_bg_map_ptr& map)
{
    item_type& item = *data.items[id];

    if(map != item.map)
    {
        item.map = map;
        item.update_map(data.handles[id]);

        if(display_manager::bg_enabled(id))
        {
            data.commit = true;
        }
    }
}

void set_map(int id, regular_bg_map_ptr&& map)
{
    item_type& item = *data.items[id];

    if(map != item.map)
    {
        item.map = move(map);
        item.update_map(data.handles[id]);

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
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

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

regular_bg_attributes regular_attributes(int id)
{
    return regular_bg_attributes(map(id), priority(id), mosaic_enabled(id));
}

void set_regular_attributes(int id, const regular_bg_attributes& attributes)
{
    item_type& item = *data.items[id];
    hw::bgs::handle& handle = data.handles[id];
    _set_regular_attributes(item.map, attributes, handle.cnt);
    item.map = attributes.map();

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

void update_map_tiles_cbb(int map_id, int tiles_cbb)
{
    for(int id = 0; id < hw::bgs::count(); ++id)
    {
        if(optional<item_type>& item_cnt = data.items[id])
        {
            item_type& item = *item_cnt;

            if(item.map.id() == map_id)
            {
                hw::bgs::set_tiles_cbb(tiles_cbb, data.handles[id]);

                if(display_manager::bg_enabled(id))
                {
                    data.commit = true;
                }
            }
        }
    }
}

void update_map_palette_bpp_mode(int map_id, palette_bpp_mode new_bpp_mode)
{
    for(int id = 0; id < hw::bgs::count(); ++id)
    {
        if(optional<item_type>& item_cnt = data.items[id])
        {
            item_type& item = *item_cnt;

            if(item.map.id() == map_id)
            {
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
    if(base_position == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(positions_ptr[index].integer());
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t((base_position + positions_ptr[index]).integer());
        }
    }
}

void fill_hblank_effect_vertical_positions(fixed base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    if(base_position == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(positions_ptr[index].integer());
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t((base_position + positions_ptr[index]).integer());
        }
    }
}

void fill_hblank_effect_regular_attributes(int id, const regular_bg_attributes* attributes_ptr, uint16_t* dest_ptr)
{
    const regular_bg_map_ptr& map = data.items[id]->map;
    uint16_t bg_cnt = data.handles[id].cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const regular_bg_attributes& attributes = attributes_ptr[index];
        uint16_t& dest_cnt = dest_ptr[index];
        dest_cnt = bg_cnt;
        _set_regular_attributes(map, attributes, dest_cnt);
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

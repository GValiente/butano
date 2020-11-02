/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_bgs_manager.h"

#include "btn_bgs.h"
#include "btn_pool.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_sort_key.h"
#include "btn_algorithm.h"
#include "btn_camera_ptr.h"
#include "btn_config_bgs.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_display_manager.h"
#include "btn_regular_bg_attributes.h"
#include "../hw/include/btn_hw_bgs.h"

namespace btn::bgs_manager
{

namespace
{
    static_assert(BTN_CFG_BGS_MAX_ITEMS > 0);

    class item_type
    {

    public:
        fixed_point position;
        point hw_position;
        size half_dimensions;
        unsigned usages = 1;
        sort_key bg_sort_key;
        hw::bgs::handle handle;
        optional<regular_bg_map_ptr> map;
        optional<camera_ptr> camera;
        int8_t handles_index = -1;
        bool blending_enabled: 1;
        bool visible: 1;
        bool update: 1;

        item_type(regular_bg_builder&& builder, regular_bg_map_ptr&& _map) :
            position(builder.position()),
            bg_sort_key(builder.priority(), builder.z_order()),
            map(move(_map)),
            camera(builder.release_camera()),
            blending_enabled(builder.blending_enabled()),
            visible(builder.visible()),
            update(true)
        {
            hw::bgs::setup_regular(builder, handle);
            update_map();
        }

        void update_map()
        {
            const regular_bg_map_ptr& map_ref = *map;
            size map_dimensions = map_ref.dimensions();
            hw::bgs::handle new_handle = handle;
            hw::bgs::set_tiles_cbb(map_ref.tiles().cbb(), new_handle);
            hw::bgs::set_map_sbb(map_ref.id(), new_handle);
            hw::bgs::set_bpp_mode(map_ref.bpp_mode(), new_handle);
            hw::bgs::set_map_dimensions(map_dimensions, new_handle);
            handle = new_handle;
            half_dimensions = map_dimensions * 4;
            update_hw_position();
        }

        void update_hw_position()
        {
            int real_x = position.x().right_shift_integer();
            int real_y = position.y().right_shift_integer();

            if(camera)
            {
                const fixed_point& camera_position = camera->position();
                real_x -= camera_position.x().right_shift_integer();
                real_y -= camera_position.y().right_shift_integer();
            }

            update_hw_x(real_x);
            update_hw_y(real_y);
        }

        void update_hw_x(int real_x)
        {
            int hw_x = -real_x - (display::width() / 2) + half_dimensions.width();
            hw_position.set_x(hw_x);
            hw::bgs::set_x(hw_x, handle);
        }

        void update_hw_y(int real_y)
        {
            int hw_y = -real_y - (display::height() / 2) + half_dimensions.height();
            hw_position.set_y(hw_y);
            hw::bgs::set_y(hw_y, handle);
        }
    };


    class static_data
    {

    public:
        pool<item_type, BTN_CFG_BGS_MAX_ITEMS> items_pool;
        vector<item_type*, BTN_CFG_BGS_MAX_ITEMS> items_vector;
        hw::bgs::handle handles[hw::bgs::count()];
        bool rebuild_handles = false;
        bool commit = false;
    };

    BTN_DATA_EWRAM static_data data;


    void _insert_item(item_type& new_item)
    {
        sort_key bg_sort_key = new_item.bg_sort_key;

        if(new_item.visible)
        {
            data.rebuild_handles = true;
        }

        for(auto it = data.items_vector.begin(), end = data.items_vector.end(); it != end; ++it)
        {
            const item_type* item = *it;

            if(bg_sort_key > item->bg_sort_key)
            {
                data.items_vector.insert(it, &new_item);
                return;
            }
        }

        data.items_vector.push_back(&new_item);
    }

    void _update_item(const item_type& item)
    {
        if(! data.rebuild_handles && item.visible)
        {
            data.handles[item.handles_index] = item.handle;
            data.commit = true;
        }
    }

    pair<int, int> _indexes(id_type id, id_type other_id)
    {
        auto item = static_cast<const item_type*>(id);
        auto other_item = static_cast<const item_type*>(other_id);
        int index = -1;
        int other_index = -1;

        for(int current_index = 0, limit = data.items_vector.size(); current_index < limit; ++current_index)
        {
            const item_type* current_item = data.items_vector[current_index];

            if(current_item == item)
            {
                index = current_index;
            }
            else if(current_item == other_item)
            {
                other_index = current_index;
            }
        }

        return make_pair(index, other_index);
    }
}

int used_count()
{
    return data.items_vector.size();
}

int available_count()
{
    return data.items_vector.available();
}

id_type create(regular_bg_builder&& builder)
{
    BTN_ASSERT(! data.items_vector.full(), "No more available bgs");

    regular_bg_map_ptr map = builder.release_map();
    item_type& item = data.items_pool.create(move(builder), move(map));
    _insert_item(item);
    display_manager::set_show_bg_in_all_windows(&item, true);
    return &item;
}

id_type create_optional(regular_bg_builder&& builder)
{
    if(data.items_vector.full())
    {
        return nullptr;
    }

    optional<regular_bg_map_ptr> map = builder.release_map_optional();

    if(! map)
    {
        return nullptr;
    }

    item_type& item = data.items_pool.create(move(builder), move(*map));
    _insert_item(item);
    display_manager::set_show_bg_in_all_windows(&item, true);
    return &item;
}

void increase_usages(id_type id)
{
    auto item = static_cast<item_type*>(id);
    ++item->usages;
}

void decrease_usages(id_type id)
{
    auto item = static_cast<item_type*>(id);
    --item->usages;

    if(! item->usages)
    {
        if(! data.rebuild_handles && item->visible)
        {
            if(! data.items_vector.empty() && item != data.items_vector.back())
            {
                data.rebuild_handles = true;
            }
            else
            {
                display_manager::set_bg_enabled(item->handles_index, false);
            }
        }

        display_manager::set_show_bg_in_all_windows(item, false);
        erase(data.items_vector, item);
        data.items_pool.destroy(*item);
    }
}

optional<int> hw_id(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    int handles_index = item->handles_index;
    optional<int> result;

    if(handles_index >= 0)
    {
        result = handles_index;
    }

    return result;
}

size dimensions(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->half_dimensions * 2;
}

const regular_bg_map_ptr& map(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->map;
}

void set_map(id_type id, const regular_bg_map_ptr& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->map)
    {
        item->map = map;
        item->update_map();
        _update_item(*item);
    }
}

void set_map(id_type id, regular_bg_map_ptr&& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->map)
    {
        item->map = move(map);
        item->update_map();
        _update_item(*item);
    }
}

void remove_map(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->map.reset();
}

const fixed_point& position(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->position;
}

const point& hw_position(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->hw_position;
}

void set_x(id_type id, fixed x)
{
    auto item = static_cast<item_type*>(id);
    fixed old_x = item->position.x();
    item->position.set_x(x);

    int old_integer_x = old_x.right_shift_integer();
    int new_integer_x = x.right_shift_integer();
    int diff = new_integer_x - old_integer_x;

    if(diff)
    {
        int hw_x = item->hw_position.x() - diff;
        item->hw_position.set_x(hw_x);
        hw::bgs::set_x(hw_x, item->handle);
        _update_item(*item);
    }
}

void set_y(id_type id, fixed y)
{
    auto item = static_cast<item_type*>(id);
    fixed old_y = item->position.y();
    item->position.set_y(y);

    int old_integer_y = old_y.right_shift_integer();
    int new_integer_y = y.right_shift_integer();
    int diff = new_integer_y - old_integer_y;

    if(diff)
    {
        int hw_y = item->hw_position.y() - diff;
        item->hw_position.set_y(hw_y);
        hw::bgs::set_y(hw_y, item->handle);
        _update_item(*item);
    }
}

void set_position(id_type id, const fixed_point& position)
{
    auto item = static_cast<item_type*>(id);
    fixed_point old_position = item->position;
    item->position = position;

    point old_integer_position(old_position.x().right_shift_integer(), old_position.y().right_shift_integer());
    point new_integer_position(position.x().right_shift_integer(), position.y().right_shift_integer());
    point diff = new_integer_position - old_integer_position;

    if(diff != point())
    {
        point hw_position = item->hw_position - diff;
        item->hw_position = hw_position;

        hw::bgs::handle& handle = item->handle;
        hw::bgs::set_x(hw_position.x(), handle);
        hw::bgs::set_y(hw_position.y(), handle);
        _update_item(*item);
    }
}

int priority(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->bg_sort_key.priority();
}

void set_priority(id_type id, int priority)
{
    BTN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

    auto item = static_cast<item_type*>(id);

    if(item->bg_sort_key.priority() != priority)
    {
        item->bg_sort_key.set_priority(priority);
        hw::bgs::set_priority(priority, item->handle);
        erase(data.items_vector, item);
        _insert_item(*item);
    }
}

int z_order(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->bg_sort_key.z_order();
}

void set_z_order(id_type id, int z_order)
{
    auto item = static_cast<item_type*>(id);

    if(item->bg_sort_key.z_order() != z_order)
    {
        item->bg_sort_key.set_z_order(z_order);
        erase(data.items_vector, item);
        _insert_item(*item);
    }
}

bool above(id_type id, id_type other_id)
{
    auto item = static_cast<const item_type*>(id);
    auto other_item = static_cast<const item_type*>(other_id);
    sort_key this_sort_key = item->bg_sort_key;
    sort_key other_sort_key = other_item->bg_sort_key;

    if(this_sort_key < other_sort_key)
    {
        return true;
    }

    if(this_sort_key > other_sort_key)
    {
        return false;
    }

    pair<int, int> indexes = _indexes(id, other_id);
    return indexes.first > indexes.second;
}

void put_above(id_type id, id_type other_id)
{
    auto item = static_cast<item_type*>(id);
    auto other_item = static_cast<item_type*>(other_id);
    sort_key this_sort_key = item->bg_sort_key;
    sort_key other_sort_key = other_item->bg_sort_key;

    if(this_sort_key == other_sort_key)
    {
        pair<int, int> indexes = _indexes(id, other_id);

        if(indexes.first < indexes.second)
        {
            data.items_vector.erase(data.items_vector.begin() + indexes.first);
            data.items_vector.push_back(item);

            if(item->visible)
            {
                data.rebuild_handles = true;
            }
        }
    }
    else if(this_sort_key > other_sort_key)
    {
        int other_z_order = other_sort_key.z_order();
        set_priority(id, other_sort_key.priority());

        if(this_sort_key.z_order() > other_z_order)
        {
            set_z_order(id, other_z_order);
        }
    }
}

bool mosaic_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::bgs::mosaic_enabled(item->handle);
}

void set_mosaic_enabled(id_type id, bool mosaic_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::bgs::set_mosaic_enabled(mosaic_enabled, item->handle);
    _update_item(*item);
}

regular_bg_attributes regular_attributes(id_type id)
{
    return regular_bg_attributes(map(id), priority(id), mosaic_enabled(id));
}

void set_regular_attributes(id_type id, const regular_bg_attributes& attributes)
{
    set_map(id, attributes.map());
    set_priority(id, attributes.priority());
    set_mosaic_enabled(id, attributes.mosaic_enabled());
}

bool blending_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->blending_enabled;
}

void set_blending_enabled(id_type id, bool blending_enabled)
{
    auto item = static_cast<item_type*>(id);
    item->blending_enabled = blending_enabled;

    if(! data.rebuild_handles && item->visible)
    {
        display_manager::set_blending_bg_enabled(item->handles_index, blending_enabled);
    }
}

bool visible(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->visible;
}

void set_visible(id_type id, bool visible)
{
    auto item = static_cast<item_type*>(id);

    if(visible != item->visible)
    {
        item->visible = visible;
        data.rebuild_handles = true;
    }
}

const optional<camera_ptr>& camera(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->camera;
}

void set_camera(id_type id, camera_ptr&& camera)
{
    auto item = static_cast<item_type*>(id);

    if(camera != item->camera)
    {
        item->camera = move(camera);
        item->update_hw_position();
        _update_item(*item);
    }
}

void remove_camera(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->camera)
    {
        item->camera.reset();
        item->update_hw_position();
        _update_item(*item);
    }
}

void update_cameras()
{
    for(item_type* item : data.items_vector)
    {
        if(item->camera)
        {
            item->update_hw_position();
            _update_item(*item);
        }
    }
}

void update_map_tiles_cbb(int map_id, int tiles_cbb)
{
    for(item_type* item : data.items_vector)
    {
        if(item->map->id() == map_id)
        {
            hw::bgs::set_tiles_cbb(tiles_cbb, item->handle);
            _update_item(*item);
        }
    }
}

void update_map_palette_bpp_mode(int map_id, palette_bpp_mode new_bpp_mode)
{
    for(item_type* item : data.items_vector)
    {
        if(item->map->id() == map_id)
        {
            hw::bgs::set_bpp_mode(new_bpp_mode, item->handle);
            _update_item(*item);
        }
    }
}

void reload()
{
    data.commit = true;
}

void fill_hblank_effect_horizontal_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    if(base_position == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(positions_ptr[index].right_shift_integer());
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(base_position + positions_ptr[index].right_shift_integer());
        }
    }
}

void fill_hblank_effect_vertical_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    if(base_position == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(positions_ptr[index].right_shift_integer());
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            dest_ptr[index] = uint16_t(base_position + positions_ptr[index].right_shift_integer());
        }
    }
}

void fill_hblank_effect_regular_attributes(id_type id, const regular_bg_attributes* attributes_ptr, uint16_t* dest_ptr)
{
    auto item = static_cast<item_type*>(id);
    [[maybe_unused]] btn::size current_dimensions = item->map->dimensions();
    uint16_t bg_cnt = item->handle.cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const regular_bg_attributes& attributes = attributes_ptr[index];
        const regular_bg_map_ptr& attributes_map = attributes.map();
        BTN_ASSERT(current_dimensions == attributes_map.dimensions(), "Map dimensions mismatch");

        uint16_t dest_cnt = bg_cnt;
        hw::bgs::set_tiles_cbb(attributes_map.tiles().cbb(), dest_cnt);
        hw::bgs::set_map_sbb(attributes_map.id(), dest_cnt);
        hw::bgs::set_bpp_mode(attributes_map.bpp_mode(), dest_cnt);
        hw::bgs::set_priority(attributes.priority(), dest_cnt);
        hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), dest_cnt);
        dest_ptr[index] = dest_cnt;
    }
}

void update()
{
    if(data.rebuild_handles)
    {
        int id = hw::bgs::count() - 1;
        data.rebuild_handles = false;
        data.commit = true;

        for(item_type* item : data.items_vector)
        {
            if(item->visible)
            {
                BTN_ASSERT(BTN_CFG_BGS_MAX_ITEMS <= hw::bgs::count() || id >= 0, "Too much bgs on screen");

                item->handles_index = int8_t(id);
                data.handles[id] = item->handle;
                display_manager::set_bg_enabled(id, true);
                display_manager::set_blending_bg_enabled(id, item->blending_enabled);
                --id;
            }
            else
            {
                item->handles_index = -1;
            }
        }

        for(int index = 0; index <= id; ++index)
        {
            display_manager::set_bg_enabled(index, false);
        }

        display_manager::update_windows_visible_bgs();
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

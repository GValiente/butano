/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bgs_manager.h"

#include "bn_math.h"
#include "bn_pool.h"
#include "bn_vector.h"
#include "bn_display.h"
#include "bn_sort_key.h"
#include "bn_config_bgs.h"
#include "bn_display_manager.h"
#include "bn_bg_blocks_manager.h"
#include "../hw/include/bn_hw_bgs.h"

#include "bn_bgs.cpp.h"
#include "bn_regular_bg_ptr.cpp.h"
#include "bn_regular_bg_item.cpp.h"
#include "bn_regular_bg_builder.cpp.h"
#include "bn_regular_bg_attributes.cpp.h"

namespace bn::bgs_manager
{

namespace
{
    static_assert(BN_CFG_BGS_MAX_ITEMS > 0);

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
        uint16_t old_big_map_x = 0;
        uint16_t old_big_map_y = 0;
        int8_t handles_index = -1;
        bool blending_enabled: 1;
        bool visible: 1;
        bool update: 1;
        bool big_map: 1;
        bool commit_big_map: 1;
        bool full_commit_big_map: 1;

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
            hw::bgs::handle new_handle = handle;
            hw::bgs::set_tiles_cbb(map_ref.tiles().cbb(), new_handle);
            hw::bgs::set_map_sbb(map_ref.id(), new_handle);
            hw::bgs::set_bpp_mode(map_ref.bpp_mode(), new_handle);

            size map_dimensions = map_ref.dimensions();
            int map_width = map_dimensions.width();
            int map_height = map_dimensions.height();
            int map_size = 0;
            big_map = true;

            if(map_width == 32 || map_width == 64)
            {
                if(map_height == 32 || map_height == 64)
                {
                    big_map = false;

                    if(map_width == 64)
                    {
                        ++map_size;
                    }

                    if(map_height == 64)
                    {
                        map_size += 2;
                    }
                }
            }

            hw::bgs::set_map_dimensions(map_size, new_handle);
            handle = new_handle;
            half_dimensions = map_dimensions * 4;
            update_hw_position();
            commit_big_map = big_map;
            full_commit_big_map = big_map;
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
            commit_hw_x();
        }

        void update_hw_y(int real_y)
        {
            int hw_y = -real_y - (display::height() / 2) + half_dimensions.height();
            hw_position.set_y(hw_y);
            commit_hw_y();
        }

        void commit_hw_x()
        {
            int x = hw_position.x();
            hw::bgs::set_x(x, handle);

            if(big_map)
            {
                BN_ASSERT(x >= 0 && x < (half_dimensions.width() * 2) - display::width(),
                          "Regular BGs with big maps\ndon't allow horizontal wrapping: ",
                          x, " - ", (half_dimensions.width() * 2) - display::width());

                commit_big_map = true;
            }
        }

        void commit_hw_y()
        {
            int y = hw_position.y();
            hw::bgs::set_y(y, handle);

            if(big_map)
            {
                BN_ASSERT(y >= 0 && y < (half_dimensions.height() * 2) - display::height(),
                          "Regular BGs with big maps\ndon't allow vertical wrapping: ",
                          y, " - ", (half_dimensions.height() * 2) - display::height());

                commit_big_map = true;
            }
        }
    };


    class static_data
    {

    public:
        pool<item_type, BN_CFG_BGS_MAX_ITEMS> items_pool;
        vector<item_type*, BN_CFG_BGS_MAX_ITEMS> items_vector;
        hw::bgs::handle handles[hw::bgs::count()];
        bool rebuild_handles = false;
        bool commit = false;
    };

    BN_DATA_EWRAM static_data data;


    [[nodiscard]] bool _check_unique_big_map(item_type& item)
    {
        if(item.big_map)
        {
            for(item_type* other_item : data.items_vector)
            {
                if(other_item != &item && other_item->map == item.map)
                {
                    return false;
                }
            }
        }

        return true;
    }

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
    BN_ASSERT(! data.items_vector.full(), "No more available BGs");

    regular_bg_map_ptr map = builder.release_map();
    item_type& item = data.items_pool.create(move(builder), move(map));
    BN_ASSERT(_check_unique_big_map(item), "Two or more BGs have the same big map");

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
    BN_ASSERT(_check_unique_big_map(item), "Two or more BGs have the same big map");

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
        BN_ASSERT(_check_unique_big_map(*item), "Two or more BGs have the same big map");

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
        BN_ASSERT(_check_unique_big_map(*item), "Two or more BGs have the same big map");

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
        item->hw_position.set_x(item->hw_position.x() - diff);
        item->commit_hw_x();
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
        item->hw_position.set_y(item->hw_position.y() - diff);
        item->commit_hw_y();
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
        item->hw_position -= diff;
        item->commit_hw_x();
        item->commit_hw_y();
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
    BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

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

void put_above(id_type id)
{
    auto item = static_cast<item_type*>(id);
    sort_key this_sort_key = item->bg_sort_key;
    bool order_modified = false;

    for(int index = 0, limit = data.items_vector.size() - 1; index < limit; ++index)
    {
        item_type*& current_item_ptr = data.items_vector[index];

        if(current_item_ptr == item)
        {
            item_type*& next_item_ptr = data.items_vector[index + 1];

            if(next_item_ptr->bg_sort_key == this_sort_key)
            {
                swap(current_item_ptr, next_item_ptr);
                order_modified = true;
            }
            else
            {
                break;
            }
        }
    }

    if(order_modified && item->visible)
    {
        data.rebuild_handles = true;
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
    [[maybe_unused]] bn::size current_dimensions = item->map->dimensions();
    uint16_t bg_cnt = item->handle.cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const regular_bg_attributes& attributes = attributes_ptr[index];
        const regular_bg_map_ptr& attributes_map = attributes.map();
        BN_ASSERT(current_dimensions == attributes_map.dimensions(), "Map dimensions mismatch");

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
                BN_ASSERT(BN_CFG_BGS_MAX_ITEMS <= hw::bgs::count() || id >= 0, "Too much BGs on screen");

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

void commit_big_maps()
{
    for(item_type* item : data.items_vector)
    {
        if(item->big_map)
        {
            regular_bg_map_ptr& map = *item->map;
            int map_handle = map.handle();
            int old_map_x = item->old_big_map_x;
            int old_map_y = item->old_big_map_y;
            int new_map_x = item->hw_position.x() / 8;
            int new_map_y = item->hw_position.y() / 8;
            bool full_commit_big_map = item->full_commit_big_map || bg_blocks_manager::must_commit(map_handle);
            bool commit_big_map = full_commit_big_map;

            if(! commit_big_map && item->commit_big_map && item->visible)
            {
                commit_big_map = old_map_x != new_map_x || old_map_y != new_map_y;
            }

            if(commit_big_map)
            {
                item->old_big_map_x = new_map_x;
                item->old_big_map_y = new_map_y;
                item->commit_big_map = false;
                item->full_commit_big_map = false;

                if(full_commit_big_map || bn::abs(new_map_x - old_map_x) > 1 || bn::abs(new_map_y - old_map_y) > 1)
                {
                    bg_blocks_manager::set_regular_map_position(map_handle, new_map_x, new_map_y);
                }
                else
                {
                    if(new_map_x < old_map_x)
                    {
                        bg_blocks_manager::update_regular_map_col(map_handle, new_map_x, new_map_y);
                    }
                    else if(new_map_x > old_map_x)
                    {
                        bg_blocks_manager::update_regular_map_col(map_handle, new_map_x + 31, new_map_y);
                    }

                    if(new_map_y < old_map_y)
                    {
                        bg_blocks_manager::update_regular_map_row(map_handle, new_map_x, new_map_y);
                    }
                    else if(new_map_y > old_map_y)
                    {
                        bg_blocks_manager::update_regular_map_row(map_handle, new_map_x, new_map_y + 21);
                    }
                }
            }
        }
    }
}

}

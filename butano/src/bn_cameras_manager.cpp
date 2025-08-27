/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_cameras_manager.h"

#include "bn_limits.h"
#include "bn_config_cameras.h"
#include "bn_bgs_manager.h"
#include "bn_sprites_manager.h"
#include "bn_display_manager.h"

#include "bn_cameras.cpp.h"
#include "bn_camera_ptr.cpp.h"

namespace bn::cameras_manager
{

namespace
{
    constexpr int max_items = BN_CFG_CAMERA_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= numeric_limits<uint8_t>::max());


    class item_type
    {

    public:
        fixed_point position;
        unsigned usages = 0;
    };


    class static_data
    {

    public:
        item_type items[max_items];
        alignas(int) uint8_t free_item_indexes_array[max_items];
        uint16_t free_item_indexes_size = max_items;
        bool update = false;
    };

    alignas(static_data) BN_DATA_EWRAM_BSS char data_buffer[sizeof(static_data)];

    [[nodiscard]] static_data& data_ref()
    {
        return *reinterpret_cast<static_data*>(data_buffer);
    }
}

void init()
{
    ::new(static_cast<void*>(data_buffer)) static_data();

    static_data& data = data_ref();

    for(int index = 0; index < max_items; ++index)
    {
        data.free_item_indexes_array[index] = uint8_t(index);
    }
}

int used_items_count()
{
    return max_items - data_ref().free_item_indexes_size;
}

int available_items_count()
{
    return data_ref().free_item_indexes_size;
}

int create(const fixed_point& position)
{
    static_data& data = data_ref();
    BN_BASIC_ASSERT(data.free_item_indexes_size, "No more cameras available");

    --data.free_item_indexes_size;

    int item_index = data.free_item_indexes_array[data.free_item_indexes_size];
    item_type& new_item = data.items[item_index];
    new_item.position = position;
    new_item.usages = 1;
    return item_index;
}

int create_optional(const fixed_point& position)
{
    static_data& data = data_ref();

    if(! data.free_item_indexes_size)
    {
        return -1;
    }

    --data.free_item_indexes_size;

    int item_index = data.free_item_indexes_array[data.free_item_indexes_size];
    item_type& new_item = data.items[item_index];
    new_item.position = position;
    new_item.usages = 1;
    return item_index;
}

void increase_usages(int id)
{
    item_type& item = data_ref().items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    static_data& data = data_ref();
    item_type& item = data.items[id];
    --item.usages;

    if(! item.usages) [[unlikely]]
    {
        data.free_item_indexes_array[data.free_item_indexes_size] = uint8_t(id);
        ++data.free_item_indexes_size;
    }
}

const fixed_point& position(int id)
{
    const item_type& item = data_ref().items[id];
    return item.position;
}

void set_x(int id, fixed x)
{
    static_data& data = data_ref();
    item_type& item = data.items[id];

    if(item.position.x() != x)
    {
        item.position.set_x(x);
        data.update = true;
    }
}

void set_y(int id, fixed y)
{
    static_data& data = data_ref();
    item_type& item = data.items[id];

    if(item.position.y() != y)
    {
        item.position.set_y(y);
        data.update = true;
    }
}

void set_position(int id, const fixed_point& position)
{
    static_data& data = data_ref();
    item_type& item = data.items[id];

    if(item.position != position)
    {
        item.position = position;
        data.update = true;
    }
}

void update()
{
    static_data& data = data_ref();

    if(data.update)
    {
        data.update = false;

        display_manager::update_cameras();
        sprites_manager::update_cameras();
        bgs_manager::update_cameras();
    }
}

}

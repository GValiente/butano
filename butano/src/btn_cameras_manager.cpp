/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_cameras_manager.h"

#include "btn_vector.h"
#include "btn_config_cameras.h"
#include "btn_bgs_manager.h"
#include "btn_sprites_manager.h"
#include "btn_display_manager.h"

#include "btn_cameras.cpp.h"
#include "btn_camera_ptr.cpp.h"

namespace btn::cameras_manager
{

namespace
{
    constexpr const int max_items = BTN_CFG_CAMERA_MAX_ITEMS;

    static_assert(max_items > 0 && max_items <= numeric_limits<int8_t>::max());


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
        vector<int8_t, max_items> free_item_indexes;
        bool update = false;
    };

    BTN_DATA_EWRAM static_data data;
}

void init()
{
    for(int index = max_items - 1; index >= 0; --index)
    {
        data.free_item_indexes.push_back(index);
    }
}

int used_items_count()
{
    return data.free_item_indexes.available();
}

int available_items_count()
{
    return data.free_item_indexes.size();
}

int create(const fixed_point& position)
{
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more camera items available");

    int item_index = data.free_item_indexes.back();
    data.free_item_indexes.pop_back();

    item_type& new_item = data.items[item_index];
    new_item.position = position;
    new_item.usages = 1;
    return item_index;
}

int create_optional(const fixed_point& position)
{
    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    int item_index = data.free_item_indexes.back();
    data.free_item_indexes.pop_back();

    item_type& new_item = data.items[item_index];
    new_item.position = position;
    new_item.usages = 1;
    return item_index;
}

void increase_usages(int id)
{
    item_type& item = data.items[id];
    ++item.usages;
}

void decrease_usages(int id)
{
    item_type& item = data.items[id];
    --item.usages;

    if(! item.usages)
    {
        data.free_item_indexes.push_back(id);
    }
}

const fixed_point& position(int id)
{
    const item_type& item = data.items[id];
    return item.position;
}

void set_x(int id, fixed x)
{
    item_type& item = data.items[id];

    if(item.position.x() != x)
    {
        item.position.set_x(x);
        data.update = true;
    }
}

void set_y(int id, fixed y)
{
    item_type& item = data.items[id];

    if(item.position.y() != y)
    {
        item.position.set_y(y);
        data.update = true;
    }
}

void set_position(int id, const fixed_point& position)
{
    item_type& item = data.items[id];

    if(item.position != position)
    {
        item.position = position;
        data.update = true;
    }
}

void update()
{
    if(data.update)
    {
        data.update = false;

        display_manager::update_cameras();
        sprites_manager::update_cameras();
        bgs_manager::update_cameras();
    }
}

}

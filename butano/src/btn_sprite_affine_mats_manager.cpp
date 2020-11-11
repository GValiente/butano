/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_sprite_affine_mats_manager.h"

#include "btn_vector.h"
#include "btn_sprites_manager_item.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"
#include "../hw/include/btn_hw_sprite_affine_mats_constants.h"

#include "btn_sprite_affine_mats.cpp.h"
#include "btn_sprite_affine_mat_ptr.cpp.h"

namespace btn::sprite_affine_mats_manager
{

namespace
{
    constexpr const int max_items = hw::sprite_affine_mats::count();

    static_assert(max_items <= numeric_limits<int8_t>::max());

    class item_type
    {

    public:
        sprite_affine_mat_attributes attributes;
        intrusive_list<sprite_affine_mat_attach_node_type> attached_nodes;
        unsigned usages;
        bool flipped_identity;
        bool double_size;
        bool remove_if_not_needed;

        void init()
        {
            attributes = sprite_affine_mat_attributes();
            usages = 1;
            flipped_identity = true;
            double_size = false;
            remove_if_not_needed = false;
        }

        void init(const sprite_affine_mat_attributes& new_attributes)
        {
            attributes = new_attributes;
            usages = 1;
            remove_if_not_needed = false;

            if(attributes.flipped_identity())
            {
                flipped_identity = true;
                double_size = false;
            }
            else
            {
                flipped_identity = false;
                double_size = attributes.double_size();
            }
        }
    };


    class registers
    {

    public:
        explicit registers(const sprite_affine_mat_attributes& attributes) :
            _pa(attributes.pa_register_value()),
            _pb(attributes.pb_register_value()),
            _pc(attributes.pc_register_value()),
            _pd(attributes.pd_register_value())
        {
        }

        [[nodiscard]] friend bool operator==(const registers& a, const registers& b) = default;

    private:
        int _pa;
        int _pb;
        int _pc;
        int _pd;
    };


    class static_data
    {

    public:
        item_type items[max_items];
        vector<int8_t, max_items> free_item_indexes;
        hw::sprite_affine_mats::handle* handles_ptr = nullptr;
        int first_index_to_commit = max_items;
        int last_index_to_commit = 0;
        int first_index_to_remove_if_not_needed = max_items;
        int last_index_to_remove_if_not_needed = 0;
    };

    BTN_DATA_EWRAM static_data data;


    void _update_indexes_to_commit(int index)
    {
        data.first_index_to_commit = min(data.first_index_to_commit, index);
        data.last_index_to_commit = max(data.last_index_to_commit, index);
    }

    void _update(int index)
    {
        item_type& item = data.items[index];
        const sprite_affine_mat_attributes& attributes = item.attributes;
        bool new_double_size;

        if(attributes.flipped_identity())
        {
            new_double_size = false;

            if(! item.flipped_identity)
            {
                item.flipped_identity = true;

                if(! item.attached_nodes.empty())
                {
                    item.remove_if_not_needed = true;

                    data.first_index_to_remove_if_not_needed = min(data.first_index_to_remove_if_not_needed, index);
                    data.last_index_to_remove_if_not_needed = max(data.last_index_to_remove_if_not_needed, index);
                }
            }
        }
        else
        {
            new_double_size = attributes.double_size();
            item.flipped_identity = false;
            item.remove_if_not_needed = false;
        }

        _update_indexes_to_commit(index);

        if(item.double_size != new_double_size)
        {
            item.double_size = new_double_size;

            for(sprite_affine_mat_attach_node_type& attached_node : item.attached_nodes)
            {
                sprites_manager_item& sprite_item = sprites_manager_item::affine_mat_attach_node_item(attached_node);
                sprites_manager::update_affine_mat(&sprite_item, index, new_double_size);
            }
        }
    }

    [[nodiscard]] int _create()
    {
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.init();
        hw::sprite_affine_mats::setup(data.handles_ptr[item_index]);
        _update_indexes_to_commit(item_index);
        return item_index;
    }

    [[nodiscard]] int _create(const sprite_affine_mat_attributes& attributes)
    {
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.init(attributes);
        hw::sprite_affine_mats::setup(attributes, data.handles_ptr[item_index]);
        _update_indexes_to_commit(item_index);
        return item_index;
    }
}

void init([[maybe_unused]] int handles_size, void* handles)
{
    BTN_ASSERT(handles_size == sizeof(hw::sprite_affine_mats::handle) * max_items,
               "Invalid handles size: ", handles_size, sizeof(hw::sprite_affine_mats::handle) * max_items);

    data.handles_ptr = static_cast<hw::sprite_affine_mats::handle*>(handles);

    for(int index = max_items - 1; index >= 0; --index)
    {
        data.free_item_indexes.push_back(int8_t(index));
    }
}

int used_count()
{
    return data.free_item_indexes.available();
}

int available_count()
{
    return data.free_item_indexes.size();
}

int create()
{
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more sprite affine mats available");

    return _create();
}

int create(const sprite_affine_mat_attributes& attributes)
{
    BTN_ASSERT(! data.free_item_indexes.empty(), "No more sprite affine mats available");

    return _create(attributes);
}

int create_optional()
{
    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create();
}

int create_optional(const sprite_affine_mat_attributes& attributes)
{
    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create(attributes);
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
        BTN_ASSERT(item.attached_nodes.empty(), "There's still attached nodes");

        item.remove_if_not_needed = false;
        data.free_item_indexes.push_back(int8_t(id));
    }
}

void attach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node)
{
    item_type& item = data.items[id];
    item.attached_nodes.push_back(attach_node);
}

void dettach_sprite(int id, sprite_affine_mat_attach_node_type& attach_node)
{
    item_type& item = data.items[id];
    item.attached_nodes.erase(attach_node);
}

fixed rotation_angle(int id)
{
    return data.items[id].attributes.rotation_angle();
}

void set_rotation_angle(int id, fixed rotation_angle)
{
    item_type& item = data.items[id];

    if(rotation_angle != item.attributes.rotation_angle())
    {
        registers old_registers(item.attributes);
        item.attributes.set_rotation_angle(rotation_angle);

        if(registers(item.attributes) != old_registers)
        {
            hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
            _update(id);
        }
    }
}

fixed horizontal_scale(int id)
{
    return data.items[id].attributes.horizontal_scale();
}

void set_horizontal_scale(int id, fixed horizontal_scale)
{
    item_type& item = data.items[id];

    if(horizontal_scale != item.attributes.horizontal_scale())
    {
        int pa = item.attributes.pa_register_value();
        int pb = item.attributes.pb_register_value();
        item.attributes.set_horizontal_scale(horizontal_scale);

        if(item.attributes.pa_register_value() != pa || item.attributes.pb_register_value() != pb)
        {
            hw::sprite_affine_mats::update_horizontal_scale(item.attributes, data.handles_ptr[id]);
            _update(id);
        }
    }
}

fixed vertical_scale(int id)
{
    return data.items[id].attributes.vertical_scale();
}

void set_vertical_scale(int id, fixed vertical_scale)
{
    item_type& item = data.items[id];

    if(vertical_scale != item.attributes.vertical_scale())
    {
        int pc = item.attributes.pc_register_value();
        int pd = item.attributes.pd_register_value();
        item.attributes.set_vertical_scale(vertical_scale);

        if(item.attributes.pc_register_value() != pc || item.attributes.pd_register_value() != pd)
        {
            hw::sprite_affine_mats::update_vertical_scale(item.attributes, data.handles_ptr[id]);
            _update(id);
        }
    }
}

void set_scale(int id, fixed scale)
{
    item_type& item = data.items[id];

    if(scale != item.attributes.horizontal_scale() || scale != item.attributes.vertical_scale())
    {
        registers old_registers(item.attributes);
        item.attributes.set_scale(scale);

        if(registers(item.attributes) != old_registers)
        {
            hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
            _update(id);
        }
    }
}

void set_scale(int id, fixed horizontal_scale, fixed vertical_scale)
{
    item_type& item = data.items[id];

    if(horizontal_scale != item.attributes.horizontal_scale() || vertical_scale != item.attributes.vertical_scale())
    {
        registers old_registers(item.attributes);
        item.attributes.set_scale(horizontal_scale, vertical_scale);

        if(registers(item.attributes) != old_registers)
        {
            hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
            _update(id);
        }
    }
}

bool horizontal_flip(int id)
{
    return data.items[id].attributes.horizontal_flip();
}

void set_horizontal_flip(int id, bool horizontal_flip)
{
    item_type& item = data.items[id];

    if(horizontal_flip != item.attributes.horizontal_flip())
    {
        item.attributes.set_horizontal_flip(horizontal_flip);
        hw::sprite_affine_mats::update_horizontal_scale(item.attributes, data.handles_ptr[id]);
        _update_indexes_to_commit(id);
    }
}

bool vertical_flip(int id)
{
    return data.items[id].attributes.vertical_flip();
}

void set_vertical_flip(int id, bool vertical_flip)
{
    item_type& item = data.items[id];

    if(vertical_flip != item.attributes.vertical_flip())
    {
        item.attributes.set_vertical_flip(vertical_flip);
        hw::sprite_affine_mats::update_vertical_scale(item.attributes, data.handles_ptr[id]);
        _update_indexes_to_commit(id);
    }
}

const sprite_affine_mat_attributes& attributes(int id)
{
    return data.items[id].attributes;
}

void set_attributes(int id, const sprite_affine_mat_attributes& attributes)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes = attributes;

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

bool identity(int id)
{
    const item_type& item = data.items[id];
    return item.attributes.identity();
}

bool flipped_identity(int id)
{
    const item_type& item = data.items[id];
    return item.flipped_identity;
}

bool double_size(int id)
{
    const item_type& item = data.items[id];
    return item.double_size;
}

void reload(int id)
{
    _update_indexes_to_commit(id);
}

void update()
{
    if(data.first_index_to_remove_if_not_needed < max_items)
    {
        for(int index = data.first_index_to_remove_if_not_needed, last = data.last_index_to_remove_if_not_needed;
            index <= last; ++index)
        {
            item_type& item = data.items[index];

            if(item.remove_if_not_needed)
            {
                auto it = item.attached_nodes.begin();
                auto end = item.attached_nodes.end();
                item.remove_if_not_needed = false;
                increase_usages(index);

                while(it != end)
                {
                    sprite_affine_mat_attach_node_type& attached_node = *it;
                    ++it;

                    sprites_manager_item& sprite_item = sprites_manager_item::affine_mat_attach_node_item(attached_node);
                    sprites_manager::remove_identity_affine_mat_if_not_needed(&sprite_item);
                }

                decrease_usages(index);
            }
        }

        data.first_index_to_remove_if_not_needed = max_items;
        data.last_index_to_remove_if_not_needed = 0;
    }
}

optional<commit_data> retrieve_commit_data()
{
    optional<commit_data> result;

    if(data.first_index_to_commit < max_items)
    {
        int offset = data.first_index_to_commit;
        int count = data.last_index_to_commit - data.first_index_to_commit + 1;
        result = commit_data{ offset, count };
        data.first_index_to_commit = max_items;
        data.last_index_to_commit = 0;
    }

    return result;
}

}

/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_affine_mats_manager.h"

#include "bn_vector.h"
#include "bn_sprites_manager_item.h"
#include "../hw/include/bn_hw_sprite_affine_mats.h"
#include "../hw/include/bn_hw_sprite_affine_mats_constants.h"

#include "bn_sprite_affine_mats.cpp.h"
#include "bn_sprite_affine_mat_ptr.cpp.h"

namespace bn::sprite_affine_mats_manager
{

namespace
{
    constexpr int max_items = hw::sprite_affine_mats::count();
    constexpr int affine_mat_id_multiplier = hw::sprites::count() / hw::sprite_affine_mats::count();

    static_assert(max_items <= numeric_limits<int8_t>::max());

    class item_type
    {

    public:
        affine_mat_attributes attributes;
        intrusive_list<sprite_affine_mat_attach_node_type> attached_nodes;
        unsigned usages;
        bool flipped_identity;
        bool remove_if_not_needed;

        void init()
        {
            attributes = affine_mat_attributes();
            usages = 1;
            flipped_identity = true;
            remove_if_not_needed = false;
        }

        void init(const affine_mat_attributes& new_attributes)
        {
            attributes = new_attributes;
            usages = 1;
            flipped_identity = attributes.flipped_identity();
            remove_if_not_needed = false;
        }
    };


    class registers
    {

    public:
        explicit registers(const affine_mat_attributes& attributes) :
            _pa(int16_t(attributes.pa_register_value())),
            _pb(int16_t(attributes.pb_register_value())),
            _pc(int16_t(attributes.pc_register_value())),
            _pd(int16_t(attributes.pd_register_value()))
        {
        }

        [[nodiscard]] friend bool operator==(const registers& a, const registers& b) = default;

    private:
        int16_t _pa;
        int16_t _pb;
        int16_t _pc;
        int16_t _pd;
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

    BN_DATA_EWRAM static_data data;


    void _update_flipped_identity(int index)
    {
        item_type& item = data.items[index];
        const affine_mat_attributes& item_attributes = item.attributes;

        if(item_attributes.flipped_identity())
        {
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
            item.flipped_identity = false;
            item.remove_if_not_needed = false;
        }
    }

    void _update_indexes_to_commit(int index)
    {
        data.first_index_to_commit = min(data.first_index_to_commit, index);
        data.last_index_to_commit = max(data.last_index_to_commit, index);
    }

    void _update(int index)
    {
        item_type& item = data.items[index];
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[index]);
        _update_indexes_to_commit(index);

        for(sprite_affine_mat_attach_node_type& attached_node : item.attached_nodes)
        {
            sprites_manager_item& sprite_item = sprites_manager_item::affine_mat_attach_node_item(attached_node);
            sprites_manager::update_affine_mat_double_size(&sprite_item, index);
        }
    }

    [[nodiscard]] int _new_item_index()
    {
        if(int free_items_count = data.free_item_indexes.size())
        {
            auto free_item_indexes_data = data.free_item_indexes.data();

            if(int reserved_sprite_handles_count = sprites_manager::reserved_handles_count()) [[unlikely]]
            {
                for(int free_items_index = free_items_count - 1; free_items_index >= 0; --free_items_index)
                {
                    int current_index = free_item_indexes_data[free_items_index];

                    if(current_index * affine_mat_id_multiplier >= reserved_sprite_handles_count)
                    {
                        int result = current_index;
                        data.free_item_indexes.erase(data.free_item_indexes.begin() + free_items_index);
                        return result;
                    }
                }
            }
            else
            {
                int result = free_item_indexes_data[free_items_count - 1];
                data.free_item_indexes.pop_back();
                return result;
            }
        }

        return -1;
    }

    template<int half_width, int half_height>
    [[nodiscard]] bool _sprite_double_size(int pa, int pb, int pc, int pd, int divisor)
    {
        if(pb || pd)
        {
            int ix1 = ((-256 * half_height * pb) - (256 * half_width * pd) + (256 * pb)) / divisor;

            if(ix1 < -half_width || ix1 >= half_width)
            {
                return true;
            }

            int ix2 = ((-256 * half_height * pb) + (256 * half_width * pd) + (256 * pb) - (256 * pd)) / divisor;

            if(ix2 < -half_width || ix2 >= half_width)
            {
                return true;
            }
        }

        int iy1 = (256 * ((half_height * pa) + (half_width * pc) - pa)) / divisor;

        if(iy1 < -half_height || iy1 >= half_height)
        {
            return true;
        }

        int iy2 = (256 * ((half_height * pa) - (half_width * pc) - pa + pc)) / divisor;

        return iy2 < -half_height || iy2 >= half_height;
    }
}

void init(void* handles)
{
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
    int id = create_optional();
    BN_ASSERT(id >= 0, "No more sprite affine mats available");

    return id;
}

int create(const affine_mat_attributes& attributes)
{
    int id = create_optional(attributes);
    BN_ASSERT(id >= 0, "No more sprite affine mats available");

    return id;
}

int create_optional()
{
    int item_index = _new_item_index();

    if(item_index >= 0)
    {
        item_type& new_item = data.items[item_index];
        new_item.init();
        hw::sprite_affine_mats::setup(data.handles_ptr[item_index]);
        _update_indexes_to_commit(item_index);
    }

    return item_index;
}

int create_optional(const affine_mat_attributes& attributes)
{
    int item_index = _new_item_index();

    if(item_index >= 0)
    {
        item_type& new_item = data.items[item_index];
        new_item.init(attributes);
        hw::sprite_affine_mats::setup(attributes, data.handles_ptr[item_index]);
        _update_indexes_to_commit(item_index);
    }

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
        _update_flipped_identity(id);

        if(registers(item.attributes) != old_registers)
        {
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
        _update_flipped_identity(id);

        if(item.attributes.pa_register_value() != pa || item.attributes.pb_register_value() != pb)
        {
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
        _update_flipped_identity(id);

        if(item.attributes.pc_register_value() != pc || item.attributes.pd_register_value() != pd)
        {
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
        _update_flipped_identity(id);

        if(registers(item.attributes) != old_registers)
        {
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
        _update_flipped_identity(id);

        if(registers(item.attributes) != old_registers)
        {
            _update(id);
        }
    }
}

fixed horizontal_shear(int id)
{
    return data.items[id].attributes.horizontal_shear();
}

void set_horizontal_shear(int id, fixed horizontal_shear)
{
    item_type& item = data.items[id];

    if(horizontal_shear != item.attributes.horizontal_shear())
    {
        int pb = item.attributes.pb_register_value();
        item.attributes.set_horizontal_shear(horizontal_shear);
        _update_flipped_identity(id);

        if(item.attributes.pb_register_value() != pb)
        {
            _update(id);
        }
    }
}

fixed vertical_shear(int id)
{
    return data.items[id].attributes.vertical_shear();
}

void set_vertical_shear(int id, fixed vertical_shear)
{
    item_type& item = data.items[id];

    if(vertical_shear != item.attributes.vertical_shear())
    {
        int pc = item.attributes.pc_register_value();
        item.attributes.set_vertical_shear(vertical_shear);
        _update_flipped_identity(id);

        if(item.attributes.pc_register_value() != pc)
        {
            _update(id);
        }
    }
}

void set_shear(int id, fixed shear)
{
    item_type& item = data.items[id];

    if(shear != item.attributes.horizontal_shear() || shear != item.attributes.vertical_shear())
    {
        int pb = item.attributes.pb_register_value();
        int pc = item.attributes.pc_register_value();
        item.attributes.set_shear(shear);
        _update_flipped_identity(id);

        if(item.attributes.pb_register_value() != pb || item.attributes.pc_register_value() != pc)
        {
            _update(id);
        }
    }
}

void set_shear(int id, fixed horizontal_shear, fixed vertical_shear)
{
    item_type& item = data.items[id];

    if(horizontal_shear != item.attributes.horizontal_shear() || vertical_shear != item.attributes.vertical_shear())
    {
        int pb = item.attributes.pb_register_value();
        int pc = item.attributes.pc_register_value();
        item.attributes.set_shear(horizontal_shear, vertical_shear);
        _update_flipped_identity(id);

        if(item.attributes.pb_register_value() != pb || item.attributes.pc_register_value() != pc)
        {
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
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
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
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
        _update_indexes_to_commit(id);
    }
}

const affine_mat_attributes& attributes(int id)
{
    return data.items[id].attributes;
}

void set_attributes(int id, const affine_mat_attributes& attributes)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes = attributes;
    _update_flipped_identity(id);

    if(registers(attributes) != old_registers)
    {
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

bool sprite_double_size(int id, const sprite_shape_size& shape_size)
{
    const item_type& item = data.items[id];

    if(item.flipped_identity)
    {
        return false;
    }

    const affine_mat_attributes& item_attributes = item.attributes;
    int pa = item_attributes.pa_register_value();
    int pb = item_attributes.pb_register_value();
    int pc = item_attributes.pc_register_value();
    int pd = item_attributes.pd_register_value();
    int divisor = (pa * pd) - (pb * pc);

    if(! divisor) [[unlikely]]
    {
        return true;
    }

    switch(shape_size.shape())
    {

    case sprite_shape::SQUARE:
        return _sprite_double_size<32, 32>(pa, pb, pc, pd, divisor);

    case sprite_shape::WIDE:
        switch(shape_size.size())
        {

        case sprite_size::SMALL:
            return _sprite_double_size<32, 16>(pa, pb, pc, pd, divisor);

        case sprite_size::NORMAL:
            return _sprite_double_size<32, 8>(pa, pb, pc, pd, divisor);

        case sprite_size::BIG:
        case sprite_size::HUGE:
            return _sprite_double_size<32, 16>(pa, pb, pc, pd, divisor);

        default:
            return false;
        }

    case sprite_shape::TALL:
        switch(shape_size.size())
        {

        case sprite_size::SMALL:
            return _sprite_double_size<16, 32>(pa, pb, pc, pd, divisor);

        case sprite_size::NORMAL:
            return _sprite_double_size<8, 32>(pa, pb, pc, pd, divisor);

        case sprite_size::BIG:
        case sprite_size::HUGE:
            return _sprite_double_size<16, 32>(pa, pb, pc, pd, divisor);

        default:
            return false;
        }

    default:
        return false;
    }
}

void reserve_sprite_handles([[maybe_unused]] int sprite_handles_count)
{
    if(used_count())
    {
        int first_active_index = max_items;

        for(int index = 0; index < max_items; ++index)
        {
            if(data.items[index].usages)
            {
                first_active_index = index;
                break;
            }
        }

        BN_ASSERT(sprite_handles_count <= first_active_index * affine_mat_id_multiplier,
                  "Reserved sprite handles used by affine mats: ", sprite_handles_count, " - ", first_active_index);

        if(data.first_index_to_commit < first_active_index)
        {
            data.first_index_to_commit = first_active_index;
            data.last_index_to_commit = min(data.last_index_to_commit, first_active_index);
        }
    }
    else
    {
        data.first_index_to_commit = max_items;
        data.last_index_to_commit = 0;
    }
}

void reload(int id)
{
    _update_indexes_to_commit(id);
}

void update()
{
    int first_index_to_remove = data.first_index_to_remove_if_not_needed;

    if(first_index_to_remove < max_items)
    {
        for(int index = first_index_to_remove, last = data.last_index_to_remove_if_not_needed; index <= last; ++index)
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

commit_data retrieve_commit_data()
{
    commit_data result;
    int first_index_to_commit = data.first_index_to_commit;

    if(first_index_to_commit < max_items)
    {
        int offset = first_index_to_commit;
        int count = data.last_index_to_commit - first_index_to_commit + 1;
        result = { offset, count };
        data.first_index_to_commit = max_items;
        data.last_index_to_commit = 0;
    }
    else
    {
        result = { 0, 0 };
    }

    return result;
}

}

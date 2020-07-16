#include "btn_sprite_affine_mats_manager.h"

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_config_sprites.h"
#include "btn_sprites_manager.h"
#include "btn_sprite_affine_mats.h"
#include "btn_sprites_manager_item.h"
#include "btn_sprite_affine_mat_attributes.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"

namespace btn::sprite_affine_mats_manager
{

namespace
{
    static_assert(sprite_affine_mats::count() <= numeric_limits<int8_t>::max());

    class item_type
    {

    public:
        sprite_affine_mat_attributes attributes;
        intrusive_list<sprite_affine_mat_attach_node_type> attached_nodes;
        unsigned usages;
        bool identity;
        bool identity_changed;
        bool double_size;

        void init()
        {
            attributes = sprite_affine_mat_attributes();
            usages = 1;
            identity = true;
            identity_changed = false;
            double_size = false;
        }

        void init(const sprite_affine_mat_attributes& new_attributes)
        {
            attributes = new_attributes;
            usages = 1;
            identity_changed = false;

            if(attributes.identity())
            {
                identity = true;
                double_size = false;
            }
            else
            {
                identity = false;
                double_size = attributes.double_size();
            }
        }
    };


    class registers
    {

    public:
        explicit registers(const sprite_affine_mat_attributes& attributes) :
            _pa(attributes.first_register_value()),
            _pb(attributes.second_register_value()),
            _pc(attributes.third_register_value()),
            _pd(attributes.fourth_register_value())
        {
        }

        [[nodiscard]] friend bool operator==(const registers& a, const registers& b)
        {
            return a._pa == b._pa && a._pb == b._pb && a._pc == b._pc && a._pd == b._pd;
        }

        [[nodiscard]] friend bool operator!=(const registers& a, const registers& b)
        {
            return ! (a == b);
        }

    private:
        int _pa;
        int _pb;
        int _pc;
        int _pd;
    };


    class static_data
    {

    public:
        item_type items[sprite_affine_mats::count()];
        vector<int8_t, sprite_affine_mats::count()> free_item_indexes;
        hw::sprite_affine_mats::handle* handles_ptr = nullptr;
        int first_index_to_commit = sprite_affine_mats::count();
        int last_index_to_commit = 0;
        bool identity_changed = false;
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

        if(attributes.identity())
        {
            new_double_size = false;

            if(! item.identity)
            {
                item.identity = true;
                item.identity_changed = true;
                data.identity_changed = true;
            }
        }
        else
        {
            new_double_size = attributes.double_size();
            item.identity = false;
        }

        bool double_size_changed = item.double_size != new_double_size;
        item.double_size = new_double_size;
        _update_indexes_to_commit(index);

        if(double_size_changed)
        {
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
    BTN_ASSERT(handles_size == sizeof(hw::sprite_affine_mats::handle) * sprite_affine_mats::count(),
               "Invalid handles size: ", handles_size,
               sizeof(hw::sprite_affine_mats::handle) * sprite_affine_mats::count());

    data.handles_ptr = static_cast<hw::sprite_affine_mats::handle*>(handles);

    for(int index = sprite_affine_mats::count() - 1; index >= 0; --index)
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

        item.identity_changed = false;
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
    registers old_registers(item.attributes);
    item.attributes.set_rotation_angle(rotation_angle);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

fixed scale_x(int id)
{
    return data.items[id].attributes.scale_x();
}

void set_scale_x(int id, fixed scale_x)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_scale_x(scale_x);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::update_scale_x(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

fixed scale_y(int id)
{
    return data.items[id].attributes.scale_y();
}

void set_scale_y(int id, fixed scale_y)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_scale_y(scale_y);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::update_scale_y(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

void set_scale(int id, fixed scale)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_scale(scale);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

void set_scale(int id, fixed scale_x, fixed scale_y)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_scale(scale_x, scale_y);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

bool horizontal_flip(int id)
{
    return data.items[id].attributes.horizontal_flip();
}

void set_horizontal_flip(int id, bool horizontal_flip)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_horizontal_flip(horizontal_flip);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::update_scale_x(item.attributes, data.handles_ptr[id]);
        _update(id);
    }
}

bool vertical_flip(int id)
{
    return data.items[id].attributes.vertical_flip();
}

void set_vertical_flip(int id, bool vertical_flip)
{
    item_type& item = data.items[id];
    registers old_registers(item.attributes);
    item.attributes.set_vertical_flip(vertical_flip);

    if(registers(item.attributes) != old_registers)
    {
        hw::sprite_affine_mats::update_scale_y(item.attributes, data.handles_ptr[id]);
        _update(id);
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
    return item.identity;
}

bool double_size(int id)
{
    const item_type& item = data.items[id];
    return item.double_size;
}

void update()
{
    if(data.identity_changed)
    {
        data.identity_changed = false;

        for(int index = data.first_index_to_commit, last = data.last_index_to_commit; index <= last; ++index)
        {
            item_type& item = data.items[index];

            if(item.identity_changed)
            {
                auto it = item.attached_nodes.begin();
                auto end = item.attached_nodes.end();
                item.identity_changed = false;
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
    }
}

optional<commit_data> retrieve_commit_data()
{
    optional<commit_data> result;

    if(data.first_index_to_commit < sprite_affine_mats::count())
    {
        int offset = data.first_index_to_commit;
        int count = data.last_index_to_commit - data.first_index_to_commit + 1;
        result = commit_data{ offset, count };
        data.first_index_to_commit = sprite_affine_mats::count();
        data.last_index_to_commit = 0;
    }

    return result;
}

}

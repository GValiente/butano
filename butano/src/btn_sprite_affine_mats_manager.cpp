#include "btn_sprite_affine_mats_manager.h"

#include "btn_limits.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_algorithm.h"
#include "btn_sprite_affine_mats.h"
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
        unsigned usages;
        bool updated;

        void init(const sprite_affine_mat_attributes& new_attributes)
        {
            attributes = new_attributes;
            usages = 1;
            updated = false;
        }
    };

    class static_data
    {

    public:
        item_type items[sprite_affine_mats::count()];
        vector<int8_t, sprite_affine_mats::count()> free_item_indexes;
        hw::sprite_affine_mats::handle* handles_ptr = nullptr;
        int first_index_to_commit = sprite_affine_mats::count();
        int last_index_to_commit = 0;
    };

    BTN_DATA_EWRAM static_data data;

    void _update(int index)
    {
        data.first_index_to_commit = min(data.first_index_to_commit, index);
        data.last_index_to_commit = max(data.last_index_to_commit, index);
        data.items[index].updated = true;
    }

    [[nodiscard]] int _create()
    {
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.init(sprite_affine_mat_attributes());
        hw::sprite_affine_mats::setup(data.handles_ptr[item_index]);
        _update(item_index);
        return item_index;
    }

    [[nodiscard]] int _create(const sprite_affine_mat_attributes& attributes)
    {
        int item_index = data.free_item_indexes.back();
        data.free_item_indexes.pop_back();

        item_type& new_item = data.items[item_index];
        new_item.init(attributes);
        hw::sprite_affine_mats::setup(attributes, data.handles_ptr[item_index]);
        _update(item_index);
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

int optional_create()
{
    if(data.free_item_indexes.empty())
    {
        return -1;
    }

    return _create();
}

int optional_create(const sprite_affine_mat_attributes& attributes)
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
        data.free_item_indexes.push_back(int8_t(id));
    }
}

fixed rotation_angle(int id)
{
    return data.items[id].attributes.rotation_angle();
}

void set_rotation_angle(int id, fixed rotation_angle)
{
    item_type& item = data.items[id];
    item.attributes.set_rotation_angle(rotation_angle);
    hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
    _update(id);
}

fixed scale_x(int id)
{
    return data.items[id].attributes.scale_x();
}

void set_scale_x(int id, fixed scale_x)
{
    item_type& item = data.items[id];
    item.attributes.set_scale_x(scale_x);
    hw::sprite_affine_mats::update_scale_x(item.attributes, data.handles_ptr[id]);
    _update(id);
}

fixed scale_y(int id)
{
    return data.items[id].attributes.scale_y();
}

void set_scale_y(int id, fixed scale_y)
{
    item_type& item = data.items[id];
    item.attributes.set_scale_y(scale_y);
    hw::sprite_affine_mats::update_scale_y(item.attributes, data.handles_ptr[id]);
    _update(id);
}

void set_scale(int id, fixed scale)
{
    item_type& item = data.items[id];
    item.attributes.set_scale(scale);
    hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
    _update(id);
}

void set_scale(int id, fixed scale_x, fixed scale_y)
{
    item_type& item = data.items[id];
    item.attributes.set_scale(scale_x, scale_y);
    hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
    _update(id);
}

bool horizontal_flip(int id)
{
    return data.items[id].attributes.horizontal_flip();
}

void set_horizontal_flip(int id, bool horizontal_flip)
{
    item_type& item = data.items[id];
    item.attributes.set_horizontal_flip(horizontal_flip);
    hw::sprite_affine_mats::update_scale_x(item.attributes, data.handles_ptr[id]);
    _update(id);
}

bool vertical_flip(int id)
{
    return data.items[id].attributes.vertical_flip();
}

void set_vertical_flip(int id, bool vertical_flip)
{
    item_type& item = data.items[id];
    item.attributes.set_vertical_flip(vertical_flip);
    hw::sprite_affine_mats::update_scale_y(item.attributes, data.handles_ptr[id]);
    _update(id);
}

const sprite_affine_mat_attributes& attributes(int id)
{
    return data.items[id].attributes;
}

void set_attributes(int id, const sprite_affine_mat_attributes& attributes)
{
    item_type& item = data.items[id];
    item.attributes = attributes;
    hw::sprite_affine_mats::setup(item.attributes, data.handles_ptr[id]);
    _update(id);
}

bool identity(int id)
{
    const item_type& item = data.items[id];
    return item.attributes.identity();
}

bool double_size(int id)
{
    const item_type& item = data.items[id];
    return item.attributes.double_size();
}

bool updated(int id)
{
    const item_type& item = data.items[id];
    return item.updated;
}

bool updated()
{
    return data.first_index_to_commit < sprite_affine_mats::count();
}

void update()
{
    for(int index = data.first_index_to_commit, last = data.last_index_to_commit; index <= last; ++index)
    {
        data.items[index].updated = false;
    }
}

optional<commit_data> retrieve_commit_data()
{
    optional<commit_data> result;

    if(updated())
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

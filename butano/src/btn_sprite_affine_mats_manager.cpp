#include "btn_sprite_affine_mats_manager.h"

#include "btn_limits.h"
#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_algorithm.h"
#include "btn_sprite_affine_mat_builder.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"

namespace btn::sprite_affine_mats_manager
{

namespace
{
    static_assert(hw::sprite_affine_mats::count() <= numeric_limits<int8_t>::max(), "");

    class item_type
    {

    public:
        fixed rotation_angle;
        fixed scale_x;
        fixed scale_y;
        unsigned usages;
        hw::sprite_affine_mats::item_type hw_item;
        unsigned horizontal_flip: 1;
        unsigned vertical_flip: 1;
        unsigned updated: 1;

        void init(const sprite_affine_mat_builder& builder)
        {
            rotation_angle = builder.rotation_angle();
            scale_x = builder.scale_x();
            scale_y = builder.scale_y();
            usages = 1;
            horizontal_flip = builder.horizontal_flip();
            vertical_flip = builder.vertical_flip();
            updated = false;
            hw_item.init(builder);
        }
    };

    class static_data
    {

    public:
        item_type items[hw::sprite_affine_mats::count()];
        vector<int8_t, hw::sprite_affine_mats::count()> free_item_indexes;
        hw::sprite_affine_mats::handle* handles_ptr = nullptr;
        int first_index_to_commit = hw::sprite_affine_mats::count();
        int last_index_to_commit = 0;
    };

    BTN_DATA_EWRAM static_data data;

    void _update(int index)
    {
        data.first_index_to_commit = min(data.first_index_to_commit, index);
        data.last_index_to_commit = max(data.last_index_to_commit, index);
        data.items[index].updated = true;
    }
}

void init([[maybe_unused]] int handles_size, void* handles)
{
    BTN_ASSERT(handles_size == int(sizeof(hw::sprite_affine_mats::handle)) * hw::sprite_affine_mats::count(),
               "Invalid handles size: ", handles_size,
               sizeof(hw::sprite_affine_mats::handle) * hw::sprite_affine_mats::count());

    data.handles_ptr = static_cast<hw::sprite_affine_mats::handle*>(handles);

    for(int index = hw::sprite_affine_mats::count() - 1; index >= 0; --index)
    {
        data.free_item_indexes.push_back(int8_t(index));
    }
}

int count()
{
    return hw::sprite_affine_mats::count();
}

int used_count()
{
    return int(data.free_item_indexes.available());
}

int available_count()
{
    return int(data.free_item_indexes.size());
}

optional<int> create(const sprite_affine_mat_builder& builder)
{
    if(data.free_item_indexes.empty())
    {
        return nullopt;
    }

    int item_index = data.free_item_indexes.back();
    data.free_item_indexes.pop_back();

    item_type& new_item = data.items[item_index];
    new_item.init(builder);
    hw::sprite_affine_mats::setup(new_item.hw_item, data.handles_ptr[item_index]);
    _update(item_index);
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
        data.free_item_indexes.push_back(int8_t(id));
    }
}

fixed rotation_angle(int id)
{
    return data.items[id].rotation_angle;
}

void set_rotation_angle(int id, fixed rotation_angle)
{
    item_type& item = data.items[id];
    item.rotation_angle = rotation_angle;
    item.hw_item.set_rotation_angle(rotation_angle);
    hw::sprite_affine_mats::setup(item.hw_item, data.handles_ptr[id]);
    _update(id);
}

fixed scale_x(int id)
{
    return data.items[id].scale_x;
}

void set_scale_x(int id, fixed scale_x)
{
    item_type& item = data.items[id];
    item.scale_x = scale_x;
    item.hw_item.set_scale_x(scale_x);
    hw::sprite_affine_mats::setup(item.hw_item, data.handles_ptr[id]);
    _update(id);
}

fixed scale_y(int id)
{
    return data.items[id].scale_y;
}

void set_scale_y(int id, fixed scale_y)
{
    item_type& item = data.items[id];
    item.scale_y = scale_y;
    item.hw_item.set_scale_y(scale_y);
    hw::sprite_affine_mats::setup(item.hw_item, data.handles_ptr[id]);
    _update(id);
}

bool horizontal_flip(int id)
{
    return data.items[id].horizontal_flip;
}

void set_horizontal_flip(int id, bool horizontal_flip)
{
    item_type& item = data.items[id];
    item.horizontal_flip = horizontal_flip;
    item.hw_item.set_horizontal_flip(horizontal_flip);
    hw::sprite_affine_mats::setup(item.hw_item, data.handles_ptr[id]);
    _update(id);
}

bool vertical_flip(int id)
{
    return data.items[id].vertical_flip;
}

void set_vertical_flip(int id, bool vertical_flip)
{
    item_type& item = data.items[id];
    item.vertical_flip = vertical_flip;
    item.hw_item.set_vertical_flip(vertical_flip);
    hw::sprite_affine_mats::setup(item.hw_item, data.handles_ptr[id]);
    _update(id);
}

bool is_identity(int id)
{
    item_type& item = data.items[id];
    return item.rotation_angle == 0 && item.scale_x == 1 && item.scale_y == 1;
}

bool double_size(int id)
{
    item_type& item = data.items[id];
    return hw::sprite_affine_mats::double_size(item.hw_item, max(item.scale_x, item.scale_y));
}

bool updated(int id)
{
    item_type& item = data.items[id];
    return item.updated;
}

bool updated()
{
    return data.first_index_to_commit < hw::sprite_affine_mats::count();
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
        data.first_index_to_commit = hw::sprite_affine_mats::count();
        data.last_index_to_commit = 0;
    }

    return result;
}

}

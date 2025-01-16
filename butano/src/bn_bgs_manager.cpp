/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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
#include "bn_affine_bg_mat_attributes.h"
#include "bn_affine_mat_attributes_reader.h"
#include "../hw/include/bn_hw_bgs.h"
#include "../hw/include/bn_hw_display.h"

#include "bn_bgs.cpp.h"
#include "bn_affine_bg_ptr.cpp.h"
#include "bn_affine_bg_item.cpp.h"
#include "bn_affine_bg_builder.cpp.h"
#include "bn_affine_bg_attributes.cpp.h"
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
        unsigned usages = 1;
        fixed_point position;
        affine_bg_mat_attributes affine_mat_attributes;
        point hw_position;
        size half_dimensions;
        hw::bgs::affine_attributes hw_affine_attributes;
        sort_key bg_sort_key;
        bool visible_in_windows[hw::display::windows_count()];
        optional<regular_bg_map_ptr> regular_map;
        optional<affine_bg_map_ptr> affine_map;
        optional<camera_ptr> camera;
        uint16_t hw_cnt;
        int16_t old_big_map_x = 0;
        int16_t old_big_map_y = 0;
        int16_t new_big_map_x = 0;
        int16_t new_big_map_y = 0;
        int8_t handles_index = -1;
        bool blending_top_enabled: 1;
        bool blending_bottom_enabled: 1;
        bool visible: 1;
        bool big_map: 1;
        bool commit_big_map: 1;
        bool full_commit_big_map: 1;

        item_type(regular_bg_builder&& builder, regular_bg_map_ptr&& _regular_map) :
            position(builder.position()),
            bg_sort_key(builder.priority(), builder.z_order()),
            regular_map(move(_regular_map)),
            camera(builder.release_camera()),
            blending_top_enabled(builder.blending_top_enabled()),
            blending_bottom_enabled(builder.blending_bottom_enabled()),
            visible(builder.visible())
        {
            for(bool& visible_in_window : visible_in_windows)
            {
                visible_in_window = true;
            }

            hw::bgs::setup_regular(builder, hw_cnt);
            update_regular_map();
        }

        item_type(affine_bg_builder&& builder, affine_bg_map_ptr&& _affine_map) :
            position(builder.position()),
            affine_mat_attributes((builder.camera() ? position - builder.camera()->position() : position),
                                  _affine_map.dimensions() * 4, builder.pivot_position(), builder.mat_attributes()),
            bg_sort_key(builder.priority(), builder.z_order()),
            affine_map(move(_affine_map)),
            camera(builder.release_camera()),
            blending_top_enabled(builder.blending_top_enabled()),
            blending_bottom_enabled(builder.blending_bottom_enabled()),
            visible(builder.visible())
        {
            for(bool& visible_in_window : visible_in_windows)
            {
                visible_in_window = true;
            }

            hw::bgs::setup_affine(builder, hw_cnt);

            [[maybe_unused]] bool affine_mat_attributes_updated = update_affine_map(true);
        }

        void update_regular_map()
        {
            const regular_bg_map_ptr& map_ref = *regular_map;
            uint16_t new_hw_cnt = hw_cnt;
            hw::bgs::set_tiles_cbb(map_ref.tiles().cbb(), new_hw_cnt);
            hw::bgs::set_map_sbb(map_ref.id(), new_hw_cnt);
            hw::bgs::set_bpp(map_ref.bpp(), new_hw_cnt);

            size map_dimensions = map_ref.dimensions();
            int map_size = 0;
            big_map = map_ref.big();

            if(! big_map)
            {
                if(map_dimensions.width() == 64)
                {
                    ++map_size;
                }

                if(map_dimensions.height() == 64)
                {
                    map_size += 2;
                }
            }

            commit_big_map = big_map;
            full_commit_big_map = big_map;

            hw::bgs::set_map_dimensions(map_size, new_hw_cnt);
            hw_cnt = new_hw_cnt;
            half_dimensions = map_dimensions * 4;
            update_regular_hw_position();
        }

        [[nodiscard]] bool update_affine_map(bool force_affine_mat_attributes_update)
        {
            const affine_bg_map_ptr& map_ref = *affine_map;
            uint16_t new_hw_cnt = hw_cnt;
            hw::bgs::set_tiles_cbb(map_ref.tiles().cbb(), new_hw_cnt);
            hw::bgs::set_map_sbb(map_ref.id(), new_hw_cnt);

            size map_dimensions = map_ref.dimensions();
            int map_size;
            big_map = map_ref.big();

            if(big_map)
            {
                map_size = int(map_ref.big_canvas_size()) + 1;
            }
            else
            {
                switch(map_dimensions.width())
                {

                case 16:
                    map_size = 0;
                    break;

                case 32:
                    map_size = 1;
                    break;

                case 64:
                    map_size = 2;
                    break;

                case 128:
                default:
                    map_size = 3;
                    break;
                }
            }

            commit_big_map = big_map;
            full_commit_big_map = big_map;

            bool affine_mat_attributes_updated;
            hw::bgs::set_map_dimensions(map_size, new_hw_cnt);
            hw_cnt = new_hw_cnt;
            half_dimensions = map_dimensions * 4;

            if(force_affine_mat_attributes_update)
            {
                update_affine_mat_attributes();
                affine_mat_attributes_updated = true;
            }
            else
            {
                fixed_size fixed_half_dimensions = half_dimensions;

                if(fixed_half_dimensions != affine_mat_attributes.half_dimensions())
                {
                    affine_mat_attributes.set_half_dimensions(fixed_half_dimensions);
                    update_affine_mat_attributes();
                    affine_mat_attributes_updated = true;
                }
                else
                {
                    affine_mat_attributes_updated = false;
                }
            }

            return affine_mat_attributes_updated;
        }

        [[nodiscard]] hw::bgs::regular_offset regular_hw_offset() const
        {
            return { uint16_t(hw_position.x()), uint16_t(hw_position.y()) };
        }

        void update_regular_hw_position()
        {
            int real_x = position.x().right_shift_integer();
            int real_y = position.y().right_shift_integer();

            if(camera_ptr* camera_ptr = camera.get())
            {
                const fixed_point& camera_position = camera_ptr->position();
                real_x -= camera_position.x().right_shift_integer();
                real_y -= camera_position.y().right_shift_integer();
            }

            int hw_x = -real_x - (display::width() / 2) + half_dimensions.width();
            int hw_y = -real_y - (display::height() / 2) + half_dimensions.height();
            hw_position.set_x(hw_x);
            hw_position.set_y(hw_y);
            check_commit_big_map();
        }

        void update_affine_mat_attributes()
        {
            hw::bgs::set_affine_mat_attributes(affine_mat_attributes.mat_attributes(), hw_affine_attributes);
            update_affine_hw_x();
            update_affine_hw_y();
        }

        void update_affine_camera()
        {
            if(camera_ptr* camera_ptr = camera.get())
            {
                affine_mat_attributes.set_position(position - camera_ptr->position());
            }
            else
            {
                affine_mat_attributes.set_position(position);
            }

            update_affine_hw_x();
            update_affine_hw_y();
        }

        void check_commit_big_map()
        {
            if(big_map)
            {
                commit_big_map = true;
            }
        }

        [[nodiscard]] point affine_map_position() const
        {
            int map_x2 = (half_dimensions.width() - (bn::display::width() / 2) -
                          affine_mat_attributes.x().right_shift_integer() +
                          affine_mat_attributes.pivot_x().right_shift_integer()) >> 3;

            int map_y2 = (half_dimensions.height() - (bn::display::height() / 2) -
                          affine_mat_attributes.y().right_shift_integer() +
                          affine_mat_attributes.pivot_y().right_shift_integer()) >> 3;

            return point(map_x2, map_y2);
        }

        void update_affine_hw_x()
        {
            int dx = affine_mat_attributes.dx_register_value();
            hw_position.set_x(dx);
            hw_affine_attributes.dx = dx;

            if(big_map)
            {
                commit_big_map = true;
            }
        }

        void update_affine_hw_y()
        {
            int dy = affine_mat_attributes.dy_register_value();
            hw_position.set_y(dy);
            hw_affine_attributes.dy = dy;

            if(big_map)
            {
                commit_big_map = true;
            }
        }
    };


    class static_data
    {

    public:
        pool<item_type, BN_CFG_BGS_MAX_ITEMS> items_pool;
        vector<item_type*, BN_CFG_BGS_MAX_ITEMS> items_vector;
        hw::bgs::commit_data commit_data;
        bool rebuild_handles = false;
        bool commit = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    [[nodiscard]] bool _check_unique_regular_big_map(item_type& item)
    {
        if(item.big_map)
        {
            for(item_type* other_item : data.items_vector)
            {
                if(other_item != &item && other_item->regular_map == item.regular_map)
                {
                    return false;
                }
            }
        }

        return true;
    }

    [[nodiscard]] bool _check_unique_affine_big_map(item_type& item)
    {
        if(item.big_map)
        {
            for(item_type* other_item : data.items_vector)
            {
                if(other_item != &item && other_item->affine_map == item.affine_map)
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
        bool affine_new_item = new_item.affine_map.has_value();

        if(new_item.visible)
        {
            data.rebuild_handles = true;
        }

        for(auto it = data.items_vector.begin(), end = data.items_vector.end(); it != end; ++it)
        {
            const item_type* item = *it;

            if((affine_new_item && ! item->affine_map) || bg_sort_key > item->bg_sort_key)
            {
                data.items_vector.insert(it, &new_item);
                return;
            }
        }

        data.items_vector.push_back(&new_item);
    }

    void _update_item_hw_cnt(const item_type& item)
    {
        if(! data.rebuild_handles && item.visible)
        {
            data.commit_data.cnts[item.handles_index] = item.hw_cnt;
            data.commit = true;
        }
    }

    void _update_item_hw_regular_offset(const item_type& item)
    {
        if(! data.rebuild_handles && item.visible)
        {
            data.commit_data.regular_offsets[item.handles_index] = item.regular_hw_offset();
            data.commit = true;
        }
    }

    void _update_item_hw_affine_attributes(const item_type& item)
    {
        if(! data.rebuild_handles && item.visible)
        {
            data.commit_data.affine_attribute_sets[item.handles_index - 2] = item.hw_affine_attributes;
            data.commit = true;
        }
    }

    void _update_big_maps()
    {
        for(item_type* item : data.items_vector)
        {
            if(item->big_map)
            {
                regular_bg_map_ptr* item_regular_map = item->regular_map.get();
                int old_map_x = item->old_big_map_x;
                int old_map_y = item->old_big_map_y;
                int new_map_x;
                int new_map_y;

                if(item_regular_map)
                {
                    new_map_x = item->hw_position.x() >> 3;
                    new_map_y = item->hw_position.y() >> 3;
                }
                else
                {
                    point affine_map_position = item->affine_map_position();
                    new_map_x = affine_map_position.x();
                    new_map_y = affine_map_position.y();

                    if(new_map_x % 2)
                    {
                        --new_map_x;
                    }
                }

                int map_handle = item_regular_map ? item_regular_map->handle() : item->affine_map->handle();
                bool full_commit_big_map = item->full_commit_big_map || bg_blocks_manager::must_commit(map_handle);
                bool commit_big_map = full_commit_big_map;

                if(! commit_big_map && item->commit_big_map && item->visible)
                {
                    commit_big_map = old_map_x != new_map_x || old_map_y != new_map_y;
                }

                if(commit_big_map)
                {
                    item->new_big_map_x = int16_t(new_map_x);
                    item->new_big_map_y = int16_t(new_map_y);
                    item->commit_big_map = true;
                    item->full_commit_big_map = full_commit_big_map || bn::abs(new_map_x - old_map_x) > 8 ||
                            bn::abs(new_map_y - old_map_y) > 8;
                }
            }
        }
    }
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();
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
    BN_BASIC_ASSERT(! data.items_vector.full(), "No more BG items available");

    regular_bg_map_ptr map = builder.release_map();
    item_type& item = data.items_pool.create(move(builder), move(map));
    BN_BASIC_ASSERT(_check_unique_regular_big_map(item), "Two or more regular BGs have the same big map");

    _insert_item(item);
    return &item;
}

id_type create(affine_bg_builder&& builder)
{
    BN_BASIC_ASSERT(! data.items_vector.full(), "No more BG items available");

    affine_bg_map_ptr map = builder.release_map();
    item_type& item = data.items_pool.create(move(builder), move(map));
    BN_BASIC_ASSERT(_check_unique_affine_big_map(item), "Two or more affine BGs have the same big map");

    _insert_item(item);
    return &item;
}

id_type create_optional(regular_bg_builder&& builder)
{
    if(data.items_vector.full())
    {
        return nullptr;
    }

    optional<regular_bg_map_ptr> map = builder.release_map_optional();
    regular_bg_map_ptr* map_ptr = map.get();

    if(! map_ptr)
    {
        return nullptr;
    }

    item_type& item = data.items_pool.create(move(builder), move(*map_ptr));
    BN_BASIC_ASSERT(_check_unique_regular_big_map(item), "Two or more regular BGs have the same big map");

    _insert_item(item);
    return &item;
}

id_type create_optional(affine_bg_builder&& builder)
{
    if(data.items_vector.full())
    {
        return nullptr;
    }

    optional<affine_bg_map_ptr> map = builder.release_map_optional();
    affine_bg_map_ptr* map_ptr = map.get();

    if(! map_ptr)
    {
        return nullptr;
    }

    item_type& item = data.items_pool.create(move(builder), move(*map_ptr));
    BN_BASIC_ASSERT(_check_unique_affine_big_map(item), "Two or more affine BGs have the same big map");

    _insert_item(item);
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

    if(! item->usages) [[likely]]
    {
        if(! data.rebuild_handles && item->visible)
        {
            data.rebuild_handles = true;
        }

        erase(data.items_vector, item);
        data.items_pool.destroy(*item);
    }
}

int hw_id(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->handles_index;
}

size dimensions(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->half_dimensions * 2;
}

bool big(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->big_map;
}

const regular_bg_map_ptr& regular_map(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->regular_map;
}

const affine_bg_map_ptr& affine_map(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->affine_map;
}

void set_regular_map(id_type id, const regular_bg_map_ptr& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->regular_map)
    {
        item->regular_map = map;
        item->update_regular_map();
        BN_BASIC_ASSERT(_check_unique_regular_big_map(*item), "Two or more regular BGs have the same big map");

        _update_item_hw_cnt(*item);
        _update_item_hw_regular_offset(*item);
    }
}

void set_affine_map(id_type id, const affine_bg_map_ptr& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->affine_map)
    {
        item->affine_map = map;

        bool affine_mat_attributes_updated = item->update_affine_map(false);
        BN_BASIC_ASSERT(_check_unique_affine_big_map(*item), "Two or more affine BGs have the same big map");

        _update_item_hw_cnt(*item);

        if(affine_mat_attributes_updated)
        {
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void set_regular_map(id_type id, regular_bg_map_ptr&& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->regular_map)
    {
        item->regular_map = move(map);
        item->update_regular_map();
        BN_BASIC_ASSERT(_check_unique_regular_big_map(*item), "Two or more regular BGs have the same big map");

        _update_item_hw_cnt(*item);
        _update_item_hw_regular_offset(*item);
    }
}

void set_affine_map(id_type id, affine_bg_map_ptr&& map)
{
    auto item = static_cast<item_type*>(id);

    if(map != item->affine_map)
    {
        item->affine_map = move(map);

        bool affine_mat_attributes_updated = item->update_affine_map(false);
        BN_BASIC_ASSERT(_check_unique_affine_big_map(*item), "Two or more affine BGs have the same big map");

        _update_item_hw_cnt(*item);

        if(affine_mat_attributes_updated)
        {
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void remove_regular_map(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->regular_map.reset();
}

void remove_affine_map(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->affine_map.reset();
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

void set_regular_x(id_type id, fixed x)
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
        item->check_commit_big_map();
        _update_item_hw_regular_offset(*item);
    }
}

void set_affine_x(id_type id, fixed x)
{
    auto item = static_cast<item_type*>(id);
    fixed old_x = item->position.x();
    item->position.set_x(x);

    if(old_x.right_shift_integer() != x.right_shift_integer())
    {
        if(camera_ptr* item_camera = item->camera.get())
        {
            x -= item_camera->x();
        }

        item->affine_mat_attributes.set_x(x);
        item->update_affine_hw_x();
        _update_item_hw_affine_attributes(*item);
    }
}

void set_regular_y(id_type id, fixed y)
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
        item->check_commit_big_map();
        _update_item_hw_regular_offset(*item);
    }
}

void set_affine_y(id_type id, fixed y)
{
    auto item = static_cast<item_type*>(id);
    fixed old_y = item->position.y();
    item->position.set_y(y);

    if(old_y.right_shift_integer() != y.right_shift_integer())
    {
        if(camera_ptr* item_camera = item->camera.get())
        {
            y -= item_camera->y();
        }

        item->affine_mat_attributes.set_y(y);
        item->update_affine_hw_y();
        _update_item_hw_affine_attributes(*item);
    }
}

void set_regular_position(id_type id, const fixed_point& position)
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
        item->check_commit_big_map();
        _update_item_hw_regular_offset(*item);
    }
}

void set_affine_position(id_type id, const fixed_point& position)
{
    auto item = static_cast<item_type*>(id);
    fixed_point old_position = item->position;
    item->position = position;

    point old_integer_position(old_position.x().right_shift_integer(), old_position.y().right_shift_integer());
    point new_integer_position(position.x().right_shift_integer(), position.y().right_shift_integer());

    if(old_integer_position != new_integer_position)
    {
        if(camera_ptr* item_camera = item->camera.get())
        {
            item->affine_mat_attributes.set_position(position - item_camera->position());
        }
        else
        {
            item->affine_mat_attributes.set_position(position);
        }

        item->update_affine_hw_x();
        item->update_affine_hw_y();
        _update_item_hw_affine_attributes(*item);
    }
}

fixed rotation_angle(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.rotation_angle();
}

void set_rotation_angle(id_type id, fixed rotation_angle)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(rotation_angle != affine_mat_attributes.rotation_angle())
    {
        affine_mat_attributes_reader reader(affine_mat_attributes.mat_attributes());
        int sin = reader.sin();
        int cos = reader.cos();
        affine_mat_attributes.set_rotation_angle(rotation_angle);

        if(reader.sin() != sin || reader.cos() != cos)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

fixed horizontal_scale(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.horizontal_scale();
}

void set_horizontal_scale(id_type id, fixed horizontal_scale)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(horizontal_scale != affine_mat_attributes.horizontal_scale())
    {
        affine_mat_attributes_reader reader(affine_mat_attributes.mat_attributes());
        unsigned sx = reader.sx();
        affine_mat_attributes.set_horizontal_scale(horizontal_scale);

        if(reader.sx() != sx)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

fixed vertical_scale(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.vertical_scale();
}

void set_vertical_scale(id_type id, fixed vertical_scale)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(vertical_scale != affine_mat_attributes.vertical_scale())
    {
        affine_mat_attributes_reader reader(affine_mat_attributes.mat_attributes());
        unsigned sy = reader.sy();
        affine_mat_attributes.set_vertical_scale(vertical_scale);

        if(reader.sy() != sy)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void set_scale(id_type id, fixed scale)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(scale != affine_mat_attributes.horizontal_scale() || scale != affine_mat_attributes.vertical_scale())
    {
        affine_mat_attributes_reader reader(affine_mat_attributes.mat_attributes());
        unsigned sx = reader.sx();
        unsigned sy = reader.sy();
        affine_mat_attributes.set_scale(scale);

        if(reader.sx() != sx || reader.sy() != sy)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void set_scale(id_type id, fixed horizontal_scale, fixed vertical_scale)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(horizontal_scale != affine_mat_attributes.horizontal_scale() ||
            vertical_scale != affine_mat_attributes.vertical_scale())
    {
        affine_mat_attributes_reader reader(affine_mat_attributes.mat_attributes());
        unsigned sx = reader.sx();
        unsigned sy = reader.sy();
        affine_mat_attributes.set_scale(horizontal_scale, vertical_scale);

        if(reader.sx() != sx || reader.sy() != sy)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

fixed horizontal_shear(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.horizontal_shear();
}

void set_horizontal_shear(id_type id, fixed horizontal_shear)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(horizontal_shear != affine_mat_attributes.horizontal_shear())
    {
        int pb = affine_mat_attributes.pb_register_value();
        affine_mat_attributes.set_horizontal_shear(horizontal_shear);

        if(affine_mat_attributes.pb_register_value() != pb)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

fixed vertical_shear(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.vertical_shear();
}

void set_vertical_shear(id_type id, fixed vertical_shear)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(vertical_shear != affine_mat_attributes.vertical_shear())
    {
        int pc = affine_mat_attributes.pc_register_value();
        affine_mat_attributes.set_vertical_shear(vertical_shear);

        if(affine_mat_attributes.pc_register_value() != pc)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void set_shear(id_type id, fixed shear)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(shear != affine_mat_attributes.horizontal_shear() || shear != affine_mat_attributes.vertical_shear())
    {
        int pb = affine_mat_attributes.pb_register_value();
        int pc = affine_mat_attributes.pc_register_value();
        affine_mat_attributes.set_shear(shear);

        if(affine_mat_attributes.pb_register_value() != pb || affine_mat_attributes.pc_register_value() != pc)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void set_shear(id_type id, fixed horizontal_shear, fixed vertical_shear)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(horizontal_shear != affine_mat_attributes.horizontal_shear() ||
            vertical_shear != affine_mat_attributes.vertical_shear())
    {
        int pb = affine_mat_attributes.pb_register_value();
        int pc = affine_mat_attributes.pc_register_value();
        affine_mat_attributes.set_shear(horizontal_shear, vertical_shear);

        if(affine_mat_attributes.pb_register_value() != pb || affine_mat_attributes.pc_register_value() != pc)
        {
            item->update_affine_mat_attributes();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

bool horizontal_flip(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.horizontal_flip();
}

void set_horizontal_flip(id_type id, bool horizontal_flip)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(horizontal_flip != affine_mat_attributes.horizontal_flip())
    {
        affine_mat_attributes.set_horizontal_flip(horizontal_flip);
        item->update_affine_mat_attributes();
        _update_item_hw_affine_attributes(*item);
    }
}

bool vertical_flip(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.vertical_flip();
}

void set_vertical_flip(id_type id, bool vertical_flip)
{
    auto item = static_cast<item_type*>(id);
    affine_bg_mat_attributes& affine_mat_attributes = item->affine_mat_attributes;

    if(vertical_flip != affine_mat_attributes.vertical_flip())
    {
        affine_mat_attributes.set_vertical_flip(vertical_flip);
        item->update_affine_mat_attributes();
        _update_item_hw_affine_attributes(*item);
    }
}

const fixed_point& pivot_position(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes.pivot_position();
}

void set_pivot_x(id_type id, fixed pivot_x)
{
    auto item = static_cast<item_type*>(id);

    if(pivot_x != item->affine_mat_attributes.pivot_x())
    {
        item->affine_mat_attributes.set_pivot_x(pivot_x);
        item->update_affine_hw_x();
        _update_item_hw_affine_attributes(*item);
    }
}

void set_pivot_y(id_type id, fixed pivot_y)
{
    auto item = static_cast<item_type*>(id);

    if(pivot_y != item->affine_mat_attributes.pivot_y())
    {
        item->affine_mat_attributes.set_pivot_y(pivot_y);
        item->update_affine_hw_y();
        _update_item_hw_affine_attributes(*item);
    }
}

void set_pivot_position(id_type id, const fixed_point& pivot_position)
{
    auto item = static_cast<item_type*>(id);

    if(pivot_position != item->affine_mat_attributes.pivot_position())
    {
        item->affine_mat_attributes.set_pivot_position(pivot_position);
        item->update_affine_hw_x();
        item->update_affine_hw_y();
        _update_item_hw_affine_attributes(*item);
    }
}

const affine_bg_mat_attributes& mat_attributes(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_attributes;
}

void set_mat_attributes(id_type id, const affine_mat_attributes& mat_attributes)
{
    auto item = static_cast<item_type*>(id);

    if(mat_attributes != item->affine_mat_attributes.mat_attributes())
    {
        item->affine_mat_attributes.set_mat_attributes(mat_attributes);
        item->update_affine_mat_attributes();
        _update_item_hw_affine_attributes(*item);
    }
}

int priority(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->bg_sort_key.priority();
}

void set_priority(id_type id, int priority)
{
    auto item = static_cast<item_type*>(id);

    if(item->bg_sort_key.priority() != priority)
    {
        BN_ASSERT(priority >= 0 && priority <= bgs::max_priority(), "Invalid priority: ", priority);

        item->bg_sort_key.set_priority(priority);
        hw::bgs::set_priority(priority, item->hw_cnt);
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
        BN_ASSERT(z_order >= bgs::min_z_order() && z_order <= bgs::max_z_order(), "Invalid z order: ", z_order);

        item->bg_sort_key.set_z_order(z_order);
        erase(data.items_vector, item);
        _insert_item(*item);
    }
}

void put_above(id_type id)
{
    const auto items_vector_data = data.items_vector.data();
    auto item = static_cast<item_type*>(id);
    sort_key this_sort_key = item->bg_sort_key;
    bool order_modified = false;

    for(int index = 0, limit = data.items_vector.size() - 1; index < limit; ++index)
    {
        item_type*& current_item_ptr = items_vector_data[index];

        if(current_item_ptr == item)
        {
            item_type*& next_item_ptr = items_vector_data[index + 1];

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

void put_below(id_type id)
{
    const auto items_vector_data = data.items_vector.data();
    auto item = static_cast<item_type*>(id);
    sort_key this_sort_key = item->bg_sort_key;
    bool order_modified = false;

    for(int index = data.items_vector.size() - 1; index > 0; --index)
    {
        item_type*& current_item_ptr = items_vector_data[index];

        if(current_item_ptr == item)
        {
            item_type*& previous_item_ptr = items_vector_data[index - 1];

            if(previous_item_ptr->bg_sort_key == this_sort_key)
            {
                swap(current_item_ptr, previous_item_ptr);
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

bool wrapping_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::bgs::wrapping_enabled(item->hw_cnt);
}

void set_wrapping_enabled(id_type id, bool wrapping_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::bgs::set_wrapping_enabled(wrapping_enabled, item->hw_cnt);
    _update_item_hw_cnt(*item);
}

bool mosaic_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::bgs::mosaic_enabled(item->hw_cnt);
}

void set_mosaic_enabled(id_type id, bool mosaic_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::bgs::set_mosaic_enabled(mosaic_enabled, item->hw_cnt);
    _update_item_hw_cnt(*item);
}

bn::green_swap_mode green_swap_mode(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::bgs::green_swap_mode(item->hw_cnt);
}

void set_green_swap_mode(id_type id, bn::green_swap_mode green_swap_mode)
{
    auto item = static_cast<item_type*>(id);
    hw::bgs::set_green_swap_mode(green_swap_mode, item->hw_cnt);
    _update_item_hw_cnt(*item);
}

regular_bg_attributes regular_attributes(id_type id)
{
    return regular_bg_attributes(regular_map(id), priority(id), mosaic_enabled(id), green_swap_mode(id));
}

affine_bg_attributes affine_attributes(id_type id)
{
    return affine_bg_attributes(affine_map(id), priority(id), wrapping_enabled(id), mosaic_enabled(id),
                                green_swap_mode(id));
}

void set_regular_attributes(id_type id, const regular_bg_attributes& attributes)
{
    set_regular_map(id, attributes.map());
    set_priority(id, attributes.priority());
    set_mosaic_enabled(id, attributes.mosaic_enabled());
    set_green_swap_mode(id, attributes.green_swap_mode());
}

void set_affine_attributes(id_type id, const affine_bg_attributes& attributes)
{
    set_affine_map(id, attributes.map());
    set_priority(id, attributes.priority());
    set_wrapping_enabled(id, attributes.wrapping_enabled());
    set_mosaic_enabled(id, attributes.mosaic_enabled());
    set_green_swap_mode(id, attributes.green_swap_mode());
}

bool blending_top_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->blending_top_enabled;
}

void set_blending_top_enabled(id_type id, bool blending_top_enabled)
{
    auto item = static_cast<item_type*>(id);
    item->blending_top_enabled = blending_top_enabled;

    if(! data.rebuild_handles && item->visible)
    {
        display_manager::set_blending_top_bg_enabled(item->handles_index, blending_top_enabled);
    }
}

bool blending_bottom_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->blending_bottom_enabled;
}

void set_blending_bottom_enabled(id_type id, bool blending_bottom_enabled)
{
    auto item = static_cast<item_type*>(id);
    item->blending_bottom_enabled = blending_bottom_enabled;

    if(! data.rebuild_handles && item->visible)
    {
        display_manager::set_blending_bottom_bg_enabled(item->handles_index, blending_bottom_enabled);
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

bool show_in_window(id_type id, int window)
{
    auto item = static_cast<item_type*>(id);
    return item->visible_in_windows[window];
}

void set_show_in_window(id_type id, int window, bool show)
{
    auto item = static_cast<item_type*>(id);

    if(item->visible_in_windows[window] != show)
    {
        item->visible_in_windows[window] = show;
        display_manager::update_windows_visible_bgs();
    }
}

void set_show_all_in_window(int window, bool show)
{
    bool update = false;

    for(item_type* item : data.items_vector)
    {
        if(item->visible_in_windows[window] != show)
        {
            item->visible_in_windows[window] = show;
            update = true;
        }
    }

    if(update)
    {
        display_manager::update_windows_visible_bgs();
    }
}

void update_windows_flags(unsigned* windows_flags)
{
    for(int window = 0; window < hw::display::windows_count(); ++window)
    {
        unsigned window_flags = windows_flags[window];

        for(int bg = 0; bg < hw::bgs::count(); ++bg)
        {
            window_flags &= ~(unsigned(hw::display::window_flag::BG_0) << bg);
        }

        windows_flags[window] = window_flags;
    }

    for(item_type* item : data.items_vector)
    {
        int handles_index = item->handles_index;

        if(handles_index >= 0)
        {
            for(int window = 0; window < hw::display::windows_count(); ++window)
            {
                if(item->visible_in_windows[window])
                {
                    windows_flags[window] |= unsigned(hw::display::window_flag::BG_0) << handles_index;
                }
            }
        }
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

        if(item->regular_map)
        {
            item->update_regular_hw_position();
            _update_item_hw_regular_offset(*item);
        }
        else
        {
            item->update_affine_camera();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void remove_camera(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->camera)
    {
        item->camera.reset();

        if(item->regular_map)
        {
            item->update_regular_hw_position();
            _update_item_hw_regular_offset(*item);
        }
        else
        {
            item->update_affine_camera();
            _update_item_hw_affine_attributes(*item);
        }
    }
}

void update_cameras()
{
    for(item_type* item : data.items_vector)
    {
        if(item->camera)
        {
            if(item->regular_map)
            {
                item->update_regular_hw_position();
                _update_item_hw_regular_offset(*item);
            }
            else
            {
                item->update_affine_camera();
                _update_item_hw_affine_attributes(*item);
            }
        }
    }
}

void update_regular_map_tiles_cbb(int map_id, int tiles_cbb)
{
    for(item_type* item : data.items_vector)
    {
        regular_bg_map_ptr* item_regular_map = item->regular_map.get();

        if(item_regular_map && item_regular_map->id() == map_id)
        {
            hw::bgs::set_tiles_cbb(tiles_cbb, item->hw_cnt);
            _update_item_hw_cnt(*item);
        }
    }
}

void update_affine_map_tiles_cbb(int map_id, int tiles_cbb)
{
    for(item_type* item : data.items_vector)
    {
        affine_bg_map_ptr* item_affine_map = item->affine_map.get();

        if(item_affine_map && item_affine_map->id() == map_id)
        {
            hw::bgs::set_tiles_cbb(tiles_cbb, item->hw_cnt);
            _update_item_hw_cnt(*item);
        }
    }
}

void update_regular_map_palette_bpp(int map_id, bpp_mode bpp)
{
    for(item_type* item : data.items_vector)
    {
        regular_bg_map_ptr* item_regular_map = item->regular_map.get();

        if(item_regular_map && item_regular_map->id() == map_id)
        {
            hw::bgs::set_bpp(bpp, item->hw_cnt);
            _update_item_hw_cnt(*item);
        }
    }
}

void reload()
{
    data.commit = true;
}

void fill_hblank_effect_regular_positions(int base_position, const fixed* positions_ptr, uint16_t* dest_ptr)
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

void fill_hblank_effect_pivot_horizontal_positions(id_type id, const fixed* positions_ptr, unsigned* dest_ptr)
{
    constexpr int right_shift = fixed::precision() - hw::bgs::affine_precision();

    auto item = static_cast<item_type*>(id);
    int base_dx = item->affine_mat_attributes.dx_register_value();
    int pb = item->affine_mat_attributes.pb_register_value();

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        int result = base_dx + (positions_ptr[index].data() >> right_shift);
        dest_ptr[index] = unsigned(result);
        base_dx += pb;
    }
}

void fill_hblank_effect_pivot_vertical_positions(id_type id, const fixed* positions_ptr, unsigned* dest_ptr)
{
    constexpr int right_shift = fixed::precision() - hw::bgs::affine_precision();

    auto item = static_cast<item_type*>(id);
    int base_dy = item->affine_mat_attributes.dy_register_value();
    int pd = item->affine_mat_attributes.pd_register_value();

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        int result = base_dy + (positions_ptr[index].data() >> right_shift);
        dest_ptr[index] = unsigned(result);
        base_dy += pd;
    }
}

void fill_hblank_effect_regular_attributes(id_type id, const regular_bg_attributes* attributes_ptr, uint16_t* dest_ptr)
{
    auto item = static_cast<item_type*>(id);
    [[maybe_unused]] bn::size current_dimensions = item->regular_map->dimensions();
    uint16_t hw_cnt = item->hw_cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const regular_bg_attributes& attributes = attributes_ptr[index];
        const regular_bg_map_ptr& attributes_map = attributes.map();
        BN_BASIC_ASSERT(current_dimensions == attributes_map.dimensions(), "Different map dimensions");

        uint16_t dest_hw_cnt = hw_cnt;
        hw::bgs::set_tiles_cbb(attributes_map.tiles().cbb(), dest_hw_cnt);
        hw::bgs::set_map_sbb(attributes_map.id(), dest_hw_cnt);
        hw::bgs::set_bpp(attributes_map.bpp(), dest_hw_cnt);
        hw::bgs::set_priority(attributes.priority(), dest_hw_cnt);
        hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), dest_hw_cnt);
        hw::bgs::set_green_swap_mode(attributes.green_swap_mode(), dest_hw_cnt);
        dest_ptr[index] = dest_hw_cnt;
    }
}

void fill_hblank_effect_affine_attributes(id_type id, const affine_bg_attributes* attributes_ptr, uint16_t* dest_ptr)
{
    auto item = static_cast<item_type*>(id);
    [[maybe_unused]] bn::size current_dimensions = item->affine_map->dimensions();
    uint16_t hw_cnt = item->hw_cnt;

    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const affine_bg_attributes& attributes = attributes_ptr[index];
        const affine_bg_map_ptr& attributes_map = attributes.map();
        BN_BASIC_ASSERT(current_dimensions == attributes_map.dimensions(), "Different map dimensions");

        uint16_t dest_hw_cnt = hw_cnt;
        hw::bgs::set_tiles_cbb(attributes_map.tiles().cbb(), dest_hw_cnt);
        hw::bgs::set_map_sbb(attributes_map.id(), dest_hw_cnt);
        hw::bgs::set_priority(attributes.priority(), dest_hw_cnt);
        hw::bgs::set_wrapping_enabled(attributes.wrapping_enabled(), dest_hw_cnt);
        hw::bgs::set_mosaic_enabled(attributes.mosaic_enabled(), dest_hw_cnt);
        hw::bgs::set_green_swap_mode(attributes.green_swap_mode(), dest_hw_cnt);
        dest_ptr[index] = dest_hw_cnt;
    }
}

void rebuild_handles()
{
    if(data.rebuild_handles)
    {
        int affine_bgs_count = 0;
        data.rebuild_handles = false;
        data.commit = true;

        for(item_type* item : data.items_vector)
        {
            if(item->affine_map && item->visible)
            {
                ++affine_bgs_count;
            }
        }

        BN_BASIC_ASSERT(affine_bgs_count <= hw::bgs::affine_count(), "Too many affine BGs on screen");

        int regular_id;
        int affine_id;
        display_manager::set_mode(affine_bgs_count);
        display_manager::disable_all_bgs();
        display_manager::update_windows_visible_bgs();

        switch(affine_bgs_count)
        {

        case 0:
            regular_id = hw::bgs::count() - 1;
            affine_id = -1;
            break;

        case 1:
            regular_id = hw::bgs::count() - 3;
            affine_id = hw::bgs::count() - 2;
            break;

        default:
            regular_id = -1;
            affine_id = hw::bgs::count() - 1;
            break;
        }

        for(item_type* item : data.items_vector)
        {
            if(item->visible)
            {
                int id;

                if(item->affine_map)
                {
                    id = affine_id;
                    --affine_id;
                    data.commit_data.affine_attribute_sets[id - 2] = item->hw_affine_attributes;
                }
                else
                {
                    BN_BASIC_ASSERT(regular_id >= 0, "Too many regular BGs on screen");

                    id = regular_id;
                    --regular_id;
                    data.commit_data.regular_offsets[id] = item->regular_hw_offset();
                }

                item->handles_index = int8_t(id);
                data.commit_data.cnts[id] = item->hw_cnt;
                display_manager::set_bg_enabled(id, true);
                display_manager::set_blending_top_bg_enabled(id, item->blending_top_enabled);
                display_manager::set_blending_bottom_bg_enabled(id, item->blending_bottom_enabled);
            }
            else
            {
                item->handles_index = -1;
            }
        }
    }
}

void update()
{
    rebuild_handles();
    _update_big_maps();
}

void commit(bool use_dma)
{
    if(data.commit)
    {
        hw::bgs::commit(data.commit_data, use_dma);
        data.commit = false;
    }
}

void commit_big_maps()
{
    for(item_type* item : data.items_vector)
    {
        if(item->commit_big_map)
        {
            regular_bg_map_ptr* item_regular_map = item->regular_map.get();
            int map_handle = item_regular_map ? item_regular_map->handle() : item->affine_map->handle();
            int old_map_x = item->old_big_map_x;
            int old_map_y = item->old_big_map_y;
            int new_map_x = item->new_big_map_x;
            int new_map_y = item->new_big_map_y;
            item->old_big_map_x = int16_t(new_map_x);
            item->old_big_map_y = int16_t(new_map_y);
            item->commit_big_map = false;

            if(item->full_commit_big_map)
            {
                item->full_commit_big_map = false;

                if(item_regular_map)
                {
                    bg_blocks_manager::set_regular_map_position(map_handle, new_map_x, new_map_y);
                }
                else
                {
                    bg_blocks_manager::set_affine_map_position(map_handle, new_map_x, new_map_y);
                }
            }
            else
            {
                if(item_regular_map)
                {
                    while(new_map_x < old_map_x)
                    {
                        --old_map_x;
                        bg_blocks_manager::update_regular_map_left_col(map_handle, old_map_x, new_map_y);
                    }

                    while(new_map_x > old_map_x)
                    {
                        ++old_map_x;
                        bg_blocks_manager::update_regular_map_right_col(map_handle, old_map_x, new_map_y);
                    }

                    while(new_map_y < old_map_y)
                    {
                        --old_map_y;
                        bg_blocks_manager::update_regular_map_top_row(map_handle, new_map_x, old_map_y);
                    }

                    while(new_map_y > old_map_y)
                    {
                        ++old_map_y;
                        bg_blocks_manager::update_regular_map_bottom_row(map_handle, new_map_x, old_map_y);
                    }
                }
                else
                {
                    while(new_map_x < old_map_x)
                    {
                        --old_map_x;
                        bg_blocks_manager::update_affine_map_left_col(map_handle, old_map_x, new_map_y);
                    }

                    while(new_map_x > old_map_x)
                    {
                        ++old_map_x;
                        bg_blocks_manager::update_affine_map_right_col(map_handle, old_map_x, new_map_y);
                    }

                    while(new_map_y < old_map_y)
                    {
                        --old_map_y;
                        bg_blocks_manager::update_affine_map_top_row(map_handle, new_map_x, old_map_y);
                    }

                    while(new_map_y > old_map_y)
                    {
                        ++old_map_y;
                        bg_blocks_manager::update_affine_map_bottom_row(map_handle, new_map_x, old_map_y);
                    }
                }
            }
        }
    }
}

void stop()
{
    data.rebuild_handles = false;
    data.commit = false;
    hw::bgs::stop();
}

}

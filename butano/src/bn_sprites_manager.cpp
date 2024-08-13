/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites_manager.h"

#include "bn_vector.h"
#include "bn_sprite_first_attributes.h"
#include "bn_sprite_regular_second_attributes.h"
#include "bn_sorted_sprites.h"
#include "../hw/include/bn_hw_sprite_affine_mats_constants.h"

#include "bn_sprites.cpp.h"
#include "bn_sprite_ptr.cpp.h"
#include "bn_sprite_item.cpp.h"
#include "bn_sprite_builder.cpp.h"
#include "bn_sprite_third_attributes.cpp.h"
#include "bn_sprite_affine_second_attributes.cpp.h"

namespace bn::sprites_manager
{

namespace
{
    static_assert(BN_CFG_SPRITES_MAX_ITEMS > 0);

    using item_type = sprites_manager_item;
    using sorted_items_type = vector<item_type*, BN_CFG_SPRITES_MAX_ITEMS>;

    class static_data
    {

    public:
        pool<item_type, BN_CFG_SPRITES_MAX_ITEMS> items_pool;
        hw::sprites::handle_type handles[hw::sprites::count()];
        sorted_sprites::sorter sorter;
        int reserved_handles_count = 0;
        int first_index_to_commit = 0;
        int last_index_to_commit = hw::sprites::count() - 1;
        int last_visible_items_count = 0;
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
        bool reload_all_handles = false;
    };

    BN_DATA_EWRAM_BSS static_data data;

    void _always_update_indexes_to_commit(const item_type& item)
    {
        int handles_index = item.handles_index;

        if(handles_index >= 0)
        {
            hw::sprites::copy_handle(item.handle, data.handles[handles_index]);

            if(handles_index < data.first_index_to_commit)
            {
                data.first_index_to_commit = handles_index;
            }

            if(handles_index > data.last_index_to_commit)
            {
                data.last_index_to_commit = handles_index;
            }
        }
    }

    inline void _update_indexes_to_commit(const item_type& item)
    {
        if(! data.rebuild_handles)
        {
            _always_update_indexes_to_commit(item);
        }
    }

    void _update_item_dimensions(item_type& item)
    {
        item.update_half_dimensions();

        if(item.visible)
        {
            item.check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
    }

    void _update_item_shape_size(const sprite_shape_size& shape_size, item_type& item)
    {
        hw::sprites::set_shape_size(shape_size, item.handle);

        if(sprite_affine_mat_ptr* affine_mat = item.affine_mat.get())
        {
            if(sprite_double_size_mode(item.double_size_mode) == sprite_double_size_mode::AUTO)
            {
                bool new_double_size = sprite_affine_mats_manager::sprite_double_size(affine_mat->id(), shape_size);

                if(item.double_size != new_double_size)
                {
                    item.double_size = new_double_size;

                    if(item.visible)
                    {
                        hw::sprites::show_affine(new_double_size, item.handle);
                    }
                }
            }
        }

        _update_item_dimensions(item);
    }

    void _assign_affine_mat(bool remove_when_not_needed, item_type& item, sprite_affine_mat_ptr&& affine_mat)
    {
        item.remove_affine_mat_when_not_needed = remove_when_not_needed;

        if(const sprite_affine_mat_ptr* item_affine_mat = item.affine_mat.get())
        {
            sprite_affine_mats_manager::dettach_sprite(item_affine_mat->id(), item.affine_mat_attach_node);
        }

        int affine_mat_id = affine_mat.id();
        item.affine_mat = move(affine_mat);
        sprite_affine_mats_manager::attach_sprite(affine_mat_id, item.affine_mat_attach_node);

        bool new_double_size = item.new_double_size(affine_mat_id);
        hw::sprites::set_affine_mat(affine_mat_id, item.handle);

        if(item.visible)
        {
            hw::sprites::show_affine(new_double_size, item.handle);
        }

        if(item.double_size != new_double_size)
        {
            item.double_size = new_double_size;
            _update_item_dimensions(item);
        }
        else
        {
            _update_indexes_to_commit(item);
        }
    }

    void _remove_affine_mat(item_type& item)
    {
        sprite_affine_mat_ptr& item_affine_mat = *item.affine_mat;
        hw::sprites::set_horizontal_flip(item_affine_mat.horizontal_flip(), item.handle);
        hw::sprites::set_vertical_flip(item_affine_mat.vertical_flip(), item.handle);

        if(item.visible)
        {
            hw::sprites::show_regular(item.handle);
        }

        sprite_affine_mats_manager::dettach_sprite(item_affine_mat.id(), item.affine_mat_attach_node);
        item.affine_mat.reset();

        if(item.double_size)
        {
            item.double_size = false;
            _update_item_dimensions(item);
        }
        else
        {
            _update_indexes_to_commit(item);
        }
    }

    void _rebuild_handles()
    {
        if(data.rebuild_handles)
        {
            hw::sprites::handle_type* handles = data.handles;
            int reserved_count = data.reserved_handles_count;
            bool reload_all_handles = data.reload_all_handles;

            if(reload_all_handles) [[unlikely]]
            {
                data.reload_all_handles = false;

                for(int index = 0; index < reserved_count; ++index)
                {
                    hw::sprites::hide_and_destroy(handles[index]);
                }
            }

            int visible_items_count = _rebuild_handles_impl(reserved_count, handles, data.sorter.layers());
            BN_BASIC_ASSERT(visible_items_count >= 0, "Too many on screen sprites");

            int last_visible_items_count = data.last_visible_items_count;
            data.rebuild_handles = false;
            data.last_visible_items_count = visible_items_count;

            for(int index = visible_items_count; index < last_visible_items_count; ++index)
            {
                hw::sprites::hide_and_destroy(handles[index]);
            }

            if(reload_all_handles) [[unlikely]]
            {
                data.first_index_to_commit = 0;
                data.last_index_to_commit = hw::sprites::count() - 1;
            }
            else
            {
                int to_commit_items_count;

                if(visible_items_count < last_visible_items_count)
                {
                    to_commit_items_count = min(last_visible_items_count, hw::sprites::count() - reserved_count);
                }
                else
                {
                    to_commit_items_count = visible_items_count;
                }

                if(to_commit_items_count)
                {
                    data.first_index_to_commit = reserved_count;
                    data.last_index_to_commit = reserved_count + to_commit_items_count - 1;
                }
                else
                {
                    data.first_index_to_commit = hw::sprites::count();
                    data.last_index_to_commit = 0;
                }
            }
        }
    }
}

void init()
{
    new(&data) static_data();

    for(hw::sprites::handle_type& handle : data.handles)
    {
        hw::sprites::hide_and_destroy(handle);
    }

    sprite_affine_mats_manager::init(data.handles);
}

int used_items_count()
{
    return data.items_pool.size();
}

int available_items_count()
{
    return data.items_pool.available();
}

id_type create(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles,
               sprite_palette_ptr&& palette)
{
    BN_BASIC_ASSERT(! data.items_pool.full(), "No more sprite items available");

    item_type& new_item = data.items_pool.create(position, shape_size, move(tiles), move(palette));
    data.sorter.insert(new_item);
    data.check_items_on_screen = true;
    data.rebuild_handles = true;
    return &new_item;
}

id_type create_optional(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles,
                        sprite_palette_ptr&& palette)
{
    if(data.items_pool.full())
    {
        return nullptr;
    }

    item_type& new_item = data.items_pool.create(position, shape_size, move(tiles), move(palette));
    data.sorter.insert(new_item);
    data.check_items_on_screen = true;
    data.rebuild_handles = true;
    return &new_item;
}

id_type create(sprite_builder&& builder)
{
    BN_BASIC_ASSERT(! data.items_pool.full(), "No more sprite items available");

    item_type& new_item = data.items_pool.create(move(builder));
    data.sorter.insert(new_item);

    if(new_item.visible)
    {
        data.check_items_on_screen = true;
        data.rebuild_handles = true;
    }

    return &new_item;
}

id_type create_optional(sprite_builder&& builder)
{
    if(data.items_pool.full())
    {
        return nullptr;
    }

    optional<sprite_tiles_ptr> builder_tiles = builder.release_tiles_optional();
    sprite_tiles_ptr* tiles_ptr = builder_tiles.get();

    if(! tiles_ptr)
    {
        return nullptr;
    }

    optional<sprite_palette_ptr> builder_palette = builder.release_palette_optional();
    sprite_palette_ptr* palette_ptr = builder_palette.get();

    if(! palette_ptr)
    {
        return nullptr;
    }

    item_type& new_item = data.items_pool.create(move(builder), move(*tiles_ptr), move(*palette_ptr));
    data.sorter.insert(new_item);

    if(new_item.visible)
    {
        data.check_items_on_screen = true;
        data.rebuild_handles = true;
    }

    return &new_item;
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
        data.sorter.erase(*item);

        if(const sprite_affine_mat_ptr* item_affine_mat = item->affine_mat.get())
        {
            sprite_affine_mats_manager::dettach_sprite(item_affine_mat->id(), item->affine_mat_attach_node);
        }

        if(item->visible)
        {
            hw::sprites::hide_and_destroy(item->handle);
            data.rebuild_handles = true;
        }

        data.items_pool.destroy(*item);
    }
}

int hw_id(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->handles_index;
}

sprite_shape shape(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::shape(item->handle);
}

sprite_size size(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::size(item->handle);
}

sprite_shape_size shape_size(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::shape_size(item->handle);
}

bn::size dimensions(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return bn::size(item->half_width * 2, item->half_height * 2);
}

const sprite_tiles_ptr& tiles(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->tiles;
}

void set_tiles(id_type id, const sprite_tiles_ptr& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        hw::sprites::handle_type& handle = item->handle;
        BN_ASSERT(tiles.tiles_count() == hw::sprites::shape_size(handle).tiles_count(item->palette->bpp()),
                  "Invalid tiles count: ", tiles.tiles_count(), " - ",
                  hw::sprites::shape_size(handle).tiles_count(item->palette->bpp()));

        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = tiles;
        _update_indexes_to_commit(*item);
    }
}

void set_tiles(id_type id, sprite_tiles_ptr&& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        hw::sprites::handle_type& handle = item->handle;
        BN_ASSERT(tiles.tiles_count() == hw::sprites::shape_size(handle).tiles_count(item->palette->bpp()),
                  "Invalid tiles count: ", tiles.tiles_count(), " - ",
                  hw::sprites::shape_size(handle).tiles_count(item->palette->bpp()));

        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = move(tiles);
        _update_indexes_to_commit(*item);
    }
}

void set_tiles(id_type id, const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(item->palette->bpp()),
                  "Invalid tiles or shape size: ", tiles.tiles_count(), " - ",
                  shape_size.tiles_count(item->palette->bpp()));

        hw::sprites::handle_type& handle = item->handle;
        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = tiles;

        if(shape_size != hw::sprites::shape_size(handle))
        {
            _update_item_shape_size(shape_size, *item);
        }
        else
        {
            _update_indexes_to_commit(*item);
        }
    }
}

void set_tiles(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(item->palette->bpp()),
                  "Invalid tiles or shape size: ", tiles.tiles_count(), " - ",
                  shape_size.tiles_count(item->palette->bpp()));

        hw::sprites::handle_type& handle = item->handle;
        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = move(tiles);

        if(shape_size != hw::sprites::shape_size(handle))
        {
            _update_item_shape_size(shape_size, *item);
        }
        else
        {
            _update_indexes_to_commit(*item);
        }
    }
}

void remove_tiles(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->tiles.reset();
}

const sprite_palette_ptr& palette(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->palette;
}

void set_palette(id_type id, [[maybe_unused]] bpp_mode old_bpp, const sprite_palette_ptr& palette)
{
    auto item = static_cast<item_type*>(id);

    if(palette != item->palette)
    {
        BN_BASIC_ASSERT(old_bpp == palette.bpp(),
                        "Different palette BPP mode: ", int(old_bpp), " - ", int(palette.bpp()));

        hw::sprites::set_palette(palette.id(), item->handle);
        item->palette = palette;
        _update_indexes_to_commit(*item);
    }
}

void set_palette(id_type id, [[maybe_unused]] bpp_mode old_bpp, sprite_palette_ptr&& palette)
{
    auto item = static_cast<item_type*>(id);

    if(palette != item->palette)
    {
        BN_BASIC_ASSERT(old_bpp == palette.bpp(),
                        "Different palette BPP mode: ", int(old_bpp), " - ", int(palette.bpp()));

        hw::sprites::set_palette(palette.id(), item->handle);
        item->palette = move(palette);
        _update_indexes_to_commit(*item);
    }
}

void remove_palette(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->palette.reset();
}

void set_tiles_and_palette(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles,
                           sprite_palette_ptr&& palette)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle_type& handle = item->handle;
    bool different_shape_size = shape_size != hw::sprites::shape_size(handle);
    bool different_tiles = tiles != item->tiles;
    bool different_palette = palette != item->palette;

    if(different_shape_size || different_tiles || different_palette)
    {
        bpp_mode bpp = palette.bpp();
        BN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(bpp),
                  "Invalid tiles, palette or shape size: ", tiles.tiles_count(), " - ", shape_size.tiles_count(bpp));

        if(different_tiles)
        {
            hw::sprites::set_tiles(tiles.id(), handle);
            item->tiles = move(tiles);
        }

        if(different_palette)
        {
            hw::sprites::set_palette(palette.id(), handle);
            hw::sprites::set_bpp(bpp, handle);
            item->palette = move(palette);
        }

        if(different_shape_size)
        {
            _update_item_shape_size(shape_size, *item);
        }
        else
        {
            _update_indexes_to_commit(*item);
        }
    }
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
        int hw_x = item->hw_position.x() + diff;
        item->hw_position.set_x(hw_x);
        hw::sprites::set_x(hw_x, item->handle);

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
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
        int hw_y = item->hw_position.y() + diff;
        item->hw_position.set_y(hw_y);
        hw::sprites::set_y(hw_y, item->handle);

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
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
        point new_hw_position = item->hw_position + diff;
        item->hw_position = new_hw_position;

        hw::sprites::handle_type& handle = item->handle;
        hw::sprites::set_x(new_hw_position.x(), handle);
        hw::sprites::set_y(new_hw_position.y(), handle);

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
    }
}

int bg_priority(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->bg_priority();
}

void set_bg_priority(id_type id, int bg_priority)
{
    auto item = static_cast<item_type*>(id);

    if(bg_priority != item->bg_priority())
    {
        BN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(),
                  "Invalid BG priority: ", bg_priority);

        hw::sprites::set_bg_priority(bg_priority, item->handle);
        data.sorter.erase(*item);
        item->set_bg_priority(bg_priority);
        data.sorter.insert(*item);
        data.rebuild_handles = true;
    }
}

int z_order(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->z_order();
}

void set_z_order(id_type id, int z_order)
{
    auto item = static_cast<item_type*>(id);

    if(z_order != item->z_order())
    {
        BN_ASSERT(z_order >= sprites::min_z_order() && z_order <= sprites::max_z_order(),
                  "Invalid z order: ", z_order);

        data.sorter.erase(*item);
        item->set_z_order(z_order);
        data.sorter.insert(*item);
        data.rebuild_handles = true;
    }
}

void put_above(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(data.sorter.put_in_front_of_layer(*item))
    {
        data.rebuild_handles = true;
    }
}

void put_below(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(data.sorter.put_in_back_of_layer(*item))
    {
        data.rebuild_handles = true;
    }
}

bool horizontal_flip(id_type id)
{
    auto item = static_cast<const item_type*>(id);

    if(const sprite_affine_mat_ptr* item_affine_mat = item->affine_mat.get())
    {
        return item_affine_mat->horizontal_flip();
    }

    return hw::sprites::horizontal_flip(item->handle);
}

void set_horizontal_flip(id_type id, bool horizontal_flip)
{
    auto item = static_cast<item_type*>(id);

    if(sprite_affine_mat_ptr* item_affine_mat = item->affine_mat.get())
    {
        item_affine_mat->set_horizontal_flip(horizontal_flip);
    }
    else
    {
        hw::sprites::handle_type& handle = item->handle;

        if(horizontal_flip != hw::sprites::horizontal_flip(handle))
        {
            hw::sprites::set_horizontal_flip(horizontal_flip, handle);
            _update_indexes_to_commit(*item);
        }
    }
}

bool vertical_flip(id_type id)
{
    auto item = static_cast<const item_type*>(id);

    if(const sprite_affine_mat_ptr* item_affine_mat = item->affine_mat.get())
    {
        return item_affine_mat->vertical_flip();
    }

    return hw::sprites::vertical_flip(item->handle);
}

void set_vertical_flip(id_type id, bool vertical_flip)
{
    auto item = static_cast<item_type*>(id);

    if(sprite_affine_mat_ptr* item_affine_mat = item->affine_mat.get())
    {
        item_affine_mat->set_vertical_flip(vertical_flip);
    }
    else
    {
        hw::sprites::handle_type& handle = item->handle;

        if(vertical_flip != hw::sprites::vertical_flip(handle))
        {
            hw::sprites::set_vertical_flip(vertical_flip, handle);
            _update_indexes_to_commit(*item);
        }
    }
}

bool mosaic_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::mosaic_enabled(item->handle);
}

void set_mosaic_enabled(id_type id, bool mosaic_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle_type& handle = item->handle;

    if(mosaic_enabled != hw::sprites::mosaic_enabled(handle))
    {
        hw::sprites::set_mosaic_enabled(mosaic_enabled, handle);
        _update_indexes_to_commit(*item);
    }
}

bool blending_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->blending_enabled;
}

void set_blending_enabled(id_type id, bool blending_enabled)
{
    auto item = static_cast<item_type*>(id);

    if(blending_enabled != item->blending_enabled)
    {
        hw::sprites::handle_type& handle = item->handle;
        BN_BASIC_ASSERT(! blending_enabled || ! hw::sprites::window_enabled(handle),
                        "Blending and window can't be enabled at the same time");

        hw::sprites::set_blending_enabled(blending_enabled, display_manager::blending_fade_enabled(), handle);
        item->blending_enabled = blending_enabled;
        _update_indexes_to_commit(*item);
    }
}

bool window_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::window_enabled(item->handle);
}

void set_window_enabled(id_type id, bool window_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle_type& handle = item->handle;

    if(window_enabled != hw::sprites::window_enabled(handle))
    {
        BN_BASIC_ASSERT(! window_enabled || ! item->blending_enabled,
                        "Blending and window can't be enabled at the same time");

        hw::sprites::set_window_enabled(window_enabled, handle);
        _update_indexes_to_commit(*item);
    }
}

int view_mode(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::view_mode(item->handle);
}

bool double_size(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->double_size;
}

sprite_double_size_mode double_size_mode(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return sprite_double_size_mode(item->double_size_mode);
}

void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode)
{
    auto item = static_cast<item_type*>(id);

    if(item->double_size_mode != uint8_t(double_size_mode))
    {
        item->double_size_mode = uint8_t(double_size_mode);

        if(sprite_affine_mat_ptr* affine_mat = item->affine_mat.get())
        {
            bool new_double_size = item->new_double_size(affine_mat->id());

            if(item->double_size != new_double_size)
            {
                item->double_size = new_double_size;

                if(item->visible)
                {
                    hw::sprites::show_affine(new_double_size, item->handle);
                }

                _update_item_dimensions(*item);
            }
        }
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

        if(visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
        }
        else
        {
            hw::sprites::hide(item->handle);
            item->on_screen = false;
            item->check_on_screen = false;
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
        item->update_hw_position();

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
    }
}

void remove_camera(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->camera)
    {
        item->camera.reset();
        item->update_hw_position();

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
            data.rebuild_handles = true;
        }
    }
}

optional<sprite_affine_mat_ptr>& affine_mat(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat;
}

void set_affine_mat(id_type id, const sprite_affine_mat_ptr& affine_mat)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat != affine_mat)
    {
        _assign_affine_mat(false, *item, sprite_affine_mat_ptr(affine_mat));
    }
}

void set_affine_mat(id_type id, sprite_affine_mat_ptr&& affine_mat)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat != affine_mat)
    {
        _assign_affine_mat(false, *item, move(affine_mat));
    }
}

void set_new_affine_mat(id_type id, affine_mat_attributes& mat_attributes)
{
    auto item = static_cast<item_type*>(id);
    const hw::sprites::handle_type& handle = item->handle;
    mat_attributes.set_horizontal_flip(hw::sprites::horizontal_flip(handle));
    mat_attributes.set_vertical_flip(hw::sprites::vertical_flip(handle));
    _assign_affine_mat(true, *item, sprite_affine_mat_ptr::create(mat_attributes));
}

void remove_affine_mat(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat)
    {
        _remove_affine_mat(*item);
    }
}

bool remove_affine_mat_when_not_needed(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->remove_affine_mat_when_not_needed;
}

void set_remove_affine_mat_when_not_needed(id_type id, bool remove_when_not_needed)
{
    auto item = static_cast<item_type*>(id);
    item->remove_affine_mat_when_not_needed = remove_when_not_needed;
}

sprite_first_attributes first_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    const hw::sprites::handle_type& handle = item->handle;
    return sprite_first_attributes(item->position.y(), hw::sprites::mosaic_enabled(handle), item->blending_enabled,
                                   hw::sprites::window_enabled(handle), item->visible);
}

void set_first_attributes(id_type id, const sprite_first_attributes& first_attributes)
{
    set_y(id, first_attributes.y());
    set_mosaic_enabled(id, first_attributes.mosaic_enabled());
    set_blending_enabled(id, first_attributes.blending_enabled());
    set_window_enabled(id, first_attributes.window_enabled());
    set_visible(id, first_attributes.visible());
}

sprite_regular_second_attributes regular_second_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    BN_BASIC_ASSERT(! item->affine_mat, "Item is not regular");

    const hw::sprites::handle_type& handle = item->handle;
    return sprite_regular_second_attributes(item->position.x(), hw::sprites::horizontal_flip(handle),
                                            hw::sprites::vertical_flip(handle));
}

void set_regular_second_attributes(id_type id, const sprite_regular_second_attributes& second_attributes)
{
    auto item = static_cast<item_type*>(id);
    BN_BASIC_ASSERT(! item->affine_mat, "Item is not regular");

    set_x(id, second_attributes.x());
    set_horizontal_flip(id, second_attributes.horizontal_flip());
    set_vertical_flip(id, second_attributes.vertical_flip());
}

sprite_affine_second_attributes affine_second_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    BN_BASIC_ASSERT(item->affine_mat, "Item is not affine");

    return sprite_affine_second_attributes(item->position.x(), *item->affine_mat);
}

void set_affine_second_attributes(id_type id, const sprite_affine_second_attributes& second_attributes)
{
    auto item = static_cast<item_type*>(id);
    BN_BASIC_ASSERT(item->affine_mat, "Item is not affine");

    set_x(id, second_attributes.x());
    set_affine_mat(id, second_attributes.affine_mat());
}

sprite_third_attributes third_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return sprite_third_attributes(*item->tiles, *item->palette, item->bg_priority());
}

void set_third_attributes(id_type id, const sprite_third_attributes& third_attributes)
{
    set_tiles_and_palette(id, shape_size(id), bn::sprite_tiles_ptr(third_attributes.tiles()),
                          bn::sprite_palette_ptr(third_attributes.palette()));
    set_bg_priority(id, third_attributes.bg_priority());
}

int reserved_handles_count()
{
    return data.reserved_handles_count;
}

void set_reserved_handles_count(int reserved_handles_count)
{
    int old_reserved_handles_count = data.reserved_handles_count;

    if(reserved_handles_count != old_reserved_handles_count)
    {
        BN_ASSERT(reserved_handles_count >= 0 && reserved_handles_count < hw::sprites::count(),
                  "Invalid reserved handles count: ", reserved_handles_count);

        if(reserved_handles_count > old_reserved_handles_count)
        {
            sprite_affine_mats_manager::reserve_sprite_handles(reserved_handles_count);

            for(sorted_sprites::layer& layer : data.sorter.layers())
            {
                for(item_type& item : layer.items())
                {
                    item.handles_index = -1;
                }
            }
        }

        data.reserved_handles_count = reserved_handles_count;
        reload_all();
    }
}

void reload(id_type id)
{
    auto item = static_cast<item_type*>(id);
    _update_indexes_to_commit(*item);
}

void reload_blending()
{
    bool fade_enabled = display_manager::blending_fade_enabled();

    if(data.rebuild_handles)
    {
        for(sorted_sprites::layer& layer : data.sorter.layers())
        {
            for(item_type& item : layer.items())
            {
                hw::sprites::set_blending_enabled(item.blending_enabled, fade_enabled, item.handle);
            }
        }
    }
    else
    {
        for(sorted_sprites::layer& layer : data.sorter.layers())
        {
            for(item_type& item : layer.items())
            {
                hw::sprites::set_blending_enabled(item.blending_enabled, fade_enabled, item.handle);
                _always_update_indexes_to_commit(item);
            }
        }
    }
}

void reload_all()
{
    data.last_visible_items_count = hw::sprites::count();
    data.rebuild_handles = true;
    data.reload_all_handles = true;
}

void fill_hblank_effect_horizontal_positions(id_type id, int hw_x, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    auto item = static_cast<item_type*>(id);
    uint16_t attr1 = item->handle.attr1;

    if(hw_x == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            uint16_t& dest_value = dest_ptr[index];
            dest_value = attr1;
            hw::sprites::set_x(positions_ptr[index].right_shift_integer(), dest_value);
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            uint16_t& dest_value = dest_ptr[index];
            dest_value = attr1;
            hw::sprites::set_x(hw_x + positions_ptr[index].right_shift_integer(), dest_value);
        }
    }
}

void fill_hblank_effect_vertical_positions(id_type id, int hw_y, const fixed* positions_ptr, uint16_t* dest_ptr)
{
    auto item = static_cast<item_type*>(id);
    uint16_t attr0 = item->handle.attr0;

    if(hw_y == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            uint16_t& dest_value = dest_ptr[index];
            dest_value = attr0;
            hw::sprites::set_y(positions_ptr[index].right_shift_integer(), dest_value);
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            uint16_t& dest_value = dest_ptr[index];
            dest_value = attr0;
            hw::sprites::set_y(hw_y + positions_ptr[index].right_shift_integer(), dest_value);
        }
    }
}

void fill_hblank_effect_first_attributes(int hw_y, sprite_shape shape, bpp_mode bpp, int view_mode,
        const sprite_first_attributes* first_attributes_ptr, uint16_t* dest_ptr)
{
    bool fade_enabled = display_manager::blending_fade_enabled();

    if(hw_y == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_first_attributes& attributes = first_attributes_ptr[index];

            if(attributes.visible())
            {
                int y = attributes.y().right_shift_integer();
                int dest_value = hw::sprites::first_attributes(
                            y, shape, bpp, view_mode, attributes.mosaic_enabled(), attributes.blending_enabled(),
                            attributes.window_enabled(), fade_enabled);
                dest_ptr[index] = uint16_t(dest_value);
            }
            else
            {
                hw::sprites::hide_and_destroy(dest_ptr[index]);
            }
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_first_attributes& attributes = first_attributes_ptr[index];

            if(attributes.visible())
            {
                int y = hw_y + attributes.y().right_shift_integer();
                int dest_value = hw::sprites::first_attributes(
                            y, shape, bpp, view_mode, attributes.mosaic_enabled(), attributes.blending_enabled(),
                            attributes.window_enabled(), fade_enabled);
                dest_ptr[index] = uint16_t(dest_value);
            }
            else
            {
                hw::sprites::hide_and_destroy(dest_ptr[index]);
            }
        }
    }
}

void fill_hblank_effect_regular_second_attributes([[maybe_unused]] id_type id, int hw_x, sprite_size size,
        const sprite_regular_second_attributes* second_attributes_ptr, uint16_t* dest_ptr)
{
    if(hw_x == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_regular_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = second_attributes.x().right_shift_integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.horizontal_flip(),
                                                                      second_attributes.vertical_flip()));
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_regular_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = hw_x + second_attributes.x().right_shift_integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.horizontal_flip(),
                                                                      second_attributes.vertical_flip()));
        }
    }
}

void fill_hblank_effect_affine_second_attributes([[maybe_unused]] id_type id, int hw_x, sprite_size size,
        const sprite_affine_second_attributes* second_attributes_ptr, uint16_t* dest_ptr)
{
    if(hw_x == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_affine_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = second_attributes.x().right_shift_integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.affine_mat().id()));
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_affine_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = hw_x + second_attributes.x().right_shift_integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.affine_mat().id()));
        }
    }
}

void fill_hblank_effect_third_attributes([[maybe_unused]] sprite_shape_size shape_size,
                                         const sprite_third_attributes* third_attributes_ptr, uint16_t* dest_ptr)
{
    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const sprite_third_attributes& attributes = third_attributes_ptr[index];
        const sprite_tiles_ptr& attributes_tiles = attributes.tiles();
        const sprite_palette_ptr& attributes_palette = attributes.palette();
        BN_ASSERT(attributes_tiles.tiles_count() == shape_size.tiles_count(attributes_palette.bpp()),
                  "Invalid tiles or palette: ", attributes_tiles.tiles_count(), " - ",
                  shape_size.tiles_count(attributes_palette.bpp()));

        dest_ptr[index] = uint16_t(hw::sprites::third_attributes(attributes_tiles.id(), attributes_palette.id(),
                                                                 attributes.bg_priority()));
    }
}

void update_cameras()
{
    if(_update_cameras_impl(data.sorter.layers()))
    {
        data.check_items_on_screen = true;
        data.rebuild_handles = true;
    }
}

void remove_identity_affine_mat_when_not_needed(id_type id)
{
    auto item = static_cast<item_type*>(id);
    _remove_affine_mat(*item);
}

void update_auto_double_size(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->visible)
    {
        hw::sprites::show_affine(item->double_size, item->handle);
    }

    _update_item_dimensions(*item);
}

void update()
{
    sprite_affine_mats_manager::update();

    if(data.check_items_on_screen)
    {
        data.check_items_on_screen = false;
        _check_items_on_screen(data.sorter.layers());
    }

    _rebuild_handles();
}

void commit(bool use_dma)
{
    sprite_affine_mats_manager::commit_data affine_mats_commit_data =
            sprite_affine_mats_manager::retrieve_commit_data();
    int first_index_to_commit = data.first_index_to_commit;
    int last_index_to_commit = data.last_index_to_commit;

    if(int count = affine_mats_commit_data.count)
    {
        int multiplier = hw::sprites::count() / hw::sprite_affine_mats::count();
        int first_mat_index_to_commit = affine_mats_commit_data.offset * multiplier;
        int last_mat_index_to_commit = first_mat_index_to_commit + (count * multiplier) - 1;
        first_index_to_commit = min(first_index_to_commit, first_mat_index_to_commit);
        last_index_to_commit = max(last_index_to_commit, last_mat_index_to_commit);
    }

    if(first_index_to_commit < hw::sprites::count())
    {
        int commit_items_count = last_index_to_commit - first_index_to_commit + 1;
        hw::sprites::commit(data.handles[0], first_index_to_commit, commit_items_count, use_dma);
        data.first_index_to_commit = hw::sprites::count();
        data.last_index_to_commit = 0;
    }
}

}

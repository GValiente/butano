#include "btn_sprites_manager.h"

#include "btn_pool.h"
#include "btn_vector.h"
#include "btn_sorted_sprites.h"
#include "btn_config_sprites.h"
#include "btn_sprite_affine_mats.h"
#include "btn_sprite_first_attributes.h"
#include "btn_sprite_third_attributes.h"
#include "btn_sprite_affine_mats_manager.h"
#include "btn_sprite_affine_second_attributes.h"
#include "btn_sprite_regular_second_attributes.h"

namespace btn::sprites_manager
{

namespace
{
    static_assert(BTN_CFG_SPRITES_MAX_ITEMS > 0);

    using item_type = sprites_manager_item;
    using sorted_items_type = vector<item_type*, BTN_CFG_SPRITES_MAX_ITEMS>;

    class static_data
    {

    public:
        pool<item_type, BTN_CFG_SPRITES_MAX_ITEMS> items_pool;
        hw::sprites::handle handles[sprites::sprites_count()];
        int first_index_to_commit = 0;
        int last_index_to_commit = sprites::sprites_count() - 1;
        int last_visible_items_count = 0;
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
    };

    BTN_DATA_EWRAM static_data data;

    void _copy_handle(const hw::sprites::handle& from, hw::sprites::handle& to)
    {
        to.attr0 = from.attr0;
        to.attr1 = from.attr1;
        to.attr2 = from.attr2;
    }

    void _update_indexes_to_commit(int handles_index)
    {
        if(data.first_index_to_commit != sprites::sprites_count()) [[likely]]
        {
            if(handles_index < data.first_index_to_commit)
            {
                data.first_index_to_commit = handles_index;
            }
            else
            {
                data.last_index_to_commit = max(data.last_index_to_commit, handles_index);
            }
        }
        else
        {
            data.first_index_to_commit = handles_index;
            data.last_index_to_commit = handles_index;
        }
    }

    void _update_handle(const item_type& item)
    {
        if(! data.rebuild_handles)
        {
            int handles_index = item.handles_index;

            if(handles_index >= 0)
            {
                _copy_handle(item.handle, data.handles[handles_index]);
                _update_indexes_to_commit(handles_index);
            }
        }
    }

    void _hide_handle(const item_type& item)
    {
        if(! data.rebuild_handles && item.on_screen)
        {
            int handles_index = item.handles_index;
            BTN_ASSERT(handles_index >= 0, "Invalid handles index");

            hw::sprites::hide(data.handles[handles_index]);
            _update_indexes_to_commit(handles_index);
        }
    }

    void _update_item_dimensions(item_type& item)
    {
        item.update_half_dimensions();

        if(item.visible)
        {
            item.check_on_screen = true;
            data.check_items_on_screen = true;
        }
    }

    void _assign_affine_mat(item_type& item, sprite_affine_mat_ptr&& affine_mat)
    {
        bool old_double_size = item.affine_mat && hw::sprites::double_size(item.handle);

        if(item.affine_mat)
        {
            sprite_affine_mats_manager::dettach_sprite(item.affine_mat->id(), *item.attached_sprite);
        }

        item.affine_mat = move(affine_mat);
        item.attached_sprite = &sprite_affine_mats_manager::attach_sprite(item.affine_mat->id(), &item);

        bool new_double_size = item.double_size();
        hw::sprites::set_affine_mat(item.affine_mat->id(), new_double_size, item.handle);

        if(old_double_size != new_double_size)
        {
            _update_item_dimensions(item);
        }

        _update_handle(item);
    }

    void _remove_affine_mat(item_type& item)
    {
        sprite_affine_mat_ptr& affine_mat = *item.affine_mat;
        bool double_size = hw::sprites::double_size(item.handle);
        hw::sprites::set_horizontal_flip(affine_mat.horizontal_flip(), item.handle);
        hw::sprites::set_vertical_flip(affine_mat.vertical_flip(), item.handle);
        hw::sprites::remove_affine_mat(item.handle);
        sprite_affine_mats_manager::dettach_sprite(affine_mat.id(), *item.attached_sprite);
        item.affine_mat.reset();
        item.attached_sprite = nullptr;

        if(double_size)
        {
            _update_item_dimensions(item);
        }

        _update_handle(item);
    }

    void _check_items_on_screen()
    {
        if(data.check_items_on_screen)
        {
            data.check_items_on_screen = false;
            data.rebuild_handles |= _check_items_on_screen_impl();
        }
    }

    void _rebuild_handles()
    {
        if(data.rebuild_handles)
        {
            int visible_items_count = 0;
            data.rebuild_handles = false;

            for(auto& layer : sorted_sprites::layers())
            {
                for(item_type& item : *layer)
                {
                    if(item.on_screen)
                    {
                        BTN_ASSERT(BTN_CFG_SPRITES_MAX_ITEMS <= sprites::sprites_count() ||
                                   visible_items_count <= sprites::sprites_count(), "Too much sprites on screen");

                        _copy_handle(item.handle, data.handles[visible_items_count]);
                        item.handles_index = int8_t(visible_items_count);
                        ++visible_items_count;
                    }
                    else
                    {
                        item.handles_index = -1;
                    }
                }
            }

            int last_visible_items_count = data.last_visible_items_count;
            data.last_visible_items_count = visible_items_count;

            while(visible_items_count < last_visible_items_count)
            {
                hw::sprites::hide(data.handles[visible_items_count]);
                ++visible_items_count;
            }

            if(visible_items_count)
            {
                data.first_index_to_commit = 0;
                data.last_index_to_commit = visible_items_count - 1;
            }
        }
    }
}

void init()
{
    for(hw::sprites::handle& handle : data.handles)
    {
        hw::sprites::hide(handle);
    }

    sprite_affine_mats_manager::init(sizeof(data.handles), data.handles);
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
    BTN_ASSERT(! data.items_pool.full(), "No more sprite items available");

    item_type& new_item = data.items_pool.create(position, shape_size, move(tiles), move(palette));
    sorted_sprites::insert(new_item);
    data.check_items_on_screen |= new_item.visible;
    return &new_item;
}

id_type optional_create(const fixed_point& position, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles,
                        sprite_palette_ptr&& palette)
{
    if(data.items_pool.full())
    {
        return nullptr;
    }

    item_type& new_item = data.items_pool.create(position, shape_size, move(tiles), move(palette));
    sorted_sprites::insert(new_item);
    data.check_items_on_screen |= new_item.visible;
    return &new_item;
}

id_type create(sprite_builder&& builder)
{
    BTN_ASSERT(! data.items_pool.full(), "No more sprite items available");

    sprite_tiles_ptr tiles = builder.release_tiles();
    sprite_palette_ptr palette = builder.release_palette();
    item_type& new_item = data.items_pool.create(move(builder), move(tiles), move(palette));
    sorted_sprites::insert(new_item);
    data.check_items_on_screen |= new_item.visible;
    return &new_item;
}

id_type optional_create(sprite_builder&& builder)
{
    if(data.items_pool.full())
    {
        return nullptr;
    }

    optional<sprite_tiles_ptr> tiles = builder.optional_release_tiles();

    if(! tiles)
    {
        return nullptr;
    }

    optional<sprite_palette_ptr> palette = builder.optional_release_palette();

    if(! palette)
    {
        return nullptr;
    }

    item_type& new_item = data.items_pool.create(move(builder), move(*tiles), move(*palette));
    sorted_sprites::insert(new_item);
    data.check_items_on_screen |= new_item.visible;
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

    if(! item->usages)
    {
        _hide_handle(*item);
        sorted_sprites::erase(*item);

        if(item->affine_mat)
        {
            sprite_affine_mats_manager::dettach_sprite(item->affine_mat->id(), *item->attached_sprite);
        }

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

btn::size dimensions(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->half_dimensions * 2;
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
        BTN_ASSERT(! item->tiles || item->tiles->tiles_count() == tiles.tiles_count(),
                   "Invalid tiles count: ", item->tiles->tiles_count(), " - ", tiles.tiles_count());

        hw::sprites::set_tiles(tiles.id(), item->handle);
        item->tiles = tiles;
        _update_handle(*item);
    }
}

void set_tiles(id_type id, sprite_tiles_ptr&& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        BTN_ASSERT(! item->tiles || item->tiles->tiles_count() == tiles.tiles_count(),
                   "Invalid tiles count: ", item->tiles->tiles_count(), " - ", tiles.tiles_count());

        hw::sprites::set_tiles(tiles.id(), item->handle);
        item->tiles = move(tiles);
        _update_handle(*item);
    }
}

void set_tiles(id_type id, const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(item->palette->bpp_mode()),
                   "Invalid tiles or shape size: ", tiles.tiles_count(), " - ",
                   shape_size.tiles_count(item->palette->bpp_mode()));

        hw::sprites::handle& handle = item->handle;
        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = tiles;
        _update_handle(*item);

        if(shape_size != hw::sprites::shape_size(handle))
        {
            hw::sprites::set_shape_size(shape_size, handle);
            item->update_half_dimensions();

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
        }
    }
}

void remove_tiles(id_type id)
{
    auto item = static_cast<item_type*>(id);
    item->tiles.reset();
}

void set_tiles(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles)
{
    auto item = static_cast<item_type*>(id);

    if(tiles != item->tiles)
    {
        BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(item->palette->bpp_mode()),
                   "Invalid tiles or shape size: ", tiles.tiles_count(), " - ",
                   shape_size.tiles_count(item->palette->bpp_mode()));

        hw::sprites::handle& handle = item->handle;
        hw::sprites::set_tiles(tiles.id(), handle);
        item->tiles = move(tiles);
        _update_handle(*item);

        if(shape_size != hw::sprites::shape_size(handle))
        {
            hw::sprites::set_shape_size(shape_size, handle);
            item->update_half_dimensions();

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
        }
    }
}

const sprite_palette_ptr& palette(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return *item->palette;
}

void set_palette(id_type id, const sprite_palette_ptr& palette)
{
    auto item = static_cast<item_type*>(id);

    if(palette != item->palette)
    {
        BTN_ASSERT(! item->palette || item->palette->bpp_mode() == palette.bpp_mode(),
                   "Palette BPP mode mismatch: ", int(item->palette->bpp_mode()), " - ", int(palette.bpp_mode()));

        hw::sprites::set_palette(palette.id(), item->handle);
        item->palette = palette;
        _update_handle(*item);
    }
}

void set_palette(id_type id, sprite_palette_ptr&& palette)
{
    auto item = static_cast<item_type*>(id);

    if(palette != item->palette)
    {
        BTN_ASSERT(! item->palette || item->palette->bpp_mode() == palette.bpp_mode(),
                   "Palette BPP mode mismatch: ", int(item->palette->bpp_mode()), " - ", int(palette.bpp_mode()));

        hw::sprites::set_palette(palette.id(), item->handle);
        item->palette = move(palette);
        _update_handle(*item);
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
    hw::sprites::handle& handle = item->handle;
    bool different_shape_size = shape_size != hw::sprites::shape_size(handle);
    bool different_tiles = tiles != item->tiles;
    bool different_palette = palette != item->palette;

    if(different_shape_size || different_tiles || different_palette)
    {
        BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
                   "Invalid tiles, palette or shape size: ", tiles.tiles_count(), " - ",
                   shape_size.tiles_count(palette.bpp_mode()));

        if(different_shape_size)
        {
            hw::sprites::set_shape_size(shape_size, handle);
            item->update_half_dimensions();

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
        }

        if(different_tiles)
        {
            hw::sprites::set_tiles(tiles.id(), handle);
            item->tiles = move(tiles);
        }

        if(different_palette)
        {
            hw::sprites::set_palette(palette.id(), handle);
            hw::sprites::set_bpp_mode(palette.bpp_mode(), handle);
            item->palette = move(palette);
        }

        _update_handle(*item);
    }
}

const fixed_point& position(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->position;
}

const fixed_point& hw_position(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->hw_position;
}

void set_x(id_type id, fixed x)
{
    auto item = static_cast<item_type*>(id);
    fixed_point& item_position = item->position;
    fixed x_diff = x - item_position.x();

    if(x_diff != 0)
    {
        fixed hw_x = item->hw_position.x() + x_diff;
        int hw_x_integer = hw_x.integer();
        bool hw_changed = hw_x_integer != item->hw_position.x().integer();
        item_position.set_x(x);
        item->hw_position.set_x(hw_x);

        if(hw_changed)
        {
            hw::sprites::set_x(hw_x_integer, item->handle);
            _update_handle(*item);

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
        }
    }
}

void set_y(id_type id, fixed y)
{
    auto item = static_cast<item_type*>(id);
    fixed_point& item_position = item->position;
    fixed y_diff = y - item_position.y();

    if(y_diff != 0)
    {
        fixed hw_y = item->hw_position.y() + y_diff;
        int hw_y_integer = hw_y.integer();
        bool hw_changed = hw_y_integer != item->hw_position.y().integer();
        item_position.set_y(y);
        item->hw_position.set_y(hw_y);

        if(hw_changed)
        {
            hw::sprites::set_y(hw_y_integer, item->handle);
            _update_handle(*item);

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
        }
    }
}

void set_position(id_type id, const fixed_point& position)
{
    auto item = static_cast<item_type*>(id);
    fixed_point& item_position = item->position;
    fixed_point position_diff = position - item_position;

    if(position_diff != fixed_point())
    {
        fixed_point hw_position = item->hw_position + position_diff;
        int hw_x_integer = hw_position.x().integer();
        int hw_y_integer = hw_position.y().integer();
        bool hw_changed = hw_x_integer != item->hw_position.x().integer() ||
                hw_y_integer != item->hw_position.y().integer();
        item_position = position;
        item->hw_position = hw_position;

        if(hw_changed)
        {
            hw::sprites::handle& handle = item->handle;
            hw::sprites::set_x(hw_x_integer, handle);
            hw::sprites::set_y(hw_y_integer, handle);
            _update_handle(*item);

            if(item->visible)
            {
                item->check_on_screen = true;
                data.check_items_on_screen = true;
            }
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
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= sprites::max_bg_priority(), "Invalid bg priority: ", bg_priority);

    auto item = static_cast<item_type*>(id);

    if(bg_priority != item->bg_priority())
    {
        hw::sprites::set_bg_priority(bg_priority, item->handle);
        sorted_sprites::erase(*item);
        item->set_bg_priority(bg_priority);
        sorted_sprites::insert(*item);
        data.rebuild_handles |= item->on_screen;
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
        sorted_sprites::erase(*item);
        item->set_z_order(z_order);
        sorted_sprites::insert(*item);
        data.rebuild_handles |= item->on_screen;
    }
}

void put_in_front_of_sort_layer(id_type id)
{
    auto item = static_cast<item_type*>(id);
    sorted_sprites::erase(*item);
    sorted_sprites::insert(*item);
    data.rebuild_handles |= item->on_screen;
}

bool horizontal_flip(id_type id)
{
    auto item = static_cast<const item_type*>(id);

    if(item->affine_mat)
    {
        return item->affine_mat->horizontal_flip();
    }

    return hw::sprites::horizontal_flip(item->handle);
}

void set_horizontal_flip(id_type id, bool horizontal_flip)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat)
    {
        item->affine_mat->set_horizontal_flip(horizontal_flip);
    }
    else
    {
        hw::sprites::set_horizontal_flip(horizontal_flip, item->handle);
        _update_handle(*item);
    }
}

bool vertical_flip(id_type id)
{
    auto item = static_cast<const item_type*>(id);

    if(item->affine_mat)
    {
        return item->affine_mat->vertical_flip();
    }

    return hw::sprites::vertical_flip(item->handle);
}

void set_vertical_flip(id_type id, bool vertical_flip)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat)
    {
        item->affine_mat->set_vertical_flip(vertical_flip);
    }
    else
    {
        hw::sprites::set_vertical_flip(vertical_flip, item->handle);
        _update_handle(*item);
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
    hw::sprites::set_mosaic_enabled(mosaic_enabled, item->handle);
    _update_handle(*item);
}

bool blending_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::blending_enabled(item->handle);
}

void set_blending_enabled(id_type id, bool blending_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    BTN_ASSERT(! blending_enabled || ! hw::sprites::window_enabled(handle),
               "Blending and window can't be enabled at the same time");

    hw::sprites::set_blending_enabled(blending_enabled, handle);
    _update_handle(*item);
}

bool window_enabled(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::window_enabled(item->handle);
}

void set_window_enabled(id_type id, bool window_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    BTN_ASSERT(! window_enabled || ! hw::sprites::blending_enabled(handle),
               "Blending and window can't be enabled at the same time");

    hw::sprites::set_window_enabled(window_enabled, handle);
    _update_handle(*item);
}

int affine_mode(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::affine_mode(item->handle);
}

bool double_size(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return hw::sprites::double_size(item->handle);
}

sprite_double_size_mode double_size_mode(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return sprite_double_size_mode(item->double_size_mode);
}

void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode)
{
    auto item = static_cast<item_type*>(id);

    item->double_size_mode = unsigned(double_size_mode);

    if(item->affine_mat)
    {
        bool old_double_size = double_size(id);
        bool new_double_size = item->double_size();

        if(old_double_size != new_double_size)
        {
            hw::sprites::set_affine_mat(item->affine_mat->id(), new_double_size, item->handle);
            _update_item_dimensions(*item);
            _update_handle(*item);
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

        if(visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
        }
        else
        {
            _hide_handle(*item);
            item->on_screen = false;
            item->check_on_screen = false;
        }
    }
}

bool ignore_camera(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return item->ignore_camera;
}

void set_ignore_camera(id_type id, bool ignore_camera)
{
    auto item = static_cast<item_type*>(id);

    if(ignore_camera != item->ignore_camera)
    {
        item->ignore_camera = ignore_camera;
        item->update_hw_position();
        _update_handle(*item);

        if(item->visible)
        {
            item->check_on_screen = true;
            data.check_items_on_screen = true;
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
        _assign_affine_mat(*item, sprite_affine_mat_ptr(affine_mat));
    }
}

void set_affine_mat(id_type id, sprite_affine_mat_ptr&& affine_mat)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat != affine_mat)
    {
        _assign_affine_mat(*item, move(affine_mat));
    }
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
    const hw::sprites::handle& handle = item->handle;
    return sprite_first_attributes(item->position.y(), hw::sprites::mosaic_enabled(handle),
                                   hw::sprites::blending_enabled(handle), hw::sprites::window_enabled(handle));
}

void set_first_attributes(id_type id, const sprite_first_attributes& first_attributes)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    hw::sprites::set_mosaic_enabled(first_attributes.mosaic_enabled(), handle);
    hw::sprites::set_blending_enabled(first_attributes.blending_enabled(), handle);
    hw::sprites::set_window_enabled(first_attributes.window_enabled(), handle);
    set_position(id, fixed_point(item->position.x(), first_attributes.y()));
}

sprite_regular_second_attributes regular_second_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    BTN_ASSERT(! item->affine_mat, "Item is not regular");

    const hw::sprites::handle& handle = item->handle;
    return sprite_regular_second_attributes(item->position.x(), hw::sprites::horizontal_flip(handle),
                                            hw::sprites::vertical_flip(handle));
}

void set_regular_second_attributes(id_type id, const sprite_regular_second_attributes& second_attributes)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(! item->affine_mat, "Item is not regular");

    hw::sprites::set_horizontal_flip(second_attributes.horizontal_flip(), item->handle);
    hw::sprites::set_vertical_flip(second_attributes.vertical_flip(), item->handle);
    set_position(id, fixed_point(second_attributes.x(), item->position.y()));
}

sprite_affine_second_attributes affine_second_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    const optional<sprite_affine_mat_ptr>& affine_mat = item->affine_mat;
    BTN_ASSERT(affine_mat, "Item is not affine");

    return sprite_affine_second_attributes(item->position.x(), *affine_mat);
}

void set_affine_second_attributes(id_type id, const sprite_affine_second_attributes& second_attributes)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->affine_mat, "Item is not affine");

    set_position(id, fixed_point(second_attributes.x(), item->position.y()));
    set_affine_mat(id, second_attributes.affine_mat());
}

sprite_third_attributes third_attributes(id_type id)
{
    auto item = static_cast<const item_type*>(id);
    return sprite_third_attributes(*item->tiles, *item->palette, item->bg_priority());
}

void set_third_attributes(id_type id, const sprite_third_attributes& third_attributes)
{
    auto item = static_cast<item_type*>(id);
    const sprite_tiles_ptr& tiles = third_attributes.tiles();
    const sprite_palette_ptr& palette = third_attributes.palette();
    bool different_tiles = tiles != item->tiles;
    bool different_palette = palette != item->palette;

    if(different_tiles || different_palette)
    {
        BTN_ASSERT(tiles.tiles_count() == shape_size(id).tiles_count(palette.bpp_mode()),
                   "Invalid tiles or palette: ", tiles.tiles_count(), " - ",
                   shape_size(id).tiles_count(palette.bpp_mode()));

        hw::sprites::handle& handle = item->handle;

        if(different_tiles)
        {
            hw::sprites::set_tiles(tiles.id(), handle);
            item->tiles = tiles;
        }

        if(different_palette)
        {
            hw::sprites::set_palette(palette.id(), handle);
            hw::sprites::set_bpp_mode(palette.bpp_mode(), handle);
            item->palette = palette;
        }

        _update_handle(*item);
    }

    set_bg_priority(id, third_attributes.bg_priority());
}

void fill_hblank_effect_first_attributes(fixed hw_y, sprite_shape shape, palette_bpp_mode bpp_mode, int affine_mode,
        const sprite_first_attributes* first_attributes_ptr, uint16_t* dest_ptr)
{
    if(hw_y == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_first_attributes& first_attributes = first_attributes_ptr[index];
            int y = first_attributes.y().integer();
            int dest_value = hw::sprites::first_attributes(
                        y, shape, bpp_mode, affine_mode, first_attributes.mosaic_enabled(),
                        first_attributes.blending_enabled(), first_attributes.window_enabled());
            dest_ptr[index] = uint16_t(dest_value);
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_first_attributes& first_attributes = first_attributes_ptr[index];
            int y = (hw_y + first_attributes.y()).integer();
            int dest_value = hw::sprites::first_attributes(
                        y, shape, bpp_mode, affine_mode, first_attributes.mosaic_enabled(),
                        first_attributes.blending_enabled(), first_attributes.window_enabled());
            dest_ptr[index] = uint16_t(dest_value);
        }
    }
}

void fill_hblank_effect_regular_second_attributes([[maybe_unused]] id_type id, fixed hw_x, sprite_size size,
        const sprite_regular_second_attributes* second_attributes_ptr, uint16_t* dest_ptr)
{
    BTN_ASSERT(! static_cast<item_type*>(id)->affine_mat, "Item is not regular");

    if(hw_x == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_regular_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = second_attributes.x().integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.horizontal_flip(),
                                                                      second_attributes.vertical_flip()));
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_regular_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = (hw_x + second_attributes.x()).integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.horizontal_flip(),
                                                                      second_attributes.vertical_flip()));
        }
    }
}

void fill_hblank_effect_affine_second_attributes([[maybe_unused]] id_type id, fixed hw_x, sprite_size size,
        const sprite_affine_second_attributes* second_attributes_ptr, uint16_t* dest_ptr)
{
    BTN_ASSERT(static_cast<item_type*>(id)->affine_mat, "Item is not affine");

    if(hw_x == 0)
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_affine_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = second_attributes.x().integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.affine_mat().id()));
        }
    }
    else
    {
        for(int index = 0, limit = display::height(); index < limit; ++index)
        {
            const sprite_affine_second_attributes& second_attributes = second_attributes_ptr[index];
            int x = (hw_x + second_attributes.x()).integer();
            dest_ptr[index] = uint16_t(hw::sprites::second_attributes(x, size, second_attributes.affine_mat().id()));
        }
    }
}

void fill_hblank_effect_third_attributes([[maybe_unused]] sprite_shape_size shape_size,
                                         const sprite_third_attributes* third_attributes_ptr, uint16_t* dest_ptr)
{
    for(int index = 0, limit = display::height(); index < limit; ++index)
    {
        const sprite_third_attributes& third_attributes = third_attributes_ptr[index];
        const sprite_tiles_ptr& tiles = third_attributes.tiles();
        const sprite_palette_ptr& palette = third_attributes.palette();
        BTN_ASSERT(tiles.tiles_count() == shape_size.tiles_count(palette.bpp_mode()),
                   "Invalid tiles or palette: ", tiles.tiles_count(), " - ",
                   shape_size.tiles_count(palette.bpp_mode()));

        int bg_priority = third_attributes.bg_priority();
        dest_ptr[index] = uint16_t(hw::sprites::third_attributes(tiles.id(), palette.id(), bg_priority));
    }
}

void update_camera()
{
    update_camera_impl_result result = _update_camera_impl(camera::position());
    data.check_items_on_screen |= result.check_items_on_screen;
    data.rebuild_handles |= result.rebuild_handles;
}

void remove_identity_affine_mat_if_not_needed(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->remove_affine_mat_when_not_needed)
    {
        _remove_affine_mat(*item);
    }
}

void update_affine_mat(id_type id, int affine_mat_id, bool double_size)
{
    auto item = static_cast<item_type*>(id);

    if(sprite_double_size_mode(item->double_size_mode) == sprite_double_size_mode::AUTO)
    {
        hw::sprites::set_affine_mat(affine_mat_id, double_size, item->handle);
        _update_item_dimensions(*item);
        _update_handle(*item);
    }
}

void update()
{
    sprite_affine_mats_manager::update();
    _check_items_on_screen();
    _rebuild_handles();

    if(auto commit_data = sprite_affine_mats_manager::retrieve_commit_data())
    {
        int multiplier = sprites::sprites_count() / sprite_affine_mats::count();
        int first_mat_index_to_commit = commit_data->offset * multiplier;
        int last_mat_index_to_commit = first_mat_index_to_commit + (commit_data->count * multiplier) - 1;
        data.first_index_to_commit = min(data.first_index_to_commit, first_mat_index_to_commit);
        data.last_index_to_commit = max(data.last_index_to_commit, last_mat_index_to_commit);
    }
}

void commit()
{
    int first_index_to_commit = data.first_index_to_commit;

    if(first_index_to_commit < sprites::sprites_count())
    {
        int commit_items_count = data.last_index_to_commit - first_index_to_commit + 1;
        hw::sprites::commit(data.handles[0], first_index_to_commit, commit_items_count);
        data.first_index_to_commit = sprites::sprites_count();
        data.last_index_to_commit = 0;
    }
}

}

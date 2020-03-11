#include "btn_sprites_manager.h"

#include "btn_pool.h"
#include "btn_vector.h"
#include "btn_sorted_sprites.h"
#include "btn_config_sprites.h"
#include "btn_sprite_affine_mats_manager.h"

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
        hw::sprites::handle handles[hw::sprites::count()];
        int first_index_to_commit = hw::sprites::count();
        int last_index_to_commit = 0;
        int last_visible_items_count = 0;
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
    };

    BTN_DATA_EWRAM static_data data;

    void _update_handle(item_type& item)
    {
        if(! data.rebuild_handles)
        {
            int handles_index = item.handles_index;

            if(handles_index >= 0)
            {
                item.handle.copy_to(data.handles[handles_index]);
                data.first_index_to_commit = min(data.first_index_to_commit, handles_index);
                data.last_index_to_commit = max(data.last_index_to_commit, handles_index);
            }
        }
    }

    void _hide_handle(item_type& item)
    {
        if(! data.rebuild_handles && item.on_screen)
        {
            int handles_index = item.handles_index;
            BTN_ASSERT(handles_index >= 0, "Invalid handles index");

            hw::sprites::hide(data.handles[handles_index]);
            data.first_index_to_commit = min(data.first_index_to_commit, handles_index);
            data.last_index_to_commit = max(data.last_index_to_commit, handles_index);
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

    void _assign_affine_mat(sprite_affine_mat_ptr affine_mat_ptr, item_type& item)
    {
        item.affine_mat_ptr = move(affine_mat_ptr);

        bool double_size = item.double_size();
        hw::sprites::set_affine_mat(item.affine_mat_ptr->id(), double_size, item.handle);

        if(double_size)
        {
            _update_item_dimensions(item);
        }
        else
        {
            _update_handle(item);
        }
    }

    void _remove_affine_mat(item_type& item)
    {
        sprite_affine_mat_ptr& affine_mat_ptr = *item.affine_mat_ptr;
        bool double_size = hw::sprites::double_size(item.handle);
        hw::sprites::set_horizontal_flip(affine_mat_ptr.horizontal_flip(), item.handle);
        hw::sprites::set_vertical_flip(affine_mat_ptr.vertical_flip(), item.handle);
        hw::sprites::remove_affine_mat(item.handle);
        item.affine_mat_ptr.reset();

        if(double_size)
        {
            _update_item_dimensions(item);
        }
        else
        {
            _update_handle(item);
        }
    }

    void _check_affine_mats()
    {
        if(sprite_affine_mats_manager::updated())
        {
            for(auto& layer : sorted_sprites::layers())
            {
                for(item_type& item : layer)
                {
                    if(item.affine_mat_ptr)
                    {
                        const sprite_affine_mat_ptr& affine_mat_ptr = *item.affine_mat_ptr;
                        int affine_mat_ptr_id = affine_mat_ptr.id();

                        if(sprite_affine_mats_manager::updated(affine_mat_ptr_id))
                        {
                            if(item.remove_affine_mat_when_not_needed && affine_mat_ptr.is_identity())
                            {
                                _remove_affine_mat(item);
                            }
                            else if(sprite_double_size_mode(item.double_size_mode) == sprite_double_size_mode::AUTO)
                            {
                                bool old_double_size = hw::sprites::double_size(item.handle);
                                bool new_double_size = sprite_affine_mats_manager::double_size(affine_mat_ptr_id);

                                if(old_double_size != new_double_size)
                                {
                                    hw::sprites::set_affine_mat(affine_mat_ptr_id, new_double_size, item.handle);
                                    _update_item_dimensions(item);
                                }
                            }
                        }
                    }
                }
            }

            sprite_affine_mats_manager::update();
        }
    }

    void _check_items_on_screen()
    {
        if(data.check_items_on_screen)
        {
            data.check_items_on_screen = false;
            data.rebuild_handles |= _check_items_on_screen_impl(display::dimensions());
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
                for(item_type& item : layer)
                {
                    if(item.on_screen)
                    {
                        BTN_ASSERT(BTN_CFG_SPRITES_MAX_ITEMS <= hw::sprites::count() ||
                                   visible_items_count <= hw::sprites::count(), "Too much sprites on screen");

                        item.handle.copy_to(data.handles[visible_items_count]);
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

int max_bg_priority()
{
    return hw::sprites::max_bg_priority();
}

int min_z_order()
{
    return item_type::min_z_order();
}

int max_z_order()
{
    return item_type::max_z_order();
}

void init()
{
    hw::sprites::init();
    sprite_affine_mats_manager::init(sizeof(data.handles), data.handles);
}

int used_sprites_count()
{
    return data.last_visible_items_count;
}

int available_sprites_count()
{
    return hw::sprites::count() - used_sprites_count();
}

int used_items_count()
{
    return sorted_sprites::items_count();
}

int available_items_count()
{
    return BTN_CFG_SPRITES_MAX_ITEMS - used_items_count();
}

id_type create(sprite_builder&& builder)
{
    if(data.items_pool.full())
    {
        return nullptr;
    }

    optional<sprite_tiles_ptr> tiles = builder.release_tiles();

    if(! tiles)
    {
        return nullptr;
    }

    optional<sprite_palette_ptr> palette = builder.release_palette();

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
        data.items_pool.destroy(*item);
    }
}

sprite_shape_size shape_size(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return hw::sprites::shape_size(item->handle);
}

size dimensions(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->half_dimensions * 2;
}

const sprite_tiles_ptr& tiles(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->tiles_ptr;
}

void set_tiles(id_type id, const sprite_tiles_ptr& tiles_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(tiles_ptr != item->tiles_ptr)
    {
        BTN_ASSERT(item->tiles_ptr.tiles_count() == tiles_ptr.tiles_count(), "Invalid tiles count: ",
                   item->tiles_ptr.tiles_count(), " - ", tiles_ptr.tiles_count());

        hw::sprites::set_tiles(tiles_ptr.id(), item->handle);
        item->tiles_ptr = tiles_ptr;
        _update_handle(*item);
    }
}

void set_tiles(id_type id, sprite_tiles_ptr&& tiles_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(tiles_ptr != item->tiles_ptr)
    {
        BTN_ASSERT(item->tiles_ptr.tiles_count() == tiles_ptr.tiles_count(), "Invalid tiles count: ",
                   item->tiles_ptr.tiles_count(), " - ", tiles_ptr.tiles_count());

        hw::sprites::set_tiles(tiles_ptr.id(), item->handle);
        item->tiles_ptr = move(tiles_ptr);
        _update_handle(*item);
    }
}

void set_tiles(id_type id, const sprite_shape_size& shape_size, const sprite_tiles_ptr& tiles_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(tiles_ptr != item->tiles_ptr)
    {
        BTN_ASSERT(tiles_ptr.tiles_count() == shape_size.tiles_count(item->palette_ptr.bpp_mode()),
                   "Invalid tiles or shape size: ", tiles_ptr.tiles_count(), " - ",
                   shape_size.tiles_count(item->palette_ptr.bpp_mode()));

        hw::sprites::handle& handle = item->handle;
        hw::sprites::set_tiles(tiles_ptr.id(), handle);
        item->tiles_ptr = tiles_ptr;
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

void set_tiles(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(tiles_ptr != item->tiles_ptr)
    {
        BTN_ASSERT(tiles_ptr.tiles_count() == shape_size.tiles_count(item->palette_ptr.bpp_mode()),
                   "Invalid tiles or shape size: ", tiles_ptr.tiles_count(), " - ",
                   shape_size.tiles_count(item->palette_ptr.bpp_mode()));

        hw::sprites::handle& handle = item->handle;
        hw::sprites::set_tiles(tiles_ptr.id(), handle);
        item->tiles_ptr = move(tiles_ptr);
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
    auto item = static_cast<item_type*>(id);
    return item->palette_ptr;
}

void set_palette(id_type id, const sprite_palette_ptr& palette_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(palette_ptr != item->palette_ptr)
    {
        BTN_ASSERT(item->palette_ptr.bpp_mode() == palette_ptr.bpp_mode(), "Palette BPP mode mismatch: ",
                   int(item->palette_ptr.bpp_mode()), " - ", int(palette_ptr.bpp_mode()));

        hw::sprites::set_palette(palette_ptr.id(), item->handle);
        item->palette_ptr = palette_ptr;
        _update_handle(*item);
    }
}

void set_palette(id_type id, sprite_palette_ptr&& palette_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(palette_ptr != item->palette_ptr)
    {
        BTN_ASSERT(item->palette_ptr.bpp_mode() == palette_ptr.bpp_mode(), "Palette BPP mode mismatch: ",
                   int(item->palette_ptr.bpp_mode()), " - ", int(palette_ptr.bpp_mode()));

        hw::sprites::set_palette(palette_ptr.id(), item->handle);
        item->palette_ptr = move(palette_ptr);
        _update_handle(*item);
    }
}

void set_tiles_and_palette(id_type id, const sprite_shape_size& shape_size, sprite_tiles_ptr&& tiles_ptr,
                           sprite_palette_ptr&& palette_ptr)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    bool different_shape_size = shape_size != hw::sprites::shape_size(handle);
    bool different_tiles = tiles_ptr != item->tiles_ptr;
    bool different_palette = palette_ptr != item->palette_ptr;

    if(different_shape_size || different_tiles || different_palette)
    {
        palette_bpp_mode bpp_mode = palette_ptr.bpp_mode();
        BTN_ASSERT(tiles_ptr.tiles_count() == shape_size.tiles_count(bpp_mode),
                   "Invalid tiles, palette or shape size: ", tiles_ptr.tiles_count(), " - ",
                   shape_size.tiles_count(bpp_mode));

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
            hw::sprites::set_tiles(tiles_ptr.id(), handle);
            item->tiles_ptr = move(tiles_ptr);
        }

        if(different_palette)
        {
            hw::sprites::set_palette(palette_ptr.id(), handle);
            hw::sprites::set_bpp_mode(bpp_mode, handle);
            item->palette_ptr = move(palette_ptr);
        }

        _update_handle(*item);
    }
}

const fixed_point& position(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->position;
}

void set_position(id_type id, const fixed_point& position)
{
    auto item = static_cast<item_type*>(id);
    item->position = position;
    item->update_hw_position();
    _update_handle(*item);

    if(item->visible)
    {
        item->check_on_screen = true;
        data.check_items_on_screen = true;
    }
}

int bg_priority(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->bg_priority();
}

void set_bg_priority(id_type id, int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= hw::sprites::max_bg_priority(), "Invalid bg priority: ", bg_priority);

    auto item = static_cast<item_type*>(id);

    if(bg_priority != item->bg_priority())
    {
        hw::sprites::set_bg_priority(bg_priority, item->handle);
        sorted_sprites::erase(*item);
        item->update_sort_key(bg_priority, item->z_order());
        sorted_sprites::insert(*item);
        data.rebuild_handles |= item->on_screen;
    }
}

int z_order(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->z_order();
}

void set_z_order(id_type id, int z_order)
{
    BTN_ASSERT(z_order >= item_type::min_z_order() && z_order <= item_type::max_z_order(),
               "Invalid z order: ", z_order);

    auto item = static_cast<item_type*>(id);

    if(z_order != item->z_order())
    {
        sorted_sprites::erase(*item);
        item->update_sort_key(item->bg_priority(), z_order);
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
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat_ptr)
    {
        return item->affine_mat_ptr->horizontal_flip();
    }

    return hw::sprites::horizontal_flip(item->handle);
}

void set_horizontal_flip(id_type id, bool horizontal_flip)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat_ptr)
    {
        item->affine_mat_ptr->set_horizontal_flip(horizontal_flip);
    }
    else
    {
        hw::sprites::set_horizontal_flip(horizontal_flip, item->handle);
        _update_handle(*item);
    }
}

bool vertical_flip(id_type id)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat_ptr)
    {
        return item->affine_mat_ptr->vertical_flip();
    }

    return hw::sprites::vertical_flip(item->handle);
}

void set_vertical_flip(id_type id, bool vertical_flip)
{
    auto item = static_cast<item_type*>(id);

    if(item->affine_mat_ptr)
    {
        item->affine_mat_ptr->set_vertical_flip(vertical_flip);
    }
    else
    {
        hw::sprites::set_vertical_flip(vertical_flip, item->handle);
        _update_handle(*item);
    }
}

bool mosaic_enabled(id_type id)
{
    auto item = static_cast<item_type*>(id);
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
    auto item = static_cast<item_type*>(id);
    return hw::sprites::blending_enabled(item->handle);
}

void set_blending_enabled(id_type id, bool blending_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    BTN_ASSERT(blending_enabled && hw::sprites::window_enabled(handle),
               "Blending and window can't be enabled at the same time");

    hw::sprites::set_blending_enabled(blending_enabled, handle);
    _update_handle(*item);
}

bool window_enabled(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return hw::sprites::window_enabled(item->handle);
}

void set_window_enabled(id_type id, bool window_enabled)
{
    auto item = static_cast<item_type*>(id);
    hw::sprites::handle& handle = item->handle;
    BTN_ASSERT(window_enabled && hw::sprites::blending_enabled(handle),
               "Blending and window can't be enabled at the same time");

    hw::sprites::set_window_enabled(window_enabled, item->handle);
    _update_handle(*item);
}

bool double_size(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return hw::sprites::double_size(item->handle);
}

sprite_double_size_mode double_size_mode(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return sprite_double_size_mode(item->double_size_mode);
}

void set_double_size_mode(id_type id, sprite_double_size_mode double_size_mode)
{
    auto item = static_cast<item_type*>(id);

    item->double_size_mode = unsigned(double_size_mode);

    if(item->affine_mat_ptr)
    {
        bool old_double_size = double_size(id);
        bool new_double_size = item->double_size();

        if(old_double_size != new_double_size)
        {
            hw::sprites::set_affine_mat(item->affine_mat_ptr->id(), new_double_size, item->handle);
            _update_item_dimensions(*item);
        }
    }
}

bool visible(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->visible;
}

void set_visible(id_type id, bool visible)
{
    auto item = static_cast<item_type*>(id);

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

bool ignore_camera(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->ignore_camera;
}

void set_ignore_camera(id_type id, bool ignore_camera)
{
    auto item = static_cast<item_type*>(id);
    item->ignore_camera = ignore_camera;
    item->update_hw_position();
    _update_handle(*item);

    if(item->visible)
    {
        item->check_on_screen = true;
        data.check_items_on_screen = true;
    }
}

optional<sprite_affine_mat_ptr>& affine_mat(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_ptr;
}

void set_affine_mat(id_type id, const optional<sprite_affine_mat_ptr>& affine_mat_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(affine_mat_ptr)
    {
        const sprite_affine_mat_ptr& affine_mat = *affine_mat_ptr;

        if(item->affine_mat_ptr)
        {
            if(item->affine_mat_ptr == affine_mat)
            {
                return;
            }
        }

        if(item->remove_affine_mat_when_not_needed && affine_mat.is_identity())
        {
            if(item->affine_mat_ptr)
            {
                _remove_affine_mat(*item);
            }
        }
        else
        {
            _assign_affine_mat(affine_mat, *item);
        }
    }
    else
    {
        if(item->affine_mat_ptr)
        {
            _remove_affine_mat(*item);
        }
    }
}

void set_affine_mat(id_type id, optional<sprite_affine_mat_ptr>&& affine_mat_ptr)
{
    auto item = static_cast<item_type*>(id);

    if(affine_mat_ptr)
    {
        sprite_affine_mat_ptr& affine_mat = *affine_mat_ptr;

        if(item->affine_mat_ptr)
        {
            if(item->affine_mat_ptr == affine_mat)
            {
                return;
            }
        }

        if(item->remove_affine_mat_when_not_needed && affine_mat.is_identity())
        {
            if(item->affine_mat_ptr)
            {
                _remove_affine_mat(*item);
            }
        }
        else
        {
            _assign_affine_mat(move(affine_mat), *item);
        }
    }
    else
    {
        if(item->affine_mat_ptr)
        {
            _remove_affine_mat(*item);
        }
    }
}

bool remove_affine_mat_when_not_needed(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->remove_affine_mat_when_not_needed;
}

void set_remove_affine_mat_when_not_needed(id_type id, bool remove_when_not_needed)
{
    auto item = static_cast<item_type*>(id);
    item->remove_affine_mat_when_not_needed = remove_when_not_needed;

    if(remove_when_not_needed && item->affine_mat_ptr && item->affine_mat_ptr->is_identity())
    {
        _remove_affine_mat(*item);
    }
}

void update_camera()
{
    update_camera_impl_result result = _update_camera_impl(camera::position());
    data.check_items_on_screen |= result.check_items_on_screen;
    data.rebuild_handles |= result.rebuild_handles;
}

void update()
{
    _check_affine_mats();
    _check_items_on_screen();
    _rebuild_handles();
}

void commit()
{
    int first_index_to_commit = data.first_index_to_commit;
    int last_index_to_commit = data.last_index_to_commit;

    if(auto commit_data = sprite_affine_mats_manager::retrieve_commit_data())
    {
        int multiplier = hw::sprites::count() / sprite_affine_mats_manager::count();
        int first_mat_index_to_commit = commit_data->offset * multiplier;
        int last_mat_index_to_commit = first_mat_index_to_commit + (commit_data->count * multiplier) - 1;
        first_index_to_commit = min(first_index_to_commit, first_mat_index_to_commit);
        last_index_to_commit = max(last_index_to_commit, last_mat_index_to_commit);
    }

    if(first_index_to_commit < hw::sprites::count())
    {
        int commit_items_count = last_index_to_commit - first_index_to_commit + 1;
        hw::sprites::commit(data.handles[0], first_index_to_commit, commit_items_count);
        data.first_index_to_commit = hw::sprites::count();
        data.last_index_to_commit = 0;
    }
}

}

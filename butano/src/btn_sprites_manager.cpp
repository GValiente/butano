#include "btn_sprites_manager.h"

#include "btn_sort.h"
#include "btn_pool.h"
#include "btn_size.h"
#include "btn_color.h"
#include "btn_vector.h"
#include "btn_camera.h"
#include "btn_display.h"
#include "btn_algorithm.h"
#include "btn_sprite_builder.h"
#include "btn_config_sprites.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "btn_sprite_affine_mats_manager.h"
#include "../hw/include/btn_hw_sprites.h"
#include "../hw/include/btn_hw_sprite_affine_mats.h"

namespace btn::sprites_manager
{

namespace
{
    static_assert(BTN_CFG_SPRITES_MAX_ITEMS > 0);

    class item_type
    {

    public:
        hw::sprites::handle handle;
        fixed_point position;
        unsigned usages = 1;
        unsigned sort_key;
        sprite_tiles_ptr tiles_ptr;
        int8_t handles_index = -1;
        optional<sprite_affine_mat_ptr> affine_mat_ptr;
        sprite_palette_ptr palette_ptr;
        unsigned double_size_mode: 2;
        unsigned visible: 1;
        unsigned ignore_camera: 1;
        unsigned remove_affine_mat_when_not_needed: 1;
        unsigned on_screen: 1;
        unsigned check_on_screen: 1;

        item_type(sprite_builder&& builder, sprite_tiles_ptr&& tiles, sprite_palette_ptr&& palette) :
            position(builder.position()),
            tiles_ptr(move(tiles)),
            affine_mat_ptr(builder.release_affine_mat()),
            palette_ptr(move(palette))
        {
            bool eight_bits_per_pixel = palette_ptr.eight_bits_per_pixel();
            remove_affine_mat_when_not_needed = builder.remove_affine_mat_when_not_needed();
            double_size_mode = unsigned(builder.double_size_mode());

            if(affine_mat_ptr)
            {
                if(remove_affine_mat_when_not_needed && affine_mat_ptr->is_identity())
                {
                    affine_mat_ptr.reset();
                    hw::sprites::setup_regular(builder, tiles_ptr.id(), palette_ptr.id(), eight_bits_per_pixel, handle);
                }
                else
                {
                    hw::sprites::setup_affine(builder, tiles_ptr.id(), palette_ptr.id(), eight_bits_per_pixel, handle);
                    hw::sprites::set_affine_mat(affine_mat_ptr->id(), double_size(), handle);
                }
            }
            else
            {
                hw::sprites::setup_regular(builder, tiles_ptr.id(), palette_ptr.id(), eight_bits_per_pixel, handle);
            }

            fixed_point real_position = position;
            ignore_camera = builder.ignore_camera();

            if(! ignore_camera)
            {
                real_position -= camera::position();
            }

            hw::sprites::set_position(real_position.x().integer(), real_position.y().integer(), handle);
            update_sort_key(builder.bg_priority(), builder.z_order());
            on_screen = false;

            if(builder.visible())
            {
                visible = true;
                check_on_screen = true;
            }
            else
            {
                visible = false;
                check_on_screen = false;
            }
        }

        [[nodiscard]] bool double_size() const
        {
            switch(sprite_double_size_mode(double_size_mode))
            {

            case sprite_double_size_mode::AUTO:
                return sprite_affine_mats_manager::double_size(affine_mat_ptr->id());

            case sprite_double_size_mode::ENABLED:
                return true;

            case sprite_double_size_mode::DISABLED:
                return false;
            }

            return false;
        }

        [[nodiscard]] int bg_priority() const
        {
            return sort_key / z_orders();
        }

        [[nodiscard]] int z_order() const
        {
            return sort_key % z_orders();
        }

        void update_sort_key(int bg_priority, int z_order)
        {
            sort_key = (unsigned(bg_priority) * z_orders()) + unsigned(z_order);
        }

        void delete_sort_key()
        {
            sort_key = numeric_limits<unsigned>::max();
        }
    };

    using sorted_items_type = vector<item_type*, BTN_CFG_SPRITES_MAX_ITEMS>;

    class static_data
    {

    public:
        pool<item_type, BTN_CFG_SPRITES_MAX_ITEMS> items_pool;
        sorted_items_type sorted_items;
        hw::sprites::handle handles[hw::sprites::count()];
        int first_index_to_commit = hw::sprites::count();
        int last_index_to_commit = 0;
        int last_visible_items_count = 0;
        bool sort_items = false;
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
    };

    BTN_DATA_EWRAM static_data data;

    void _update_handles(item_type& item)
    {
        int handles_index = item.handles_index;

        if(handles_index >= 0)
        {
            item.handle.copy_to(data.handles[handles_index]);
            data.first_index_to_commit = min(data.first_index_to_commit, handles_index);
            data.last_index_to_commit = max(data.last_index_to_commit, handles_index);
        }
    }

    void _assign_affine_mat(sprite_affine_mat_ptr affine_mat_ptr, item_type& item)
    {
        item.affine_mat_ptr = move(affine_mat_ptr);

        bool double_size = item.double_size();
        hw::sprites::set_affine_mat(item.affine_mat_ptr->id(), double_size, item.handle);

        if(double_size)
        {
            set_position(&item, item.position);
        }
        else
        {
            _update_handles(item);
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
            set_position(&item, item.position);
        }
        else
        {
            _update_handles(item);
        }
    }

    void _sort_items()
    {
        if(data.sort_items)
        {
            data.sort_items = false;

            auto comparator = [](const sorted_items_type::iterator& a, const sorted_items_type::iterator& b)
            {
                return (*a)->sort_key < (*b)->sort_key;
            };

            shell_sort<sorted_items_type::iterator, decltype(comparator)>(data.sorted_items.begin(),
                                                                          data.sorted_items.end(), comparator);

            size_t items_count = data.sorted_items.size();

            while(items_count)
            {
                --items_count;

                item_type* item = data.sorted_items[items_count];

                if(! item->usages)
                {
                    data.items_pool.destroy<item_type>(item);
                    data.sorted_items.pop_back();
                }
                else
                {
                    break;
                }
            }
        }
    }

    void _check_affine_mats()
    {
        if(sprite_affine_mats_manager::updated())
        {
            for(item_type* item : data.sorted_items)
            {
                if(item->affine_mat_ptr)
                {
                    const sprite_affine_mat_ptr& affine_mat_ptr = *item->affine_mat_ptr;
                    int affine_mat_ptr_id = affine_mat_ptr.id();

                    if(sprite_affine_mats_manager::updated(affine_mat_ptr_id))
                    {
                        if(item->remove_affine_mat_when_not_needed && affine_mat_ptr.is_identity())
                        {
                            _remove_affine_mat(*item);
                        }
                        else if(sprite_double_size_mode(item->double_size_mode) == sprite_double_size_mode::AUTO)
                        {
                            bool old_double_size = hw::sprites::double_size(item->handle);
                            bool new_double_size = sprite_affine_mats_manager::double_size(affine_mat_ptr_id);

                            if(old_double_size != new_double_size)
                            {
                                hw::sprites::set_affine_mat(affine_mat_ptr_id, new_double_size, item->handle);
                                set_position(item, item->position);
                            }
                        }
                    }
                }
            }
        }

        sprite_affine_mats_manager::update();
    }

    void _check_items_on_screen()
    {
        if(data.check_items_on_screen)
        {
            int display_width = display::width();
            int display_height = display::height();
            bool rebuild_handles = data.rebuild_handles;
            data.check_items_on_screen = false;

            for(item_type* item : data.sorted_items)
            {
                if(item->check_on_screen)
                {
                    fixed_point position = item->position;

                    if(! item->ignore_camera)
                    {
                        position -= camera::position();
                    }

                    size dimensions = hw::sprites::dimensions(item->handle);
                    int x = position.x().integer() - (dimensions.width() / 2);
                    bool on_screen = false;
                    item->check_on_screen = false;

                    if(x + dimensions.width() > 0 && x < display_width)
                    {
                        int y = position.y().integer() - (dimensions.height() / 2);

                        if(y + dimensions.height() > 0 && y < display_height)
                        {
                            on_screen = true;
                        }
                    }

                    if(on_screen != item->on_screen)
                    {
                        item->on_screen = on_screen;
                        rebuild_handles = true;
                    }
                }
            }

            data.rebuild_handles = rebuild_handles;
        }
    }

    void _rebuild_handles()
    {
        if(data.rebuild_handles)
        {
            int visible_items_count = 0;
            data.rebuild_handles = false;

            for(item_type* item : data.sorted_items)
            {
                if(item->on_screen)
                {
                    BTN_ASSERT(visible_items_count < hw::sprites::count(), "Too much sprites on screen");

                    item->handle.copy_to(data.handles[visible_items_count]);
                    item->handles_index = int8_t(visible_items_count);
                    ++visible_items_count;
                }
                else
                {
                    item->handles_index = -1;
                }
            }

            int last_visible_items_count = data.last_visible_items_count;
            int items_to_hide = last_visible_items_count - visible_items_count;
            data.last_visible_items_count = visible_items_count;

            if(items_to_hide > 0)
            {
                hw::sprites::hide(items_to_hide, data.handles[visible_items_count]);
                visible_items_count = last_visible_items_count;
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

void init()
{
    hw::sprites::init();
    sprite_affine_mats_manager::init(sizeof(data.handles), data.handles);
}

optional<id_type> create(sprite_builder&& builder)
{
    if(data.items_pool.full())
    {
        _sort_items();

        if(data.items_pool.full())
        {
            return nullopt;
        }
    }

    optional<sprite_tiles_ptr> tiles = builder.release_tiles();

    if(! tiles)
    {
        return nullopt;
    }

    optional<sprite_palette_ptr> palette = builder.release_palette();

    if(! palette)
    {
        return nullopt;
    }

    item_type* new_item = data.items_pool.create<item_type>(move(builder), move(*tiles), move(*palette));
    sorted_items_type& sorted_items = data.sorted_items;

    if(! data.sort_items && ! sorted_items.empty() && new_item->sort_key < sorted_items.back()->sort_key)
    {
        data.sort_items = true;
    }

    sorted_items.push_back(new_item);

    if(builder.visible())
    {
        data.check_items_on_screen = true;
    }

    return new_item;
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
        item->delete_sort_key();
        data.sort_items = true;

        if(item->on_screen)
        {
            data.rebuild_handles = true;
        }
    }
}

size dimensions(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return hw::sprites::dimensions(item->handle);
}

const sprite_tiles_ptr& tiles_ptr(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->tiles_ptr;
}

void set_tiles_ptr(id_type id, sprite_tiles_ptr tiles_ptr)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->tiles_ptr.tiles_count() == tiles_ptr.tiles_count(), "Invalid tiles count: ",
               item->tiles_ptr.tiles_count(), " - ", tiles_ptr.tiles_count());

    if(tiles_ptr != item->tiles_ptr)
    {
        hw::sprites::set_tiles(tiles_ptr.id(), item->handle);
        item->tiles_ptr = move(tiles_ptr);
        _update_handles(*item);
    }
}

const sprite_palette_ptr& palette_ptr(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->palette_ptr;
}

void set_palette_ptr(id_type id, sprite_palette_ptr palette_ptr)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->palette_ptr.eight_bits_per_pixel() == palette_ptr.eight_bits_per_pixel(),
               "Palette colors bpp mode mismatch: ",
               item->palette_ptr.eight_bits_per_pixel(), " - ", palette_ptr.eight_bits_per_pixel());

    if(palette_ptr != item->palette_ptr)
    {
        hw::sprites::set_palette(palette_ptr.id(), item->handle);
        item->palette_ptr = move(palette_ptr);
        _update_handles(*item);
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
    fixed_point real_position = position;

    if(! item->ignore_camera)
    {
        real_position -= camera::position();
    }

    hw::sprites::set_position(real_position.x().integer(), real_position.y().integer(), item->handle);
    item->position = position;
    _update_handles(*item);

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
        item->update_sort_key(bg_priority, item->z_order());
        data.sort_items = true;

        if(item->on_screen)
        {
            data.rebuild_handles = true;
        }
    }
}

int z_order(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->z_order();
}

void set_z_order(id_type id, int z_order)
{
    BTN_ASSERT(z_order >= 0 && z_order < int(sprites_manager::z_orders()), "Invalid z order: ", z_order);

    auto item = static_cast<item_type*>(id);

    if(z_order != item->z_order())
    {
        item->update_sort_key(item->bg_priority(), z_order);
        data.sort_items = true;

        if(item->on_screen)
        {
            data.rebuild_handles = true;
        }
    }
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
        _update_handles(*item);
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
        _update_handles(*item);
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
    _update_handles(*item);
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
            set_position(id, item->position);
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
        if(item->on_screen)
        {
            data.rebuild_handles = true;
        }

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
    set_position(id, item->position);
}

optional<sprite_affine_mat_ptr>& affine_mat_ptr(id_type id)
{
    auto item = static_cast<item_type*>(id);
    return item->affine_mat_ptr;
}

void set_affine_mat_ptr(id_type id, optional<sprite_affine_mat_ptr> affine_mat_ptr)
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
    _sort_items();

    for(item_type* item : data.sorted_items)
    {
        if(! item->ignore_camera)
        {
            set_position(item, item->position);
        }
    }
}

void update()
{
    _sort_items();
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
        int multiplier = hw::sprites::count() / hw::sprite_affine_mats::count();
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

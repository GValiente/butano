#include "btn_sprites_manager.h"

#include "btn_sort.h"
#include "btn_pool.h"
#include "btn_size.h"
#include "btn_color.h"
#include "btn_vector.h"
#include "btn_display.h"
#include "btn_utility.h"
#include "btn_sprite_builder.h"
#include "btn_config_sprites.h"
#include "btn_sprite_tiles_ptr.h"
#include "btn_sprite_palette_ptr.h"
#include "../hw/include/btn_hw_sprites.h"

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
        sprite_tiles_ptr tiles_ptr;
        unsigned usages = 1;
        unsigned sort_key;
        sprite_palette_ptr palette_ptr;
        unsigned visible: 1;
        unsigned on_screen: 1;
        unsigned check_on_screen: 1;

        explicit item_type(const sprite_builder& builder) :
            position(builder.position()),
            tiles_ptr(builder.tiles_ptr()),
            palette_ptr(builder.palette_ptr())
        {
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
            sort_key = integral_limits<unsigned>::max;
        }
    };

    using sorted_items_type = vector<item_type*, BTN_CFG_SPRITES_MAX_ITEMS>;

    class static_data
    {

    public:
        pool<item_type, BTN_CFG_SPRITES_MAX_ITEMS> items_pool;
        sorted_items_type sorted_items_vector;
        hw::sprites::handle handles[hw::sprites::available_sprites()];
        int commit_items_count = 0;
        int last_visible_items_count = 0;
        bool sort_items = false;
        bool check_items_on_screen = false;
        bool rebuild_handles = false;
    };

    BTN_DATA_EWRAM static_data data;

    void _check_items_on_screen()
    {
        if(data.check_items_on_screen)
        {
            int display_width = display::width();
            int display_height = display::height();
            bool rebuild_handles = data.rebuild_handles;
            data.check_items_on_screen = false;

            for(item_type* item : data.sorted_items_vector)
            {
                if(item->check_on_screen)
                {
                    size dimensions = hw::sprites::dimensions(item->handle);
                    const fixed_point& position = item->position;
                    int x = position.x().integer();
                    bool on_screen = false;
                    item->check_on_screen = false;

                    if(x + dimensions.width() > 0 && x < display_width)
                    {
                        int y = position.y().integer();

                        if(y + dimensions.height() > 0 && y < display_height)
                        {
                            on_screen = true;
                        }
                    }

                    if(on_screen || on_screen != item->on_screen)
                    {
                        item->on_screen = on_screen;
                        rebuild_handles = true;
                    }
                }
            }

            data.rebuild_handles = rebuild_handles;
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

            shell_sort<sorted_items_type::iterator, decltype(comparator)>(data.sorted_items_vector.begin(),
                                                                          data.sorted_items_vector.end(), comparator);

            size_t items_count = data.sorted_items_vector.size();

            while(items_count)
            {
                --items_count;

                item_type* item = data.sorted_items_vector[items_count];

                if(! item->usages)
                {
                    data.items_pool.destroy<item_type>(item);
                    data.sorted_items_vector.pop_back();
                }
                else
                {
                    break;
                }
            }
        }
    }

    void _rebuild_handles()
    {
        if(data.rebuild_handles)
        {
            int visible_items_count = 0;
            data.rebuild_handles = false;

            for(item_type* item : data.sorted_items_vector)
            {
                if(item->on_screen)
                {
                    BTN_ASSERT(visible_items_count < hw::sprites::available_sprites(), "Too much sprites on screen");

                    data.handles[visible_items_count] = item->handle;
                    ++visible_items_count;
                }
            }

            int last_visible_items_count = data.last_visible_items_count;
            data.last_visible_items_count = visible_items_count;

            while(visible_items_count < last_visible_items_count)
            {
                hw::sprites::hide(data.handles[visible_items_count]);
                ++visible_items_count;
            }

            data.commit_items_count = visible_items_count;
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
}

id_type create(const sprite_builder& builder)
{
    if(data.items_pool.full())
    {
        _sort_items();
        BTN_ASSERT(! data.items_pool.full(), "No more items allowed");
    }

    const fixed_point& position = builder.position();
    item_type* new_item = data.items_pool.create<item_type>(builder);
    hw::sprites::setup(builder.shape(), builder.size(), new_item->tiles_ptr.id(), new_item->palette_ptr.id(),
                       new_item->palette_ptr.colors_count() > 16, position.x().integer(), position.y().integer(),
                       builder.bg_priority(), new_item->handle);
    data.sorted_items_vector.push_back(new_item);
    data.sort_items = true;

    if(builder.visible())
    {
        data.check_items_on_screen = true;
    }

    return new_item;
}

void increase_usages(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    ++item->usages;
}

void decrease_usages(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

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
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return hw::sprites::dimensions(item->handle);
}

const sprite_tiles_ptr& tiles_ptr(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->tiles_ptr;
}

void set_tiles_ptr(id_type id, sprite_tiles_ptr tiles_ptr)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");
    BTN_ASSERT(item->tiles_ptr.tiles_count() == tiles_ptr.tiles_count(), "Invalid sprite tiles count: ",
               item->tiles_ptr.tiles_count(), " - ", tiles_ptr.tiles_count());

    hw::sprites::set_tile(tiles_ptr.id(), item->handle);
    item->tiles_ptr = move(tiles_ptr);

    if(item->on_screen)
    {
        data.rebuild_handles = true;
    }
}

const sprite_palette_ptr& palette_ptr(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->palette_ptr;
}

void set_palette_ptr(id_type id, sprite_palette_ptr palette_ptr)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");
    BTN_ASSERT(item->palette_ptr.colors_count() == palette_ptr.colors_count(), "Invalid sprite palette colors count: ",
               item->palette_ptr.colors_count(), " - ", palette_ptr.colors_count());

    hw::sprites::set_palette(palette_ptr.id(), item->handle);
    item->palette_ptr = move(palette_ptr);

    if(item->on_screen)
    {
        data.rebuild_handles = true;
    }
}

const fixed_point& position(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->position;
}

void set_position(id_type id, const fixed_point& position)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    hw::sprites::set_position(position.x().integer(), position.y().integer(), item->handle);
    item->position = position;

    if(item->visible)
    {
        item->check_on_screen = true;
        data.check_items_on_screen = true;
    }
}

int bg_priority(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->bg_priority();
}

void set_bg_priority(id_type id, int bg_priority)
{
    BTN_ASSERT(bg_priority >= 0 && bg_priority <= hw::sprites::max_bg_priority(), "Invalid bg_priority: ", bg_priority);

    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

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
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->z_order();
}

void set_z_order(id_type id, int z_order)
{
    BTN_ASSERT(z_order >= 0 && z_order < int(sprites_manager::z_orders()), "Invalid z_order: ", z_order);

    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

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

bool visible(id_type id)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    return item->visible;
}

void set_visible(id_type id, bool visible)
{
    auto item = static_cast<item_type*>(id);
    BTN_ASSERT(item->usages, "Sprite item is not used");

    item->visible = visible;

    if(visible)
    {
        item->check_on_screen = true;
        data.check_items_on_screen = true;
    }
    else
    {
        item->on_screen = false;
        item->check_on_screen = false;
        data.rebuild_handles = true;
    }
}

void update()
{
    _sort_items();
    _check_items_on_screen();
    _rebuild_handles();
}

void commit()
{
    if(data.commit_items_count)
    {
        hw::sprites::commit(data.handles, data.commit_items_count);
        data.commit_items_count = 0;
    }
}

}

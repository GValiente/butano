#include "btn_sprites_manager.h"

#include "btn_vector.h"
#include "btn_sorted_sprites.h"

namespace btn::sprites_manager
{

void _check_items_on_screen_impl()
{
    fixed display_width = display::width();
    fixed display_height = display::height();

    for(sorted_sprites::layer* layer : sorted_sprites::layers())
    {
        for(sprites_manager_item& item : *layer)
        {
            if(item.check_on_screen)
            {
                fixed x = item.hw_position.x();
                bool on_screen = false;
                item.check_on_screen = false;

                if(x < display_width)
                {
                    fixed y = item.hw_position.y();

                    if(y < display_height)
                    {
                        if(x + (item.half_width * 2) > 0)
                        {
                            if(y + (item.half_height * 2) > 0)
                            {
                                on_screen = true;
                            }
                        }
                    }
                }

                item.on_screen = on_screen;
            }
        }
    }
}

int _rebuild_handles_impl(int last_visible_items_count, void* hw_handles)
{
    auto handles = reinterpret_cast<hw::sprites::handle_type*>(hw_handles);
    int visible_items_count = 0;

    for(sorted_sprites::layer* layer : sorted_sprites::layers())
    {
        for(sprites_manager_item& item : *layer)
        {
            if(item.on_screen)
            {
                BTN_ASSERT(BTN_CFG_SPRITES_MAX_ITEMS <= sprites::sprites_count() ||
                           visible_items_count <= sprites::sprites_count(), "Too much sprites on screen");

                hw::sprites::copy_handle(item.handle, handles[visible_items_count]);
                item.handles_index = int8_t(visible_items_count);
                ++visible_items_count;
            }
            else
            {
                item.handles_index = -1;
            }
        }
    }

    for(int index = visible_items_count; index < last_visible_items_count; ++index)
    {
        hw::sprites::hide(handles[index]);
    }

    return visible_items_count;
}

bool _update_cameras_impl(sorted_sprites::layer& layer)
{
    bool check_items_on_screen = false;

    for(sprites_manager_item& item : layer)
    {
        if(item.camera)
        {
            item.update_hw_position();

            if(item.visible)
            {
                item.check_on_screen = true;
                check_items_on_screen = true;
            }
        }
    }

    return check_items_on_screen;
}

}

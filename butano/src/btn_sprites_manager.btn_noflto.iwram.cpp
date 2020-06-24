#include "btn_sprites_manager.h"

#include "btn_vector.h"
#include "btn_sorted_sprites.h"
#include "btn_sprites_manager_item.h"

namespace btn::sprites_manager
{

bool _check_items_on_screen_impl()
{
    fixed display_width = display::width();
    fixed display_height = display::height();
    bool rebuild_handles = false;

    for(auto& layer : sorted_sprites::layers())
    {
        for(sprites_manager_item& item : *layer)
        {
            if(item.check_on_screen)
            {
                fixed x = item.hw_position.x();
                bool on_screen = false;
                item.check_on_screen = false;

                if(x < display_width && x + (item.half_dimensions.width() * 2) > 0)
                {
                    fixed y = item.hw_position.y();

                    if(y < display_height && y + (item.half_dimensions.height() * 2) > 0)
                    {
                        on_screen = true;
                    }
                }

                if(on_screen != item.on_screen)
                {
                    item.on_screen = on_screen;
                    rebuild_handles = true;
                }
            }
        }
    }

    return rebuild_handles;
}

int _rebuild_handles_impl(int last_visible_items_count, void* hw_handles)
{
    auto handles = reinterpret_cast<hw::sprites::handle_type*>(hw_handles);
    int visible_items_count = 0;

    for(auto& layer : sorted_sprites::layers())
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
        }
    }

    int hide_handles_index = visible_items_count;

    while(hide_handles_index < last_visible_items_count)
    {
        hw::sprites::hide(handles[hide_handles_index]);
        ++hide_handles_index;
    }

    return visible_items_count;
}

#if BTN_CFG_CAMERA_ENABLED
    update_camera_impl_result _update_camera_impl(fixed_point camera_position)
    {
        update_camera_impl_result result;

        for(auto& layer : sorted_sprites::layers())
        {
            for(sprites_manager_item& item : *layer)
            {
                if(! item.ignore_camera)
                {
                    item.update_hw_position(camera_position);
                    result.rebuild_handles = true;

                    if(item.visible)
                    {
                        item.check_on_screen = true;
                        result.check_items_on_screen = true;
                    }
                }
            }
        }

        return result;
    }
#endif

}

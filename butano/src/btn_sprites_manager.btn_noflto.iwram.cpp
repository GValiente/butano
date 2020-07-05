#include "btn_sprites_manager.h"

#include "btn_sorted_sprites.h"

namespace btn::sprites_manager
{

bool _check_items_on_screen_impl(sorted_sprites::layer& layer)
{
    fixed display_width = display::width();
    fixed display_height = display::height();
    bool rebuild_handles = false;

    for(sprites_manager_item& item : layer)
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

    return rebuild_handles;
}

void _rebuild_handles_impl(sorted_sprites::layer& layer, void* hw_handles, int& visible_items_count)
{
    auto handles = reinterpret_cast<hw::sprites::handle_type*>(hw_handles);
    int local_visible_items_count = visible_items_count;

    for(sprites_manager_item& item : layer)
    {
        if(item.on_screen)
        {
            BTN_ASSERT(BTN_CFG_SPRITES_MAX_ITEMS <= sprites::sprites_count() ||
                       local_visible_items_count <= sprites::sprites_count(), "Too much sprites on screen");

            hw::sprites::copy_handle(item.handle, handles[local_visible_items_count]);
            item.handles_index = int8_t(local_visible_items_count);
            ++local_visible_items_count;
        }
    }

    visible_items_count = local_visible_items_count;
}

#if BTN_CFG_CAMERA_ENABLED
    void _update_camera_impl(fixed_point camera_position, sorted_sprites::layer& layer,
                             update_camera_impl_result& result)
    {
        for(sprites_manager_item& item : layer)
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
#endif

}

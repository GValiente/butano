#include "btn_sprites_manager.h"

#include "btn_display.h"
#include "btn_sorted_sprites.h"
#include "btn_sprites_manager_item.h"

namespace btn::sprites_manager
{

bool check_items_on_screen_impl()
{
    int display_width = display::width();
    int display_height = display::height();
    bool rebuild_handles = false;

    for(auto& layer : sorted_sprites::layers())
    {
        for(sprites_manager_item& item : layer.second)
        {
            if(item.check_on_screen)
            {
                int x = item.hw_position.x();
                bool on_screen = false;
                item.check_on_screen = false;

                if(x < display_width && x + (item.half_dimensions.width() * 2) > 0)
                {
                    int y = item.hw_position.y();

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

}

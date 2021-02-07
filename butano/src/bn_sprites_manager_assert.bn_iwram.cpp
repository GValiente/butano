/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites_manager.h"

#include "bn_config_sprites.h"

#if BN_CFG_SPRITES_MAX_ITEMS <= 128
    #define BN_ASSERT(condition, ...)

    #define BN_ERROR(...)
#endif

#include "bn_sorted_sprites.h"

namespace bn::sprites_manager
{

int _rebuild_handles_impl(int last_visible_items_count, void* hw_handles,
                          intrusive_list<sorted_sprites::layer>& layers)
{
    auto handles = reinterpret_cast<hw::sprites::handle_type*>(hw_handles);
    int visible_items_count = 0;

    for(sorted_sprites::layer& layer : layers)
    {
        for(sprites_manager_item& item : layer.items())
        {
            if(item.on_screen)
            {
                BN_ASSERT(visible_items_count <= hw::sprites::count(), "Too much on screen sprites");

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
        hw::sprites::hide_and_destroy(handles[index]);
    }

    return visible_items_count;
}

}

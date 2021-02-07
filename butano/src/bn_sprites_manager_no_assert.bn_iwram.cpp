/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites_manager.h"

#define BN_ASSERT(condition, ...)

#define BN_ERROR(...)

#include "bn_sorted_sprites.h"

namespace bn::sprites_manager
{

bool _check_items_on_screen_impl(void* hw_handles, intrusive_list<sorted_sprites::layer>& layers,
                                 bool rebuild_handles, int& first_index_to_commit, int& last_index_to_commit)
{
    auto handles = reinterpret_cast<hw::sprites::handle_type*>(hw_handles);
    int first_index = first_index_to_commit;
    int last_index = last_index_to_commit;

    for(sorted_sprites::layer& layer : layers)
    {
        for(sprites_manager_item& item : layer.items())
        {
            if(item.check_on_screen)
            {
                int x = item.hw_position.x();
                bool on_screen = false;
                item.check_on_screen = false;

                if(x < display::width())
                {
                    int y = item.hw_position.y();

                    if(y < display::height())
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

                if(item.on_screen != on_screen)
                {
                    item.on_screen = on_screen;

                    if(on_screen)
                    {
                        if(item.affine_mat)
                        {
                            hw::sprites::show_affine(item.double_size, item.handle);
                        }
                        else
                        {
                            hw::sprites::show_regular(item.handle);
                        }
                    }
                    else
                    {
                        hw::sprites::hide(item.handle);
                    }
                }

                if(! rebuild_handles)
                {
                    int handles_index = item.handles_index;

                    if(handles_index != -1)
                    {
                        hw::sprites::copy_handle(item.handle, handles[handles_index]);

                        if(handles_index < first_index)
                        {
                            first_index = handles_index;
                        }

                        if(handles_index > last_index)
                        {
                            last_index = handles_index;
                        }
                    }
                    else
                    {
                        rebuild_handles = true;
                    }
                }
            }
        }
    }

    first_index_to_commit = first_index;
    last_index_to_commit = last_index;
    return rebuild_handles;
}

bool _update_cameras_impl(intrusive_list<sorted_sprites::layer>& layers)
{
    bool check_items_on_screen = false;

    for(sorted_sprites::layer& layer : layers)
    {
        for(sprites_manager_item& item : layer.items())
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
    }

    return check_items_on_screen;
}

}

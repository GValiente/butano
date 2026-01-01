/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites_manager.h"

#include "bn_sprites_manager_hot.h"

namespace bn::sprites_manager
{

void _check_items_on_screen(intrusive_list<sorted_sprites::layer>& layers)
{
    hot::check_items_on_screen(layers);
}

int _rebuild_handles_impl(int reserved_handles_count, void* hw_handles, intrusive_list<sorted_sprites::layer>& layers)
{
    return hot::rebuild_handles(reserved_handles_count, hw_handles, layers);
}

bool _update_cameras_impl(intrusive_list<sorted_sprites::layer>& layers)
{
    return hot::update_cameras(layers);
}

}

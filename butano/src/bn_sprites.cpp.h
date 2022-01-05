/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprites.h"

#include "bn_sprites_manager.h"

namespace bn::sprites
{

int used_items_count()
{
    return sprites_manager::used_items_count();
}

int available_items_count()
{
    return sprites_manager::available_items_count();
}

int reserved_handles_count()
{
    return sprites_manager::reserved_handles_count();
}

void set_reserved_handles_count(int reserved_handles_count)
{
    return sprites_manager::set_reserved_handles_count(reserved_handles_count);
}

void reload()
{
    sprites_manager::reload_all();
}

}

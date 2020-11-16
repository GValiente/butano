/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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

}

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_tiles.h"

#include "bn_sprite_tiles_manager.h"

namespace bn::sprite_tiles
{

int used_tiles_count()
{
    return sprite_tiles_manager::used_tiles_count();
}

int available_tiles_count()
{
    return sprite_tiles_manager::available_tiles_count();
}

int used_items_count()
{
    return sprite_tiles_manager::used_items_count();
}

int available_items_count()
{
    return sprite_tiles_manager::available_items_count();
}

void log_status()
{
    #if BN_CFG_LOG_ENABLED
        sprite_tiles_manager::log_status();
    #endif
}

}

/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_tiles.h"

#include "bn_bg_blocks_manager.h"

namespace bn::bg_tiles
{

int used_tiles_count()
{
    return bg_blocks_manager::used_tiles_count();
}

int available_tiles_count()
{
    return bg_blocks_manager::available_tiles_count();
}

int used_blocks_count()
{
    return bg_blocks_manager::used_tile_blocks_count();
}

int available_blocks_count()
{
    return bg_blocks_manager::available_tile_blocks_count();
}

bool allow_offset()
{
    return bg_blocks_manager::allow_tiles_offset();
}

void set_allow_offset(bool allow_offset)
{
    bg_blocks_manager::set_allow_tiles_offset(allow_offset);
}

void log_status()
{
    #if BN_CFG_LOG_ENABLED
        bg_blocks_manager::log_status();
    #endif
}

}

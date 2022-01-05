/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_maps.h"

#include "bn_bg_blocks_manager.h"

namespace bn::bg_maps
{

int used_cells_count()
{
    return bg_blocks_manager::used_map_cells_count();
}

int available_cells_count()
{
    return bg_blocks_manager::available_map_cells_count();
}

int used_blocks_count()
{
    return bg_blocks_manager::used_map_blocks_count();
}

int available_blocks_count()
{
    return bg_blocks_manager::available_map_blocks_count();
}

#if BN_CFG_LOG_ENABLED
    void log_status()
    {
        bg_blocks_manager::log_status();
    }
#endif

}

/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
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

affine_bg_big_map_canvas_size new_affine_big_map_canvas_size()
{
    return bg_blocks_manager::new_affine_big_map_canvas_size();
}

void set_new_affine_big_map_canvas_size(affine_bg_big_map_canvas_size affine_big_map_canvas_size)
{
    bg_blocks_manager::set_new_affine_big_map_canvas_size(affine_big_map_canvas_size);
}

void log_status()
{
    #if BN_CFG_LOG_ENABLED
        bg_blocks_manager::log_status();
    #endif
}

}

#include "btn_bg_maps.h"

#include "btn_bg_blocks_manager.h"

namespace btn::bg_maps
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

}
